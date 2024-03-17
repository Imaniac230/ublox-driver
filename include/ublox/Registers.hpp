#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>

namespace UBLOX {
    enum class SyncChar : uint8_t { FirstByte = 0xB5, SecondByte = 0x62 };

    enum class MessageClass : uint8_t {
        Nav = 0x01,
        Rxm = 0x02,
        Inf = 0x04,
        Ack = 0x05,
        Cfg = 0x06,
        Upd = 0x09,
        Mon = 0x0A,
        Aid = 0x0B,
        Tim = 0x0D,
        Esf = 0x10,
        Mga = 0x13,
        Log = 0x21,
        Sec = 0x27,
        Hnr = 0x28,
    };

    inline static constexpr uint16_t operator+(MessageClass left, uint8_t right) {
        return (static_cast<uint16_t>(left) << 8) + right;
    }

    enum class Message : uint16_t {
        // Ack/Nack Messages
        AckAcknowledged = MessageClass::Ack + 0x01,
        AckNotAcknowledged = MessageClass::Ack + 0x00,

        // AssistNow Aiding Messages
        AidGpsAidingAlmanacData = MessageClass::Aid + 0x30,
        AidAssistNowAutonomousData = MessageClass::Aid + 0x33,
        AidGpsAidingEphemerisData = MessageClass::Aid + 0x31,
        AidGpsHealthUtcIonosphere = MessageClass::Aid + 0x02,
        AidGpsAidingData = MessageClass::Aid + 0x01,

        // Configuration Input Messages
        CfgAntenna = MessageClass::Cfg + 0x13,
        CfgDataBatching = MessageClass::Cfg + 0x93,
        CfgClearSaveLoad = MessageClass::Cfg + 0x09,
        CfgDatum = MessageClass::Cfg + 0x06,
        CfgDGnss = MessageClass::Cfg + 0x70,
        CfgOscillator = MessageClass::Cfg + 0x61,
        CfgImuMisalignment = MessageClass::Cfg + 0x56,
        CfgAccelerometer = MessageClass::Cfg + 0x4C,
        CfgGyroscope = MessageClass::Cfg + 0x4D,
        CfgWheelTick = MessageClass::Cfg + 0x82,
        CfgExternalSynchronization = MessageClass::Cfg + 0x60,
        CfgGeofence = MessageClass::Cfg + 0x69,
        CfgGnss = MessageClass::Cfg + 0x3E,
        CfgHighNavigationRate = MessageClass::Cfg + 0x5C,
        CfgInformationMessage = MessageClass::Cfg + 0x02,
        CfgJammingInterfaceMonitor = MessageClass::Cfg + 0x39,
        CfgDataLogger = MessageClass::Cfg + 0x47,
        CfgMessageRate = MessageClass::Cfg + 0x01,
        CfgNavigationEngine = MessageClass::Cfg + 0x24,
        CfgNavigationEngineExpert = MessageClass::Cfg + 0x23,
        CfgNmea = MessageClass::Cfg + 0x17,
        CfgOdometer = MessageClass::Cfg + 0x1E,
        CfgExtendedPowerManagement = MessageClass::Cfg + 0x3B,
        CfgPowerMode = MessageClass::Cfg + 0x86,
        CfgPort = MessageClass::Cfg + 0x00,
        CfgReceiverPower = MessageClass::Cfg + 0x57,
        CfgNavigationMeasurementRate = MessageClass::Cfg + 0x08,
        CfgRemoteInventory = MessageClass::Cfg + 0x34,
        CfgResetReceiver = MessageClass::Cfg + 0x04,
        CfgRxm = MessageClass::Cfg + 0x11,
        CfgSbas = MessageClass::Cfg + 0x16,
        CfgI2cSensorInterface = MessageClass::Cfg + 0x88,
        CfgSlas = MessageClass::Cfg + 0x8D,
        CfgSynchronizationManager = MessageClass::Cfg + 0x62,
        CfgConfigureAndRestart = MessageClass::Cfg + 0x64,
        CfgTimeMode2 = MessageClass::Cfg + 0x3D,
        CfgTimeMode3 = MessageClass::Cfg + 0x71,
        CfgTimePulse = MessageClass::Cfg + 0x31,
        CfgTxBufferTimeSlots = MessageClass::Cfg + 0x53,
        CfgUsb = MessageClass::Cfg + 0x1B,

        // External Sensor Fusion Messages
        EsfImuAlignment = MessageClass::Esf + 0x14,
        EsfVehicleDynamics = MessageClass::Esf + 0x15,
        EsfExternalSensorFusionMeasurements = MessageClass::Esf + 0x02,
        EsfRawSensorMeasurements = MessageClass::Esf + 0x03,
        EsfExternalSensorFusionStatus = MessageClass::Esf + 0x10,

        // High Rate Navigation Results Messages
        HnrAttitudeSolution = MessageClass::Hnr + 0x01,
        HnrVehicleDynamics = MessageClass::Hnr + 0x02,
        HnrHighRatePvtOutput = MessageClass::Hnr + 0x00,

        // Information MessagesS
        InfDebug = MessageClass::Inf + 0x04,
        InfError = MessageClass::Inf + 0x00,
        InfNotice = MessageClass::Inf + 0x02,
        InfTest = MessageClass::Inf + 0x03,
        InfWarning = MessageClass::Inf + 0x01,

