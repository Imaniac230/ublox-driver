#ifndef PACKET_BASE_H
#define PACKET_BASE_H

#include <list>

#include <ublox/Registers.hpp>
#include <util/Serde.hpp>

namespace UBLOX::Packet {
    //TODO(constants): define constants according to the data structure (header size, crc size, min packet size, etc.)
    class Base {
    public:
        typedef std::vector<uint8_t> SerializedType;

        Base(Base &&other) noexcept : serializedContents(std::move(other.serializedContents)) {}
        Base(const MessageClass messageClass, const uint8_t messageId, std::vector<uint8_t> data)
            : serializedContents({static_cast<uint8_t>(SyncChar::FirstByte), static_cast<uint8_t>(SyncChar::SecondByte),
                                  static_cast<uint8_t>(messageClass), messageId}) {
            std::vector<uint8_t> length = Serde::serializeLEInt(static_cast<uint16_t>(data.size()));
            serializedContents.resize(serializedContents.size() + length.size());
            std::move(length.begin(), length.end(), serializedContents.end() - static_cast<int>(length.size()));
            serializedContents.resize(serializedContents.size() + data.size());
            std::move(data.begin(), data.end(), serializedContents.end() - static_cast<int>(data.size()));
            appendChecksum();
        }
        Base(const Message message, std::vector<uint8_t> data)
            : Base(toClass(message), toRawId(message), std::move(data)) {}
        explicit Base(const Message message) : Base(message, {}) {}

        virtual ~Base() = default;

        [[nodiscard]] const SerializedType &serialized() const { return serializedContents; }
        [[nodiscard]] size_t size() const { return serializedContents.size(); }
        [[nodiscard]] inline uint16_t dataSize() const {
            return Serde::deserializeLEInt<uint16_t>(
                    std::vector<uint8_t>{serializedContents[4], serializedContents[5]}.data());
        }
        [[nodiscard]] inline bool checksumValid(const std::array<uint8_t, 2> &received) const {
            return (*(serializedContents.end() - 2) == received[0]) && (*(serializedContents.end() - 1) == received[1]);
        }
        [[nodiscard]] bool empty() const { return serializedContents.empty(); }
        [[nodiscard]] inline Message message() const {
            return static_cast<Message>(static_cast<MessageClass>(serializedContents[2]) + serializedContents[3]);
        }
        [[nodiscard]] inline std::vector<uint8_t> rawData() const {
            return {serializedContents.begin() + 6, serializedContents.begin() + 6 + dataSize()};
        }

    protected:
        inline void appendChecksum() {
            const std::array<uint8_t, 2> crc = calculateChecksum(serializedContents);
            serializedContents.insert(serializedContents.end(), crc.begin(), crc.end());
        }

    private:
        inline static std::array<uint8_t, 2> calculateChecksum(const SerializedType &buffer) {
            if (buffer.size() < 3) return {};

            uint8_t ckA = 0, ckB = 0;
            auto it = buffer.begin();
            if ((*it == SyncChar::FirstByte) && (*(it + 1) == SyncChar::SecondByte)) std::advance(it, 2);
            for (; it != buffer.end(); ++it) {
                ckA += *it;
                ckB += ckA;
            }

            return {ckA, ckB};
        }

        SerializedType serializedContents;
    };
}// namespace UBLOX::Packet


#endif//PACKET_BASE_H
