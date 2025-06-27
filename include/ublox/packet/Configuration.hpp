#ifndef CONFIGURATION_PACKET_H
#define CONFIGURATION_PACKET_H

#include <ublox/packet/Base.hpp>

//FIXME(deprecated): this whole configuration structure is deprecated in newer versions,
//  transfer to the new message definitions
namespace UBLOX::Packet::Cfg {
    class Port : public Base {
    public:
        // Common for all ports
        //FIXME(uart-id): set these correctly according to the ublox documentation
        enum class UARTID : uint8_t {
            Uart1 = 0x01,
            Uart2 = 0x02,
        };
        enum class ID : uint8_t {
            I2c = 0x00,
            Uart1 = static_cast<uint8_t>(UARTID::Uart1),
            Uart2 = static_cast<uint8_t>(UARTID::Uart2),
            Usb = 0x03,
            Spi = 0x04,
        };
        enum class InProtocol : uint16_t { None = 0x00, Ubx = 0x01, Nmea = 0x02, Rtcm = 0x04, Rtcm3 = 0x20 };
        enum class OutProtocol : uint16_t { None = 0x00, Ubx = 0x01, Nmea = 0x02, Rtcm3 = 0x20 };

        struct Mode {
            uint32_t raw;
        };

        // Valid for UART port
        enum class CharLength : uint8_t { Bits5 = 0x00, Bits6 = 0x01, Bits7 = 0x02, Bits8 = 0x03 };
        enum class Parity : uint8_t { Even = 0x00, Odd = 0x01, None = 0x04 };
        enum class StopBits : uint8_t { One = 0x00, OneAndHalve = 0x01, Two = 0x02, Halve = 0x03 };
        struct __attribute__((__packed__)) __attribute__((aligned(1))) ModeUARTRaw {
            uint32_t reserved1 : 6 = 0;
            uint32_t charLen : 2 = static_cast<uint32_t>(CharLength::Bits8);
            uint32_t reserved2 : 1 = 0;
            uint32_t parity : 3 = static_cast<uint32_t>(Parity::None);
            uint32_t stopBits : 2 = static_cast<uint32_t>(StopBits::One);
            uint32_t reserved3 = 0;
        };
        struct ModeUART : Mode {
            explicit ModeUART(const ModeUARTRaw raw) : Mode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        // Valid for SPI port
        enum class SPIMode : uint8_t { Mode0 = 0x00, Mode1 = 0x01, Mode2 = 0x02, Mode3 = 0x03 };
        struct __attribute__((__packed__)) __attribute__((aligned(1))) ModeSPIRaw {
            uint32_t reserved1 : 1 = 0;
            uint32_t spiMode : 2 = static_cast<uint32_t>(SPIMode::Mode0);
            uint32_t reserved2 : 5 = 0;
            uint32_t numberOfFFBytes : 6 = 0;
            uint32_t reserved3 = 0;
        };
        struct ModeSPI : Mode {
            explicit ModeSPI(const ModeSPIRaw raw) : Mode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        // Valid for I2C port
        struct __attribute__((__packed__)) __attribute__((aligned(1))) ModeI2CRaw {
            uint32_t reserved1 : 1 = 0;
            uint32_t slaveAddress : 7 = 0x07;// valid from 0x07 to 0x78
            uint32_t reserved2 = 0;
        };
        struct ModeI2C : Mode {
            explicit ModeI2C(const ModeI2CRaw raw) : Mode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        Port(const ID id, const Mode mode, const uint32_t baudRateBitsps, const InProtocol inProtocol,
             const OutProtocol outProtocol, const bool extendedTxTimeout)
            : Base(Message::CfgPort, {static_cast<uint8_t>(id),
                                      0x00,//reserved 1 byte
                                      0x00,//TODO: TX ready pin config
                                      0x00,//TODO: TX ready pin config
                                      Serde::serializeLEInt(mode.raw)[0],
                                      Serde::serializeLEInt(mode.raw)[1],
                                      Serde::serializeLEInt(mode.raw)[2],
                                      Serde::serializeLEInt(mode.raw)[3],
                                      Serde::serializeLEInt(baudRateBitsps)[0],
                                      Serde::serializeLEInt(baudRateBitsps)[1],
                                      Serde::serializeLEInt(baudRateBitsps)[2],
                                      Serde::serializeLEInt(baudRateBitsps)[3],
                                      Serde::serializeLEInt(static_cast<uint16_t>(inProtocol))[0],
                                      Serde::serializeLEInt(static_cast<uint16_t>(inProtocol))[1],
                                      Serde::serializeLEInt(static_cast<uint16_t>(outProtocol))[0],
                                      Serde::serializeLEInt(static_cast<uint16_t>(outProtocol))[1],
                                      Serde::serializeLEInt<uint16_t>(extendedTxTimeout)[0],
                                      Serde::serializeLEInt<uint16_t>(extendedTxTimeout)[1],
                                      0x00,//reserved 2 bytes
                                      0x00}) {}
    };

