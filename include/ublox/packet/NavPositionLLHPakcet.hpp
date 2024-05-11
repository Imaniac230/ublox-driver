#ifndef NAVPOSITIONLLHPAKCET_H
#define NAVPOSITIONLLHPAKCET_H

#include <ublox/packet/Packet.hpp>

namespace UBLOX::Packet {
    class NavPositionLLH : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavGeodeticPositionSolution;
        struct Data {
            uint32_t iTOWTimestampMillis = 0;
            int32_t longitude = 0;
            int32_t latitude = 0;
            int32_t ellipsoidHeightMm = 0;
            int32_t MSLHeightMm = 0;
            uint32_t horizontalAccuracyMm = 0;
            uint32_t verticalAccuracyMm = 0;
        };

        explicit NavPositionLLH(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(&raw[offset]),
                    .longitude = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(uint32_t)]),
                    .latitude = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .ellipsoidHeightMm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .MSLHeightMm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .horizontalAccuracyMm = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(int32_t)]),
                    .verticalAccuracyMm = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
            };
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet

#endif//NAVPOSITIONLLHPAKCET_H