        // Logging Messages
        LogBatchedLogData = MessageClass::Log + 0x11,
        LogCreateFile = MessageClass::Log + 0x07,
        LogEraseData = MessageClass::Log + 0x03,
        LogEntryIndex = MessageClass::Log + 0x0E,
        LogInformation = MessageClass::Log + 0x08,
        LogRequestBatchData = MessageClass::Log + 0x10,
        LogOdometer = MessageClass::Log + 0x0F,
        LogPositionFix = MessageClass::Log + 0x0B,
        LogByteString = MessageClass::Log + 0x0D,
        LogRequestData = MessageClass::Log + 0x09,
        LogStoreString = MessageClass::Log + 0x04,

        // Multiple GNSS Assistance Messages

        // Monitoring Messages
        MonDataBatchingBufferStatus = MessageClass::Mon + 0x32,
        MonGnssInformationMessage = MessageClass::Mon + 0x28,
        MonExtendedHardwareStatus = MessageClass::Mon + 0x0B,
        MonHardwareStatus = MessageClass::Mon + 0x09,
        MonIoSystemStatus = MessageClass::Mon + 0x02,
        MonMessageParseAndProcessStatus = MessageClass::Mon + 0x06,
        MonInstalledPatches = MessageClass::Mon + 0x27,
        MonReceiverBufferStatus = MessageClass::Mon + 0x07,
        MonReceiverStatus = MessageClass::Mon + 0x21,
        MonSynchronizationManagerStatus = MessageClass::Mon + 0x2E,
        MonSensorProductTest = MessageClass::Mon + 0x2F,
        MonTransmitterBufferStatus = MessageClass::Mon + 0x08,
        MonReceiverAndSoftwareVersion = MessageClass::Mon + 0x04,

        // Navigation Results Messages
        NavAssistNowAutonomousStatus = MessageClass::Nav + 0x60,
        NavAttitudeSolution = MessageClass::Nav + 0x05,
        NavClockSolution = MessageClass::Nav + 0x22,
        NavCovarianceMatrices = MessageClass::Nav + 0x36,
        NavDgpsNavData = MessageClass::Nav + 0x31,
        NavDilutionOfPrecision = MessageClass::Nav + 0x04,
        NavPositionErrorEllipseParameters = MessageClass::Nav + 0x3D,
        NavEndOfEpoch = MessageClass::Nav + 0x61,
        NavGeofencingStatus = MessageClass::Nav + 0x39,
        NavHighPrecisionEcefPositionSolution = MessageClass::Nav + 0x13,
        NavHighPrecisionGeodeticPositionSolution = MessageClass::Nav + 0x14,
        NavNavigationMessageCrosscheck = MessageClass::Nav + 0x28,
        NavOdometerSolution = MessageClass::Nav + 0x09,
        NavGnssOrbitDatabaseInfo = MessageClass::Nav + 0x34,
        NavEcefPositionSolution = MessageClass::Nav + 0x01,
        NavGeodeticPositionSolution = MessageClass::Nav + 0x02,
        NavPositionVelocityTimeSolution = MessageClass::Nav + 0x07,
        NavRelativePositioningInformation = MessageClass::Nav + 0x3C,
        NavResetOdometer = MessageClass::Nav + 0x10,
        NavSatelliteInformation = MessageClass::Nav + 0x35,
        NavSbasStatusData = MessageClass::Nav + 0x32,
        NavQzssL1sSlasStatusData = MessageClass::Nav + 0x42,
        NavNavigationSolutionInformation = MessageClass::Nav + 0x06,
        NavReceiverNavigationStatus = MessageClass::Nav + 0x03,
        NavSpaceVehicleInformation = MessageClass::Nav + 0x30,
        NavSurveyInData = MessageClass::Nav + 0x3B,
        NavBeidouTimeSolution = MessageClass::Nav + 0x24,
        NavGalileoTimeSolution = MessageClass::Nav + 0x25,
        NavGlonassTimeSolution = MessageClass::Nav + 0x23,
        NavGpsTimeSolution = MessageClass::Nav + 0x20,
        NavLeapSecondEventInformation = MessageClass::Nav + 0x26,
        NavUtcTimeSolution = MessageClass::Nav + 0x21,
        NavEcefVelocitySolution = MessageClass::Nav + 0x11,
        NavNedVelocitySolution = MessageClass::Nav + 0x12,

        // Receiver Manager Messages
        RxmIndoorMessagingSystem = MessageClass::Rxm + 0x61,
        RxmRrlpSatelliteMeasurements = MessageClass::Rxm + 0x14,
        RxmPowerManagementRequest = MessageClass::Rxm + 0x41,
        RxmMultiGnssRawMeasurements = MessageClass::Rxm + 0x15,
        RxmGalileoSarRlmReport = MessageClass::Rxm + 0x59,
        RxmRtcmInputStatus = MessageClass::Rxm + 0x32,
        RxmBroadcastNavigationDataSubframe = MessageClass::Rxm + 0x13,
        RxmSvStatusInfo = MessageClass::Rxm + 0x20,

        // Security Feature Messages
        SecUniqueChipId = MessageClass::Sec + 0x03,

        // Timing Messages

        // Firmware Update Messages

    };

    inline static constexpr uint8_t toRawClass(const Message message) {
        return static_cast<uint8_t>(static_cast<uint16_t>(message) >> 8);
    }
    inline static constexpr MessageClass toClass(const Message message) {
        return static_cast<MessageClass>(static_cast<uint16_t>(message) >> 8);
    }
    inline static constexpr uint8_t toRawId(const Message message) { return static_cast<uint8_t>(message); }
    inline static constexpr Message toMessage(const MessageClass messageClass, const uint8_t messageId) {
        return static_cast<Message>(messageClass + messageId);
    }
    inline static constexpr Message toMessage(const uint8_t messageClass, const uint8_t messageId) {
        return toMessage(static_cast<MessageClass>(messageClass), messageId);
    }
}// namespace UBLOX

#endif//REGISTERS_H