    inline static constexpr Port::InProtocol operator|(const Port::InProtocol left, const Port::InProtocol right) {
        return static_cast<Port::InProtocol>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }
    inline static constexpr Port::InProtocol operator|(const Port::InProtocol left, const uint16_t right) {
        return static_cast<Port::InProtocol>(static_cast<uint16_t>(left) | right);
    }
    inline static constexpr Port::InProtocol operator|(const uint16_t left, const Port::InProtocol right) {
        return right | left;
    }
    inline static constexpr Port::OutProtocol operator|(const Port::OutProtocol left, const Port::OutProtocol right) {
        return static_cast<Port::OutProtocol>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }
    inline static constexpr Port::OutProtocol operator|(const Port::OutProtocol left, const uint16_t right) {
        return static_cast<Port::OutProtocol>(static_cast<uint16_t>(left) | right);
    }
    inline static constexpr Port::OutProtocol operator|(const uint16_t left, const Port::OutProtocol right) {
        return right | left;
    }

    class I2C : public Port {
    public:
        I2C(const ModeI2C mode, const InProtocol inProtocol, const OutProtocol outProtocol,
            const bool extendedTxTimeout)
            : Port(ID::I2c, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };


    class SPI : public Port {
    public:
        SPI(const ModeSPI mode, const InProtocol inProtocol, const OutProtocol outProtocol,
            const bool extendedTxTimeout)
            : Port(ID::Spi, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };

    class UART : public Port {
    public:
        UART(const UARTID portId, const CharLength charLength, Parity parity, StopBits stopBits,
             const uint32_t baudRateBitsps, const InProtocol inProtocol, const OutProtocol outProtocol,
             const bool extendedTxTimeout)
            : Port(static_cast<ID>(portId),
                   ModeUART(ModeUARTRaw{.charLen = static_cast<uint32_t>(charLength),
                                        .parity = static_cast<uint32_t>(parity),
                                        .stopBits = static_cast<uint32_t>(stopBits)}),
                   baudRateBitsps, inProtocol, outProtocol, extendedTxTimeout) {}
    };

    class USB : public Port {
    public:
        USB(const InProtocol inProtocol, const OutProtocol outProtocol)
            : Port(ID::Usb, Mode{}, 0, inProtocol, outProtocol, false) {}
    };

    class MessageRate : public Base {
    public:
        //FIXME(port-rates): verify which index belongs to which port
        struct Rates {
            uint8_t i2c = 0;
            uint8_t uart1 = 0;
            uint8_t uart2 = 0;
            uint8_t usb = 0;
            uint8_t spi = 0;
            uint8_t unknown = 0;
        };

        MessageRate(const Message message, const uint8_t rate)
            : Base(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rate}) {}

        MessageRate(const Message message, const Rates rates)
            : Base(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rates.i2c, rates.uart1, rates.uart2,
                                             rates.usb, rates.spi, rates.unknown}) {}
    };

    class DifferentialGNSS : public Base {
    public:
        enum class Mode : uint8_t { RtkFloat = 2, RtkFixed = 3 };

        explicit DifferentialGNSS(const Mode mode)
            : Base(Message::CfgDGnss, {static_cast<uint8_t>(mode), 0x00, 0x00, 0x00}) {}
    };

    class NavigationRate : public Base {
    public:
        enum class TimeReference : uint16_t { Utc = 0, Gps = 1, Glonass = 2, Beidou = 3, Galileo = 4, Navic = 5 };

        NavigationRate(const uint16_t measurementRateMillis, const uint16_t navigationRateCycles,
                       const TimeReference reference)
            : Base(Message::CfgNavigationMeasurementRate,
                   {Serde::serializeLEInt(measurementRateMillis)[0], Serde::serializeLEInt(measurementRateMillis)[1],
                    Serde::serializeLEInt(navigationRateCycles)[0], Serde::serializeLEInt(navigationRateCycles)[1],
                    Serde::serializeLEInt(static_cast<uint16_t>(reference))[0],
                    Serde::serializeLEInt(static_cast<uint16_t>(reference))[1]}) {}
    };

