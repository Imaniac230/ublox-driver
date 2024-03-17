#ifndef PACKET_H
#define PACKET_H

#include <list>

#include <ublox/Registers.hpp>

#include <util/Utils.hpp>

namespace UBLOX {
    class Packet {
    public:
        typedef std::vector<uint8_t> SerializedType;

        Packet(const MessageClass messageClass, const uint8_t messageId, std::vector<uint8_t> data)
            : serializedContents({static_cast<uint8_t>(SyncChar::FirstByte), static_cast<uint8_t>(SyncChar::SecondByte),
                                  static_cast<uint8_t>(messageClass), messageId}) {
            const std::vector<uint8_t> length = Utils::serializeLEInt(static_cast<uint16_t>(data.size()));
            serializedContents.insert(serializedContents.end(), length.begin(), length.end());
            const auto d = std::move(data);
            serializedContents.insert(serializedContents.end(), d.begin(), d.end());
            appendChecksum();
        }
        Packet(const Message message, std::vector<uint8_t> data)
            : Packet(toClass(message), toRawId(message), std::move(data)) {}
        explicit Packet(const Message message) : Packet(message, {}) {}

        virtual ~Packet() = default;

        [[nodiscard]] const SerializedType &serialized() const { return serializedContents; }
        [[nodiscard]] size_t size() const { return serializedContents.size(); }
        [[nodiscard]] inline uint16_t dataSize() const {
            return Utils::deserializeLEInt<uint16_t>(
                    std::vector<uint8_t>{serializedContents[4], serializedContents[5]}.data());
        }
        [[nodiscard]] inline bool checksumValid(const std::array<uint8_t, 2> &received) const {
            return (*(serializedContents.end() - 2) == received[0]) && (*(serializedContents.end() - 1) == received[1]);
        }
        [[nodiscard]] bool empty() const { return serializedContents.empty(); }
        [[nodiscard]] inline Message message() const {
            return static_cast<Message>(static_cast<MessageClass>(serializedContents[2]) + serializedContents[3]);
        }
        [[nodiscard]] inline std::vector<uint8_t> data() const {
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
            for (auto it = std::next(buffer.begin(), 2); it != buffer.end(); ++it) {
                ckA += *it;
                ckB += ckA;
            }

            return {ckA, ckB};
        }

        SerializedType serializedContents;
    };
}// namespace UBLOX

#endif//PACKET_H
