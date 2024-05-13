#ifndef CONFIGURATION_PACKET_H
#define CONFIGURATION_PACKET_H

#include <ublox/packet/Base.hpp>

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
        MessageRate(const Message message, const uint8_t rate)
            : Base(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rate}) {}

        //FIXME(port-rates): verify which index belongs to which port
        MessageRate(const Message message, const std::array<uint8_t, 6> rates)
            : Base(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rates[0], rates[1], rates[2],
                                             rates[3], rates[4], rates[5]}) {}
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
}// namespace UBLOX::Packet::Cfg
//[[deprecated]];// namespace UBLOX::Packet::Cfg

#endif//CONFIGURATION_PACKET_H