    class TimeMode : public Base {
    public:
        enum class Mode : uint8_t { Disabled = 0, SurveyIn = 1, Fixed = 2 };
        struct PositionECEFOrLLA {
            int32_t XOrLatitude = 0; //cm or degree = * 1e-7
            int32_t YOrLongitude = 0;//cm or degree = * 1e-7
            int32_t ZOrAltitude = 0; //cm
            uint32_t accuracy = 0;   // mm = * 0.1
        };
        struct HighPrecisionPositionECEFOrLLA {
            int8_t XOrLatitude = 0; //mm = * 0.1 or degree = * 1e-9
            int8_t YOrLongitude = 0;//mm = * 0.1 or degree = * 1e-9
            int8_t ZOrAltitude = 0; //mm = * 0.1
        };

        TimeMode()
            : Base(Message::CfgTimeMode3, {0x00,
                                           0x00,//reserved
                                           static_cast<uint8_t>(Mode::Disabled),
                                           0x00,//given position mode, only first bit: 0 -> ECEF, 1 -> LLA
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[3],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[3],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[3],
                                           HighPrecisionPositionECEFOrLLA{}.XOrLatitude,
                                           HighPrecisionPositionECEFOrLLA{}.YOrLongitude,
                                           HighPrecisionPositionECEFOrLLA{}.ZOrAltitude,
                                           0x00,//reserved
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[3],
                                           0x00,//survey-in minimum duration
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,//survey-in position accuracy limit
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,//8 bytes reserved
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00}) {}

        explicit TimeMode(const PositionECEFOrLLA position, const bool positionInLla = false,
                          const std::optional<HighPrecisionPositionECEFOrLLA> highPrecisionPosition = std::nullopt)
            : Base(Message::CfgTimeMode3,
                   {0x00,
                    0x00,//reserved
                    static_cast<uint8_t>(Mode::Fixed),
                    static_cast<uint8_t>(positionInLla),//given position mode, only first bit: 0 -> ECEF, 1 -> LLA
                    Serde::serializeLEInt(position.XOrLatitude)[0],
                    Serde::serializeLEInt(position.XOrLatitude)[1],
                    Serde::serializeLEInt(position.XOrLatitude)[2],
                    Serde::serializeLEInt(position.XOrLatitude)[3],
                    Serde::serializeLEInt(position.YOrLongitude)[0],
                    Serde::serializeLEInt(position.YOrLongitude)[1],
                    Serde::serializeLEInt(position.YOrLongitude)[2],
                    Serde::serializeLEInt(position.YOrLongitude)[3],
                    Serde::serializeLEInt(position.ZOrAltitude)[0],
                    Serde::serializeLEInt(position.ZOrAltitude)[1],
                    Serde::serializeLEInt(position.ZOrAltitude)[2],
                    Serde::serializeLEInt(position.ZOrAltitude)[3],
                    static_cast<uint8_t>(highPrecisionPosition.value_or(HighPrecisionPositionECEFOrLLA{}).XOrLatitude),
                    static_cast<uint8_t>(highPrecisionPosition.value_or(HighPrecisionPositionECEFOrLLA{}).YOrLongitude),
                    static_cast<uint8_t>(highPrecisionPosition.value_or(HighPrecisionPositionECEFOrLLA{}).ZOrAltitude),
                    0x00,//reserved
                    Serde::serializeLEInt(position.accuracy)[0],
                    Serde::serializeLEInt(position.accuracy)[1],
                    Serde::serializeLEInt(position.accuracy)[2],
                    Serde::serializeLEInt(position.accuracy)[3],
                    0x00,//survey-in minimum duration
                    0x00,
                    0x00,
                    0x00,
                    0x00,//survey-in position accuracy limit
                    0x00,
                    0x00,
                    0x00,
                    0x00,//8 bytes reserved
                    0x00,
                    0x00,
                    0x00,
                    0x00,
                    0x00,
                    0x00,
                    0x00}) {}

        TimeMode(const uint32_t minimumDurationSecs, const uint32_t positionAccuracyLimit /*mm = * 0.1*/)
            : Base(Message::CfgTimeMode3, {0x00,
                                           0x00,//reserved
                                           static_cast<uint8_t>(Mode::SurveyIn),
                                           0x00,//given position mode, only first bit: 0 -> ECEF, 1 -> LLA
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.XOrLatitude)[3],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.YOrLongitude)[3],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.ZOrAltitude)[3],
                                           HighPrecisionPositionECEFOrLLA{}.XOrLatitude,
                                           HighPrecisionPositionECEFOrLLA{}.YOrLongitude,
                                           HighPrecisionPositionECEFOrLLA{}.ZOrAltitude,
                                           0x00,//reserved
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[0],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[1],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[2],
                                           Serde::serializeLEInt(PositionECEFOrLLA{}.accuracy)[3],
                                           Serde::serializeLEInt(minimumDurationSecs)[0],
                                           Serde::serializeLEInt(minimumDurationSecs)[1],
                                           Serde::serializeLEInt(minimumDurationSecs)[2],
                                           Serde::serializeLEInt(minimumDurationSecs)[3],
                                           Serde::serializeLEInt(positionAccuracyLimit)[0],
                                           Serde::serializeLEInt(positionAccuracyLimit)[1],
                                           Serde::serializeLEInt(positionAccuracyLimit)[2],
                                           Serde::serializeLEInt(positionAccuracyLimit)[3],
                                           0x00,//8 bytes reserved
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00,
                                           0x00}) {}
    };

