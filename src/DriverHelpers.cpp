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
            stream << "UNSUPPORTED FIX";
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
    stream << " differential corrections " << (status.differentialCorrectionsAvailable ? "AVAILABLE" : "UNAVAILABLE")
           << ", carrier phase range solution " << (status.carrierPhaseRangeSolutionValid ? "VALID" : "INVALID")
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
        default:
            stream << "UNSUPPORTED STATUS";
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
        default:
            stream << "UNSUPPORTED FLAG";
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
        default:
            stream << "UNSUPPORTED STATE";
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
        default:
            stream << "UNSUPPORTED STATUS";
    }
    stream << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite &satellite) {
    stream << "{";
    stream << " gnss: ";
    switch (satellite.gnssID) {
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::Gps:
            stream << "GPS";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::Sbas:
            stream << "SBAS";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::Galileo:
            stream << "Galileo";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::BeiDou:
            stream << "BeiDou";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::Qzss:
            stream << "QZSS";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::Glonass:
            stream << "GLONASS";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::GNSSIdentifier::NavIc:
            stream << "NavIC";
            break;
        default:
            stream << "UNSUPPORTED SATELLITE";
    }
    stream << ", satellite id: " << static_cast<int>(satellite.satelliteID)
           << ", signal strength (cno): " << static_cast<int>(satellite.signalStrength)
           << " dBHz, elevation: " << static_cast<int>(satellite.elevationDeg)
           << " deg, azimuth: " << satellite.azimuthDeg
           << " deg, pseudorange residual: " << satellite.pseudorangeResidualDm << " dm(?), signal quality: ";
    //TODO: only one or multiple?
    switch (static_cast<UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality>(
            satellite.flags.signalQuality)) {
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::NoSignal:
            stream << "NoSignal";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::Searching:
            stream << "Searching";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::Acquired:
            stream << "Acquired";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::DetectedUnusable:
            stream << "DetectedUnusable";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::CodeLockedTimeSynchronized:
            stream << "CodeLockedTimeSynchronized";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::CodeAndCarrier1LockedTimeSynchronized:
            stream << "CodeAndCarrier1LockedTimeSynchronized";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::CodeAndCarrier2LockedTimeSynchronized:
            stream << "CodeAndCarrier2LockedTimeSynchronized";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::SignalQuality::CodeAndCarrier3LockedTimeSynchronized:
            stream << "CodeAndCarrier3LockedTimeSynchronized";
            break;
        default:
            stream << "UNSUPPORTED QUALITY";
    }
    stream << ", used for nav: " << (satellite.flags.usedForNavigation ? "YES" : "NO") << ", health: ";
    switch (static_cast<UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::Health>(satellite.flags.health)) {
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::Health::Unknown:
            stream << "Unknown";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::Health::Healthy:
            stream << "Healthy";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::Health::Unhealthy:
            stream << "Unhealthy";
            break;
        default:
            stream << "UNSUPPORTED HEALTH";
    }
    stream << ", differential corrections "
           << (satellite.flags.differentialCorrectionsAvailable ? "AVAILABLE" : "UNAVAILABLE")
           << ", carrier smoothed pseudorange " << (satellite.flags.carrierSmoothedPseudorangeUsed ? "USED" : "UNUSED")
           << ", orbit source: ";
    //TODO: only one or multiple?
    switch (static_cast<UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource>(satellite.flags.orbitSource)) {
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Unavailable:
            stream << "Unavailable";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Ephemeris:
            stream << "Ephemeris";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Almanac:
            stream << "Almanac";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::AssistNowOffline:
            stream << "AssistNowOffline";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::AssistNowAutonomous:
            stream << "AssistNowAutonomous";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Other1:
            stream << "Other1";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Other2:
            stream << "Other2";
            break;
        case UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite::OrbitSource::Other3:
            stream << "Other3";
            break;
        default:
            stream << "UNSUPPORTED ORBIT";
    }
    stream << ", ephemeris " << (satellite.flags.ephemerisAvailable ? "AVAILABLE" : "UNAVAILABLE") << ", almanac "
           << (satellite.flags.almanacAvailable ? "AVAILABLE" : "UNAVAILABLE") << ", AssistNow offline "
           << (satellite.flags.assistNowOfflineDataAvailable ? "AVAILABLE" : "UNAVAILABLE") << ", AssistNow auto "
           << (satellite.flags.assistNowAutonomousDataAvailable ? "AVAILABLE" : "UNAVAILABLE") << ", sbas "
           << (satellite.flags.sbasCorrectionsUsed ? "USED" : "UNUSED") << ", rtcm "
           << (satellite.flags.rtcmCorrectionsUsed ? "USED" : "UNUSED") << ", qzss "
           << (satellite.flags.qzssSlasCorrectionsUsed ? "USED" : "UNUSED") << ", spartn "
           << (satellite.flags.spartnCorrectionsUsed ? "USED" : "UNUSED") << ", pseudorange "
           << (satellite.flags.pseudorangeCorrectionsUsed ? "USED" : "UNUSED") << ", carrier range "
           << (satellite.flags.carrierRangeCorrectionsUsed ? "USED" : "UNUSED") << ", doppler "
           << (satellite.flags.dopplerCorrectionsUsed ? "USED" : "UNUSED") << ", clas "
           << (satellite.flags.clasCorrectionsUsed ? "USED" : "UNUSED");
    stream << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Mon::GNSSInformation::Data::Flag &flag) {
    stream << "{ gps: " << (flag.gps ? "YES" : "NO") << ", glonass: " << (flag.glonass ? "YES" : "NO")
           << ", beidou: " << (flag.beidou ? "YES" : "NO") << ", galileo: " << (flag.galileo ? "YES" : "NO") << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Rxm::StatusFlag &flag) {
    switch (flag) {
        case UBLOX::Packet::Rxm::StatusFlag::Unknown:
            stream << "Unknown";
            break;
        case UBLOX::Packet::Rxm::StatusFlag::Negative:
            stream << "NO";
            break;
        case UBLOX::Packet::Rxm::StatusFlag::Positive:
            stream << "YES";
            break;
        default:
            stream << "UNSUPPORTED FLAG";
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream,
                         const UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Status &status) {
    stream << "{ protocol: ";
    switch (static_cast<UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol>(status.protocol)) {
        case UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol::Unknown:
            stream << "Unknown";
            break;
        case UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol::Rtcm3:
            stream << "Rtcm3";
            break;
        case UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol::Spartn:
            stream << "Spartn";
            break;
        case UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol::PointToMultipoint:
            stream << "PointToMultipoint";
            break;
        case UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Protocol::QzssL6:
            stream << "QzssL6";
            break;
        default:
            stream << "UNSUPPORTED PROTOCOL";
    }
    stream << ", error: " << static_cast<UBLOX::Packet::Rxm::StatusFlag>(status.error)
           << ", used: " << static_cast<UBLOX::Packet::Rxm::StatusFlag>(status.used)
           << ", correction id: " << status.correctionId << ", type " << (status.typeValid ? "VALID" : "INVALID")
           << ", sub type " << (status.subTypeValid ? "VALID" : "INVALID")
           << ", input handling support: " << (status.inputHandlingSupported ? "YES" : "NO")
           << ", encrypted: " << static_cast<UBLOX::Packet::Rxm::StatusFlag>(status.encrypted)
           << ", decrypted: " << static_cast<UBLOX::Packet::Rxm::StatusFlag>(status.decrypted) << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Rxm::RTCMStatus::Data::Status &status) {
    stream << "{ crc " << (status.crcFailed ? "FAILED" : "OK")
           << ", used: " << static_cast<UBLOX::Packet::Rxm::StatusFlag>(status.used) << " }";
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::RelativePositionNED::Data::Flags &flags) {
    stream << "{ fix " << (flags.gnssFixValid ? "VALID" : "NOT VALID") << ", differential corrections "
           << (flags.differentialCorrectionsApplied ? "APPLIED" : "NOT APPLIED") << ", data and baseline "
           << (flags.dataAndBaselineValid ? "VALID" : "INVALID") << ", carrier phase range solution: ";
    switch (static_cast<UBLOX::Packet::Nav::RelativePositionNED::Data::CarrierPhaseRangeSolutionStatus>(
            flags.carrierPhaseRangeSolutionStatus)) {
        case UBLOX::Packet::Nav::RelativePositionNED::Data::CarrierPhaseRangeSolutionStatus::NoSolution:
            stream << "NoSolution";
            break;
        case UBLOX::Packet::Nav::RelativePositionNED::Data::CarrierPhaseRangeSolutionStatus::WithFloatingAmbiguities:
            stream << "WithFloatingAmbiguities";
            break;
        case UBLOX::Packet::Nav::RelativePositionNED::Data::CarrierPhaseRangeSolutionStatus::WithFixedAmbiguities:
            stream << "WithFixedAmbiguities";
            break;
        default:
            stream << "UNKNOWN STATUS";
    }
    stream << ", is moving base: " << (flags.isMovingBase ? "YES" : "NO")
           << ", ref. pos. used for moving base: " << (flags.referencePositionUsedForMovingBase ? "YES" : "NO")
           << ", ref. observ. used for moving base: " << (flags.referenceObservationsUsedForMovingBase ? "YES" : "NO")
           << ", vector heading " << (flags.vectorHeadingValid ? "VALID" : "INVALID")
           << ", all vector components normalized: " << (flags.vectorComponentsNormalized ? "YES" : "NO") << " }";
    return stream;
}
