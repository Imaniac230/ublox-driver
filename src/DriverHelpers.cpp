#include <DriverHelpers.hpp>

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::FixType &fix) {
    switch (fix) {
        case UBLOX::Packet::Nav::Status::Data::FixType::None:
            stream << "None";
            break;
        case UBLOX::Packet::Nav::Status::Data::FixType::DeadReckoning:
            stream << "DeadReckoning";
            break;
        case UBLOX::Packet::Nav::Status::Data::FixType::Fix2D:
            stream << "Fix2D";
            break;
        case UBLOX::Packet::Nav::Status::Data::FixType::Fix3D:
            stream << "Fix3D";
            break;
        case UBLOX::Packet::Nav::Status::Data::FixType::GnssAndDeadReckoning:
            stream << "GnssAndDeadReckoning";
            break;
        case UBLOX::Packet::Nav::Status::Data::FixType::Time:
            stream << "Time";
            break;
        default:
            stream << "UNKNOWN";
    }
    return stream;
}

std::string flagsStr(const uint8_t flag) {
    std::string out("{");
    if (flag & static_cast<uint8_t>(UBLOX::Packet::Nav::Status::Data::Flag::None)) out = +" None,";
    if (flag & static_cast<uint8_t>(UBLOX::Packet::Nav::Status::Data::Flag::FixValid)) out += " FixValid,";
    if (flag & static_cast<uint8_t>(UBLOX::Packet::Nav::Status::Data::Flag::DifferentialCorrectionsApplied))
        out += " DifferentialCorrectionsApplied,";
    if (flag & static_cast<uint8_t>(UBLOX::Packet::Nav::Status::Data::Flag::WeekNumberValid))
        out += " WeekNumberValid,";
    if (flag & static_cast<uint8_t>(UBLOX::Packet::Nav::Status::Data::Flag::TimeOfWeekValid))
        out += " TimeOfWeekValid,";
    out += " }";
    return out;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::FixStatus &status) {
    stream << "{";
    stream << " differential corrections " << ((status.differentialCorrectionsAvailable) ? "AVAILABLE" : "UNAVAILABLE")
           << ", carrier phase range solution " << ((status.carrierPhaseRangeSolutionValid) ? "VALID" : "INVALID")
           << ", map matching: ";
    switch (static_cast<UBLOX::Packet::Nav::Status::Data::MapMatchingStatus>(status.mapMatchingStatus)) {
        case UBLOX::Packet::Nav::Status::Data::MapMatchingStatus::None:
            stream << "None";
            break;
        case UBLOX::Packet::Nav::Status::Data::MapMatchingStatus::ValidNotUsed:
            stream << "ValidNotUsed";
            break;
        case UBLOX::Packet::Nav::Status::Data::MapMatchingStatus::ValidUsed:
            stream << "ValidUsed";
            break;
        case UBLOX::Packet::Nav::Status::Data::MapMatchingStatus::ValidUsedDeadReckoning:
            stream << "ValidUsedDeadReckoning";
            break;
    }
    stream << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::Flag2 &flag) {
    stream << "{ power save mode: ";
    switch (static_cast<UBLOX::Packet::Nav::Status::Data::PowerSaveModeState>(flag.powerSaveModeState)) {
        case UBLOX::Packet::Nav::Status::Data::PowerSaveModeState::AcquisitionOrDisabled:
            stream << "AcquisitionOrDisabled";
            break;
        case UBLOX::Packet::Nav::Status::Data::PowerSaveModeState::Tracking:
            stream << "Tracking";
            break;
        case UBLOX::Packet::Nav::Status::Data::PowerSaveModeState::PowerOptimizedTracking:
            stream << "PowerOptimizedTracking";
            break;
        case UBLOX::Packet::Nav::Status::Data::PowerSaveModeState::Inactive:
            stream << "Inactive";
            break;
    }
    stream << ", spoofing detection: ";
    switch (static_cast<UBLOX::Packet::Nav::Status::Data::SpoofingDetectionState>(flag.spoofingDetectionState)) {
        case UBLOX::Packet::Nav::Status::Data::SpoofingDetectionState::UnknownOrDeactivated:
            stream << "UnknownOrDeactivated";
            break;
        case UBLOX::Packet::Nav::Status::Data::SpoofingDetectionState::NoSpoofing:
            stream << "NoSpoofing";
            break;
        case UBLOX::Packet::Nav::Status::Data::SpoofingDetectionState::Spoofing:
            stream << "Spoofing";
            break;
        case UBLOX::Packet::Nav::Status::Data::SpoofingDetectionState::MultipleSpoofing:
            stream << "MultipleSpoofing";
            break;
    }
    stream << ", carrier phase range solution: ";
    switch (static_cast<UBLOX::Packet::Nav::Status::Data::CarrierPhaseRangeSolutionStatus>(
            flag.carrierPhaseRangeSolutionStatus)) {
        case UBLOX::Packet::Nav::Status::Data::CarrierPhaseRangeSolutionStatus::NoSolution:
            stream << "NoSolution";
            break;
        case UBLOX::Packet::Nav::Status::Data::CarrierPhaseRangeSolutionStatus::WithFloatingAmbiguities:
            stream << "WithFloatingAmbiguities";
            break;
        case UBLOX::Packet::Nav::Status::Data::CarrierPhaseRangeSolutionStatus::WithFixedAmbiguities:
            stream << "WithFixedAmbiguities";
            break;
    }
    stream << " }";
    return stream;
}