    class ResetAndClearBackup : public Base {
    public:
        enum class ClearModePreset : uint16_t { HotStart = 0x0000, WarmStart = 0x0001, ColdStart = 0xFFFF };
        enum class ResetMode : uint8_t {
            HardwareImmediate = 0x00,
            ControlledSoftware = 0x01,
            GnssOnly = 0x02,
            HardwareAfterShutdown = 0x04,
            ControlledGnssStop = 0x08,
            ControlledGnssStart = 0x09
        };
        struct __attribute__((__packed__)) __attribute__((aligned(1))) ClearMode {
            uint16_t ephemeris : 1 = 0;
            uint16_t almanac : 1 = 0;
            uint16_t health : 1 = 0;
            uint16_t klobucharParameters : 1 = 0;
            uint16_t position : 1 = 0;
            uint16_t clockDrift : 1 = 0;
            uint16_t oscillator : 1 = 0;
            uint16_t utc : 1 = 0;
            uint16_t rtc : 1 = 0;
            uint16_t sfdrAndWeakSignalCompensationEstimates : 1 = 0;
            uint16_t sfdrVehicleMonitoring : 1 = 0;
            uint16_t tct : 1 = 0;
            uint16_t autonomousOrbit : 1 = 0;
            uint16_t reserved = 0;
        };

        explicit ResetAndClearBackup(const ClearMode clearMode,
                                     const ResetMode resetMode = ResetMode::HardwareImmediate)
            : Base(Message::CfgResetReceiver,
                   {Serde::serializeLEInt(*reinterpret_cast<const uint16_t *>(&clearMode))[0],
                    Serde::serializeLEInt(*reinterpret_cast<const uint16_t *>(&clearMode))[1],
                    static_cast<uint8_t>(resetMode), 0x00 /*1 byte reserved*/}) {}

        explicit ResetAndClearBackup(const ClearModePreset clearMode = ClearModePreset::ColdStart,
                                     const ResetMode resetMode = ResetMode::HardwareImmediate)
            : ResetAndClearBackup(*reinterpret_cast<const ClearMode *>(&clearMode), resetMode) {}
    };

    class ClearSaveLoad : public Base {
    public:
        enum class Mode : uint8_t { Clear, Save, Load };

        //NOTE: ignoring memory device optional group - will always operate on BBR and Flash
        //NOTE: any set bit in each uint32_t value should trigger the action, but we're setting all just in case
        //        explicit ClearSaveLoad(const Mode mode)
        //            : Base(Message::CfgClearSaveLoad,
        //                   {static_cast<uint8_t>((mode == Mode::Clear) ? 0x01 : 0x00), 0x00, 0x00, 0x00,
        //                    static_cast<uint8_t>((mode == Mode::Save) ? 0x01 : 0x00), 0x00, 0x00, 0x00,
        //                    static_cast<uint8_t>((mode == Mode::Load) ? 0x01 : 0x00), 0x00, 0x00, 0x00}) {}
        explicit ClearSaveLoad(const Mode mode)
            : Base(Message::CfgClearSaveLoad,
                   {Serde::serializeLEInt<uint32_t>((mode == Mode::Clear) ? 0xFFFFFFFF : 0x00000000)[0],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Clear) ? 0xFFFFFFFF : 0x00000000)[1],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Clear) ? 0xFFFFFFFF : 0x00000000)[2],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Clear) ? 0xFFFFFFFF : 0x00000000)[3],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Save) ? 0xFFFFFFFF : 0x00000000)[0],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Save) ? 0xFFFFFFFF : 0x00000000)[1],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Save) ? 0xFFFFFFFF : 0x00000000)[2],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Save) ? 0xFFFFFFFF : 0x00000000)[3],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Load) ? 0xFFFFFFFF : 0x00000000)[0],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Load) ? 0xFFFFFFFF : 0x00000000)[1],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Load) ? 0xFFFFFFFF : 0x00000000)[2],
                    Serde::serializeLEInt<uint32_t>((mode == Mode::Load) ? 0xFFFFFFFF : 0x00000000)[3]}) {}
    };
}// namespace UBLOX::Packet::Cfg
//[[deprecated]];// namespace UBLOX::Packet::Cfg

#endif//CONFIGURATION_PACKET_H
