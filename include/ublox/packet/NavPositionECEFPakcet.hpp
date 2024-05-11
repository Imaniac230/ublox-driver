#ifndef NAVPOSITIONECEFPAKCET_H
#define NAVPOSITIONECEFPAKCET_H

#include <ublox/packet/Packet.hpp>

namespace UBLOX::Packet {
    class NavPositionECEF : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavEcefPositionSolution;
        struct Data {
            uint32_t iTOWTimestampMillis = 0;
            int32_t XCm = 0;
            int32_t YCm = 0;
            int32_t ZCm = 0;
            uint32_t positionAccuracyCm = 0;
        };

        explicit NavPositionECEF(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(&raw[offset]),
                    .XCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(uint32_t)]),
                    .YCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .ZCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .positionAccuracyCm = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(int32_t)]),
            };
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet

#endif//NAVPOSITIONECEFPAKCET_H
