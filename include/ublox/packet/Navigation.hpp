#ifndef NAVIGATION_PACKET_H
#define NAVIGATION_PACKET_H

#include <ublox/packet/Base.hpp>

namespace UBLOX::Packet::Nav {
    class PositionECEF : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavEcefPositionSolution;
        struct Data {
            uint32_t iTOWTimestampMillis = 0;
            int32_t XCm = 0;
            int32_t YCm = 0;
            int32_t ZCm = 0;
            uint32_t positionAccuracyCm = 0;
        };

        explicit PositionECEF(Base &&raw) : Base(std::move(raw)) {}

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

    class PositionLLH : public Base {
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

        explicit PositionLLH(Base &&raw) : Base(std::move(raw)) {}

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

    class Odometer : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavOdometerSolution;
        struct Data {
            uint8_t version = 0;
            //3 bytes reserved
            uint32_t iTOWTimestampMillis = 0;
            uint32_t resetGroundDistanceM = 0;
            uint32_t coldStartGroundDistanceM = 0;
            uint32_t resetGroundDistanceStd = 0;
        };

        explicit Odometer(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(
                            &raw[offset += sizeof(uint8_t) + 3 * sizeof(uint8_t)]),//NOTE: must skip 3 reserved bytes
                    .resetGroundDistanceM = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .coldStartGroundDistanceM = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .resetGroundDistanceStd = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
            };
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet::Nav

#endif//NAVIGATION_PACKET_H
