#ifndef MONITOR_PACKET_H
#define MONITOR_PACKET_H

#include <ublox/packet/Base.hpp>

namespace UBLOX::Packet::Mon {
    class ReceiverAndSoftwareVersion : public Base {
    public:
        static constexpr Message MESSAGE = Message::MonReceiverAndSoftwareVersion;
        struct Data {
            std::array<uint8_t, 30> swVersion{};
            std::array<uint8_t, 10> hwVersion{};
            std::vector<std::array<uint8_t, 30>> extensions{};
        };

        explicit ReceiverAndSoftwareVersion(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            data = Data{};
            std::move(raw.begin(), raw.begin() + 30, data.swVersion.begin());
            std::move(raw.begin() + 30, raw.begin() + 30 + 10, data.hwVersion.begin());
            int dataStart = 30 + 10;
            while (dataStart < dataSize()) {
                std::array<uint8_t, 30> extension{};
                std::move(raw.begin() + dataStart, raw.begin() + dataStart + 30, extension.begin());
                data.extensions.emplace_back(extension);
                dataStart += 30;
            }
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };

    class GNSSInformation : public Base {
    public:
        static constexpr Message MESSAGE = Message::MonGnssInformationMessage;
        struct Data {
            struct __attribute__((__packed__)) __attribute__((aligned(1))) Flag {
                uint8_t gps : 1 = 0;
                uint8_t glonass : 1 = 0;
                uint8_t beidou : 1 = 0;
                uint8_t galileo : 1 = 0;
                uint8_t reserved = 0;
            };

            uint8_t version = 0x00;
            Flag supportedGnss{};
            Flag defaultGnss{};
            Flag enabledGnss{};
            uint8_t maximumConcurrentGnss = 0;
            //3 bytes (uint8_1) reserved
        };

        explicit GNSSInformation(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            static constexpr uint16_t skippedDataOffset = 3 * sizeof(Data::Flag);
            data = Data{
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                    .maximumConcurrentGnss =
                            Serde::deserializeLEInt<uint8_t>(&raw[offset + sizeof(uint8_t) + skippedDataOffset]),
            };
            const auto supportedGnss = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]);
            data.supportedGnss = *reinterpret_cast<const Data::Flag *>(&supportedGnss);
            const auto defaultGnss = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(Data::Flag)]);
            data.defaultGnss = *reinterpret_cast<const Data::Flag *>(&defaultGnss);
            const auto enabledGnss = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(Data::Flag)]);
            data.enabledGnss = *reinterpret_cast<const Data::Flag *>(&enabledGnss);
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet::Mon

#endif//MONITOR_PACKET_H
