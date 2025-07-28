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
            uint8_t version = 0x00;
            //3 bytes (uint8_t) reserved
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
            enum class MapMatchingStatus : uint8_t {
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
            enum class PowerSaveModeState : uint8_t {
                AcquisitionOrDisabled = 0,
                Tracking = 1,
                PowerOptimizedTracking = 2,
                Inactive = 3
            };
            enum class SpoofingDetectionState : uint8_t {
                UnknownOrDeactivated = 0,
                NoSpoofing = 1,
                Spoofing = 2,
                MultipleSpoofing = 3
            };
            enum class CarrierPhaseRangeSolutionStatus : uint8_t {
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
            uint8_t version = 0x00;
            //3 bytes (uint8_t) reserved
            uint32_t iTOWTimestampMillis = 0;
            uint32_t observationDurationSecs = 0;
            int32_t meanXEcefCm = 0;
            int32_t meanYEcefCm = 0;
            int32_t meanZEcefCm = 0;
            int8_t meanHighPrecisionXEcef = 0;//mm = * 0.1 -> meanXEcefCm + (0.01 * meanHighPrecisionXEcef)
            int8_t meanHighPrecisionYEcef = 0;//mm = * 0.1 -> meanYEcefCm + (0.01 * meanHighPrecisionYEcef)
            int8_t meanHighPrecisionZEcef = 0;//mm = * 0.1 -> meanZEcefCm + (0.01 * meanHighPrecisionZEcef)
            //1 byte (uint8_t) reserved
            uint32_t meanPositionAccuracy = 0;//mm = * 0.1
            uint32_t numberOfUsedObservations = 0;
            bool valid = false;
            bool inProgress = false;
            //2 bytes (uint8_t) reserved
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

    class SatelliteInfo : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavSatelliteInformation;
        struct Data {
            struct Satellite {
                enum class GNSSIdentifier : uint8_t {
                    Gps = 0,
                    Sbas = 1,
                    Galileo = 2,
                    BeiDou = 3,
                    Qzss = 5,
                    Glonass = 6,
                    NavIc = 7
                };
                enum class SignalQuality : uint8_t {
                    NoSignal = 0,
                    Searching = 1,
                    Acquired = 2,
                    DetectedUnusable = 3,
                    CodeLockedTimeSynchronized = 4,
                    CodeAndCarrier1LockedTimeSynchronized = 5,
                    CodeAndCarrier2LockedTimeSynchronized = 6,
                    CodeAndCarrier3LockedTimeSynchronized = 7
                };
                enum class Health : uint8_t { Unknown = 0, Healthy = 1, Unhealthy = 2 };
                enum class OrbitSource : uint8_t {
                    Unavailable = 0,
                    Ephemeris = 1,
                    Almanac = 2,
                    AssistNowOffline = 3,
                    AssistNowAutonomous = 4,
                    Other1 = 5,
                    Other2 = 6,
                    Other3 = 7
                };
                struct __attribute__((__packed__)) __attribute__((aligned(1))) Flags {
                    uint32_t signalQuality : 3 = static_cast<uint8_t>(SignalQuality::NoSignal);
                    uint32_t usedForNavigation : 1 = 0;
                    uint32_t health : 2 = static_cast<uint8_t>(Health::Unknown);
                    uint32_t differentialCorrectionsAvailable : 1 = 0;
                    uint32_t carrierSmoothedPseudorangeUsed : 1 = 0;
                    uint32_t orbitSource : 3 = static_cast<uint8_t>(OrbitSource::Unavailable);
                    uint32_t ephemerisAvailable : 1 = 0;
                    uint32_t almanacAvailable : 1 = 0;
                    uint32_t assistNowOfflineDataAvailable : 1 = 0;
                    uint32_t assistNowAutonomousDataAvailable : 1 = 0;
                    uint32_t reserved1 : 1 = 0;
                    uint32_t sbasCorrectionsUsed : 1 = 0;
                    uint32_t rtcmCorrectionsUsed : 1 = 0;
                    uint32_t qzssSlasCorrectionsUsed : 1 = 0;
                    uint32_t spartnCorrectionsUsed : 1 = 0;
                    uint32_t pseudorangeCorrectionsUsed : 1 = 0;
                    uint32_t carrierRangeCorrectionsUsed : 1 = 0;
                    uint32_t dopplerCorrectionsUsed : 1 = 0;
                    uint32_t clasCorrectionsUsed : 1 = 0;
                    uint32_t reserved2 = 0;
                };

                GNSSIdentifier gnssID = GNSSIdentifier::Gps;
                uint8_t satelliteID = 0;
                uint8_t signalStrength = 0;
                int8_t elevationDeg = 0;          //range: +/-90
                int16_t azimuthDeg = 0;           //range: 0-360
                int16_t pseudorangeResidualDm = 0;//TODO: verify unit
                Flags flags{};
            };

            uint32_t iTOWTimestampMillis = 0;
            uint8_t version = 0x01;
            uint8_t numberOfSatellites = 0;
            //2 bytes (uint8_t) reserved
            std::list<Satellite> satellites{};
        };

        explicit SatelliteInfo(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(&raw[offset]),
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint32_t)]),
                    .numberOfSatellites = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]),
            };
            offset += 2 * sizeof(uint8_t);//NOTE: must skip 2 reserved bytes
            for (uint8_t satellite = 0; satellite < data.numberOfSatellites; ++satellite) {
                data.satellites.push_back(Data::Satellite{
                        .gnssID = static_cast<Data::Satellite::GNSSIdentifier>(
                                Serde::deserializeLEInt<uint8_t>(&raw[offset])),
                        .satelliteID = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]),
                        .signalStrength = Serde::deserializeLEInt<uint8_t>(&raw[offset += sizeof(uint8_t)]),
                        .elevationDeg = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(uint8_t)]),
                        .azimuthDeg = Serde::deserializeLEInt<int16_t>(&raw[offset += sizeof(int8_t)]),
                        .pseudorangeResidualDm = Serde::deserializeLEInt<int16_t>(&raw[offset += sizeof(int16_t)])});
                const auto flags = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(int16_t)]);
                data.satellites.back().flags = *reinterpret_cast<const Data::Satellite::Flags *>(&flags);
                offset += sizeof(uint32_t);
            }
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };

    class RelativePositionNED : public Base {
    public:
        static constexpr Message MESSAGE = Message::NavRelativePositioningInformation;
        struct Data {
            enum class CarrierPhaseRangeSolutionStatus : uint8_t {
                NoSolution = 0,
                WithFloatingAmbiguities = 1,
                WithFixedAmbiguities = 2
            };
            struct __attribute__((__packed__)) __attribute__((aligned(1))) Flags {
                uint32_t gnssFixValid : 1 = 0;
                uint32_t differentialCorrectionsApplied : 1 = 0;
                uint32_t dataAndBaselineValid : 1 = 0;
                uint32_t carrierPhaseRangeSolutionStatus : 2 =
                        static_cast<uint8_t>(CarrierPhaseRangeSolutionStatus::NoSolution);
                uint32_t isMovingBase : 1 = 0;
                uint32_t referencePositionUsedForMovingBase : 1 = 0;
                uint32_t referenceObservationsUsedForMovingBase : 1 = 0;
                uint32_t vectorHeadingValid : 1 = 0;
                uint32_t vectorComponentsNormalized : 1 = 0;
                uint32_t reserved = 0;
            };

            uint8_t version = 0x01;
            //1 byte (uint8_1) reserved
            uint16_t referenceStationId = 0;
            uint32_t iTOWTimestampMillis = 0;
            int32_t northCm = 0;
            int32_t eastCm = 0;
            int32_t downCm = 0;
            int32_t vectorLengthCm = 0;
            int32_t vectorHeading = 0;// degree = * 1e-5
            //4 bytes (uint8_t) reserved
            int8_t highPrecisionNorth = 0;       //mm = * 0.1 -> northCm + (0.01 * highPrecisionNorth)
            int8_t highPrecisionEast = 0;        //mm = * 0.1 -> eastCm + (0.01 * highPrecisionEast)
            int8_t highPrecisionDown = 0;        //mm = * 0.1 -> downCm + (0.01 * highPrecisionDown)
            int8_t highPrecisionVectorLength = 0;//mm = * 0.1 -> vectorLengthCm + (0.01 * highPrecisionVectorLength)
            uint32_t accuracyNorth = 0;          //mm = * 0.1
            uint32_t accuracyEast = 0;           //mm = * 0.1
            uint32_t accuracyDown = 0;           //mm = * 0.1
            uint32_t accuracyVectorLength = 0;   //mm = * 0.1
            uint32_t accuracyVectorHeading = 0;  //degree = * 1e-5
            //4 bytes (uint8_t) reserved
            Flags flags{};
        };

        explicit RelativePositionNED(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            uint16_t offset = 0;
            data = Data{
                    .version = Serde::deserializeLEInt<uint8_t>(&raw[offset]),
                    .referenceStationId = Serde::deserializeLEInt<uint16_t>(
                            &raw[offset += sizeof(uint8_t) + sizeof(uint8_t)]),//NOTE: must skip 1 reserved byte
                    .iTOWTimestampMillis = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint16_t)]),
                    .northCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(uint32_t)]),
                    .eastCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .downCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .vectorLengthCm = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .vectorHeading = Serde::deserializeLEInt<int32_t>(&raw[offset += sizeof(int32_t)]),
                    .highPrecisionNorth = Serde::deserializeLEInt<int8_t>(
                            &raw[offset += sizeof(int32_t) + 4 * sizeof(uint8_t)]),//NOTE: must skip 4 reserved bytes
                    .highPrecisionEast = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int8_t)]),
                    .highPrecisionDown = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int8_t)]),
                    .highPrecisionVectorLength = Serde::deserializeLEInt<int8_t>(&raw[offset += sizeof(int8_t)]),
                    .accuracyNorth = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(int8_t)]),
                    .accuracyEast = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .accuracyDown = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .accuracyVectorLength = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
                    .accuracyVectorHeading = Serde::deserializeLEInt<uint32_t>(&raw[offset += sizeof(uint32_t)]),
            };
            const auto flags = Serde::deserializeLEInt<uint32_t>(
                    &raw[offset += sizeof(uint32_t) + 4 * sizeof(uint8_t)]);//NOTE: must skip 4 reserved bytes
            data.flags = *reinterpret_cast<const Data::Flags *>(&flags);
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet::Nav

#endif//NAVIGATION_PACKET_H
