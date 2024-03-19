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
            data = Data{
                    .iTOWTimestampMillis = Utils::deserializeLEInt<uint32_t>(&raw[0]),
                    .XCm = Utils::deserializeLEInt<int32_t>(&raw[4]),
                    .YCm = Utils::deserializeLEInt<int32_t>(&raw[8]),
                    .ZCm = Utils::deserializeLEInt<int32_t>(&raw[12]),
                    .positionAccuracyCm = Utils::deserializeLEInt<uint32_t>(&raw[16]),
            };
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet

#endif//NAVPOSITIONECEFPAKCET_H
