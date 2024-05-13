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

    class Status : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavReceiverNavigationStatus;
        struct Data {
            enum class FixType : uint8_t {
                None = 0x00,
                DeadReckoning = 0x01,
                Fix2D = 0x02,
                Fix3D = 0x03,
                GnssAndDeadReckoning = 0x04,
                Time = 0x05
            };
            enum class Flag : uint8_t {
                None = 0x00,
                FixValid = 0x01,
                DifferentialCorrectionsApplied = 0x02,
                WeekNumberValid = 0x04,
                TimeOfWeekValid = 0x08,
            };
            enum class MapMatchingStatus {
                None = 0,
                ValidNotUsed = 1,
                ValidUsed = 2,
                ValidUsedDeadReckoning = 3//TODO: verify what this one should mean
            };
            struct __attribute__((__packed__)) __attribute__((aligned(1))) FixStatus {
                uint8_t differentialCorrectionsAvailable : 1 = 0;
                uint8_t carrierPhaseRangeSolutionValid : 1 = 0;
                uint8_t reserved : 4 = 0;
                uint8_t mapMatchingStatus : 2 = static_cast<uint8_t>(MapMatchingStatus::None);
            };
            enum class PowerSaveModeState {
                AcquisitionOrDisabled = 0,
                Tracking = 1,
                PowerOptimizedTracking = 2,
                Inactive = 3
            };
            enum class SpoofingDetectionState {
                UnknownOrDeactivated = 0,
                NoSpoofing = 1,
                Spoofing = 2,
                MultipleSpoofing = 3
            };
            enum class CarrierPhaseRangeSolutionStatus {
                NoSolution = 0,
                WithFloatingAmbiguities = 1,
                WithFixedAmbiguities = 2
            };
            struct __attribute__((__packed__)) __attribute__((aligned(1))) Flag2 {
                uint8_t powerSaveModeState : 2 = static_cast<uint8_t>(PowerSaveModeState::AcquisitionOrDisabled);
                uint8_t reserved1 : 1 = 0;
                uint8_t spoofingDetectionState : 2 = static_cast<uint8_t>(SpoofingDetectionState::UnknownOrDeactivated);
                uint8_t reserved2 : 1 = 0;
                uint8_t carrierPhaseRangeSolutionStatus : 2 =
                        static_cast<uint8_t>(CarrierPhaseRangeSolutionStatus::NoSolution);
            };

            uint32_t iTOWTimestampMillis = 0;
            FixType fixType = FixType::None;
            uint8_t flags = 0;
            FixStatus fixStatus{};
            Flag2 flags2{};
            uint32_t timeToFirstFixMillis = 0;
            uint32_t elapsedSinceStartupMillis = 0;
        };

        explicit Status(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            static constexpr uint16_t skippedDataOffset = sizeof(Data::FixStatus) + sizeof(Data::Flag2);
            data = Data{
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(&raw[offset]),
                    .fixType = static_cast<Data::FixType>(
                            Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint32_t)])),
                    .flags = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(Data::FixType)]),
                    .timeToFirstFixMillis =
                            Serde::deserializeLEInt<uint32_t>(&raw[offset + sizeof(Data::FixType) + skippedDataOffset]),
                    .elapsedSinceStartupMillis = Serde::deserializeLEInt<uint32_t>(
                            &raw[offset + sizeof(Data::FixType) + skippedDataOffset + sizeof(uint32_t)]),
            };
            const auto fixStatus = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(Data::Flag)]);
            data.fixStatus = *reinterpret_cast<const Data::FixStatus *>(&fixStatus);
            const auto flags2 = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(Data::FixStatus)]);
            data.flags2 = *reinterpret_cast<const Data::Flag2 *>(&flags2);
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };

    class SurveyInData : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavSurveyInData;
        struct Data {
            uint8_t version = 0;
            //3 bytes reserved
            uint32_t iTOWTimestampMillis = 0;
            uint32_t observationDurationSecs = 0;
            int32_t meanXEcefCm = 0;
            int32_t meanYEcefCm = 0;
            int32_t meanZEcefCm = 0;
            int8_t meanHighPrecisionXEcef = 0;//given in 0.1 of mm -> meanXEcefCm + (0.01 * meanHighPrecisionXEcef)
            int8_t meanHighPrecisionYEcef = 0;//given in 0.1 of mm -> meanYEcefCm + (0.01 * meanHighPrecisionYEcef)
            int8_t meanHighPrecisionZEcef = 0;//given in 0.1 of mm -> meanZEcefCm + (0.01 * meanHighPrecisionZEcef)
            //1 byte reserved
            uint32_t meanPositionAccuracy = 0;//given in 0.1 of mm
            uint32_t numberOfUsedObservations = 0;
            bool valid = false;
            bool inProgress = false;
            //2 bytes reserved
        };

        explicit SurveyInData(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(
                            &raw[offset += sizeof(uint8_t) + 3 * sizeof(uint8_t)]),//NOTE: must skip 3 reserved bytes
                    .observationDurationSecs = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .meanXEcefCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(uint32_t)]),
                    .meanYEcefCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .meanZEcefCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .meanHighPrecisionXEcef = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int32_t)]),
                    .meanHighPrecisionYEcef = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int8_t)]),
                    .meanHighPrecisionZEcef = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int8_t)]),
                    .meanPositionAccuracy = Serde::deserializeLEInt<uint32_t>(
                            &raw[offset += sizeof(int8_t) + sizeof(uint8_t)]),//NOTE: must skip 1 reserved byte
                    .numberOfUsedObservations = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .valid = static_cast<bool>(Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint32_t)])),
                    .inProgress = static_cast<bool>(Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)])),
            };
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet::Nav

#endif//NAVIGATION_PACKET_H
