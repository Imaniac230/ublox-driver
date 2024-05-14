#ifndef RECEIVER_PACKET_H
#define RECEIVER_PACKET_H

#include <ublox/packet/Base.hpp>

namespace UBLOX::Packet::Rxm {
    enum class StatusFlag : uint8_t { Unknown = 0, Negative = 1, Positive = 2 };

    class DifferentialCorrectionsStatus : public Base {
    public:
        static constexpr Message MESSAGE = Message::RxmDifferentialCorrectionInputStatus;
        struct Data {
            enum class Protocol : uint8_t { Unknown = 0, Rtcm3 = 1, Spartn = 2, PointToMultipoint = 29, QzssL6 = 30 };
            struct __attribute__((__packed__)) __attribute__((aligned(1))) Status {
                uint32_t protocol : 5 = static_cast<uint8_t>(Protocol::Unknown);
                uint32_t error : 2 = static_cast<uint8_t>(StatusFlag::Unknown);
                uint32_t used : 2 = static_cast<uint8_t>(StatusFlag::Unknown);
                uint32_t correctionId : 16 = 0xFFFF;
                uint32_t typeValid : 1 = 0;
                uint32_t subTypeValid : 1 = 0;
                uint32_t inputHandlingSupported : 1 = 0;
                uint32_t encrypted : 2 = static_cast<uint8_t>(StatusFlag::Unknown);
                uint32_t decrypted : 2 = static_cast<uint8_t>(StatusFlag::Unknown);
            };

            uint8_t version = 0x01;
            uint8_t energyPerBitToNoisePowerSd = 0;//valid only for Point to Multipoint SPARTN
            //2 bytes (uint8_t) reserved
            Status status{};
            uint16_t type = 0;
            uint16_t subType = 0;
        };

        explicit DifferentialCorrectionsStatus(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            //NOTE: must also skip 2 reserved bytes
            static constexpr uint16_t skippedDataOffset = sizeof(Data::Status) + 2 * sizeof(uint8_t);
            data = Data{
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                    .energyPerBitToNoisePowerSd = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]),
                    .type = Serde::deserializeLEInt<uint16_t>(&raw[offset + sizeof(uint8_t) + skippedDataOffset]),
                    .subType = Serde::deserializeLEInt<uint16_t>(
                            &raw[offset + sizeof(uint8_t) + skippedDataOffset + sizeof(uint16_t)]),
            };
            const auto status = Serde::deserializeLEInt<uint32_t>(
                    &raw[offset += sizeof(uint8_t) + 2 * sizeof(uint8_t)]);//NOTE: must skip 2 reserved bytes
            data.status = *reinterpret_cast<const Data::Status *>(&status);
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };

    class RTCMStatus : public Base {
    public:
        static constexpr Message MESSAGE = Message::RxmRtcmInputStatus;
        struct Data {
            struct __attribute__((__packed__)) __attribute__((aligned(1))) Status {
                uint8_t crcFailed : 1 = 0;
                uint8_t used : 2 = static_cast<uint8_t>(StatusFlag::Unknown);
                uint8_t reserved = 0;
            };

            uint8_t version = 0x02;
            Status status{};
            uint16_t subType = 0;//only valid for proprietary rtcm4072
            uint16_t referenceStationId = 0;
        };

        explicit RTCMStatus(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            static constexpr uint16_t skippedDataOffset = sizeof(Data::Status);
            data = Data{.version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                        .subType =
                                Serde::deserializeLEInt<uint16_t>(&raw[offset + sizeof(uint8_t) + skippedDataOffset]),
                        .referenceStationId = Serde::deserializeLEInt<uint16_t>(
                                &raw[offset + sizeof(uint8_t) + skippedDataOffset + sizeof(uint16_t)])};
            const auto status = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]);
            data.status = *reinterpret_cast<const Data::Status *>(&status);
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet::Rxm

#endif//RECEIVER_PACKET_H
