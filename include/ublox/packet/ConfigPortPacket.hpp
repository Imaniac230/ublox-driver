#ifndef CONFIGPORTPACKET_H
#define CONFIGPORTPACKET_H

#include <ublox/packet/Packet.hpp>

namespace UBLOX::Packet {
    class ConfigPort : public Base {
    public:
        // Common for all ports
        //FIXME(uart-id): set these correctly according to the ublox documentation
        enum class UART : uint8_t {
            Uart1 = 0x01,
            Uart2 = 0x02,
        };
        enum class PortID : uint8_t {
            I2c = 0x00,
            Uart1 = static_cast<uint8_t>(UART::Uart1),
            Uart2 = static_cast<uint8_t>(UART::Uart2),
            Usb = 0x03,
            Spi = 0x04,
        };
        enum class InProtocol : uint16_t { Ubx = 0x01, Nmea = 0x02, Rtcm = 0x04, Rtcm3 = 0x20 };
        enum class OutProtocol : uint16_t { Ubx = 0x01, Nmea = 0x02, Rtcm3 = 0x20 };

        struct PortMode {
            uint32_t raw;
        };

        // Valid for UART port
        enum class CharLength : uint8_t { Bits5 = 0x00, Bits6 = 0x01, Bits7 = 0x02, Bits8 = 0x03 };
        enum class Parity : uint8_t { Even = 0x00, Odd = 0x01, None = 0x04 };
        enum class StopBits : uint8_t { One = 0x00, OneAndHalve = 0x01, Two = 0x02, Halves = 0x03 };
        struct __attribute__((__packed__)) __attribute__((aligned(1))) PortModeUARTRaw {
            uint32_t reserved1 : 6 = 0;
            uint32_t charLen : 2 = static_cast<uint32_t>(CharLength::Bits8);
            uint32_t reserved2 : 1 = 0;
            uint32_t parity : 3 = static_cast<uint32_t>(Parity::Even);
            uint32_t stopBits : 2 = static_cast<uint32_t>(StopBits::One);
            uint32_t reserved3 = 0;
        };
        struct PortModeUART : PortMode {
            explicit PortModeUART(const PortModeUARTRaw raw) : PortMode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        // Valid for SPI port
        enum class SPIMode : uint8_t { Mode0 = 0x00, Mode1 = 0x01, Mode2 = 0x02, Mode3 = 0x03 };
        struct __attribute__((__packed__)) __attribute__((aligned(1))) PortModeSPIRaw {
            uint32_t reserved1 : 1 = 0;
            uint32_t spiMode : 2 = static_cast<uint32_t>(SPIMode::Mode0);
            uint32_t reserved2 : 5 = 0;
            uint32_t numberOfFFBytes : 6 = 0;
            uint32_t reserved3 = 0;
        };
        struct PortModeSPI : PortMode {
            explicit PortModeSPI(const PortModeSPIRaw raw) : PortMode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        // Valid for I2C port
        struct __attribute__((__packed__)) __attribute__((aligned(1))) PortModeI2CRaw {
            uint32_t reserved1 : 1 = 0;
            uint32_t slaveAddress : 7 = 0x07;// valid from 0x07 to 0x78
            uint32_t reserved2 = 0;
        };
        struct PortModeI2C : PortMode {
            explicit PortModeI2C(const PortModeI2CRaw raw) : PortMode{*reinterpret_cast<const uint32_t *>(&raw)} {};
        };

        ConfigPort(const PortID id, const PortMode mode, const uint32_t baudRateBitsps, const InProtocol inProtocol,
                   const OutProtocol outProtocol, const bool extendedTxTimeout)
            : Base(Message::CfgPort, {static_cast<uint8_t>(id),
                                      0x00,
                                      0x00,//TODO: TX ready pin config
                                      0x00,//TODO: TX ready pin config
                                      Utils::serializeLEInt(mode.raw)[0],
                                      Utils::serializeLEInt(mode.raw)[1],
                                      Utils::serializeLEInt(mode.raw)[2],
                                      Utils::serializeLEInt(mode.raw)[3],
                                      Utils::serializeLEInt(baudRateBitsps)[0],
                                      Utils::serializeLEInt(baudRateBitsps)[1],
                                      Utils::serializeLEInt(baudRateBitsps)[2],
                                      Utils::serializeLEInt(baudRateBitsps)[3],
                                      Utils::serializeLEInt(static_cast<uint16_t>(inProtocol))[0],
                                      Utils::serializeLEInt(static_cast<uint16_t>(inProtocol))[1],
                                      Utils::serializeLEInt(static_cast<uint16_t>(outProtocol))[0],
                                      Utils::serializeLEInt(static_cast<uint16_t>(outProtocol))[1],
                                      Utils::serializeLEInt<uint16_t>(extendedTxTimeout)[0],
                                      Utils::serializeLEInt<uint16_t>(extendedTxTimeout)[1],
                                      0x00,
                                      0x00}) {}
    };

    using OutProtocol = ConfigPort::OutProtocol;
    using InProtocol = ConfigPort::InProtocol;
    inline static constexpr InProtocol operator|(const InProtocol left, const InProtocol right) {
        return static_cast<InProtocol>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }
    inline static constexpr InProtocol operator|(const InProtocol left, const uint16_t right) {
        return static_cast<InProtocol>(static_cast<uint16_t>(left) | right);
    }
    inline static constexpr InProtocol operator|(const uint16_t left, const InProtocol right) { return right | left; }
    inline static constexpr OutProtocol operator|(const OutProtocol left, const OutProtocol right) {
        return static_cast<OutProtocol>(static_cast<uint16_t>(left) | static_cast<uint16_t>(right));
    }
    inline static constexpr OutProtocol operator|(const OutProtocol left, const uint16_t right) {
        return static_cast<OutProtocol>(static_cast<uint16_t>(left) | right);
    }
    inline static constexpr OutProtocol operator|(const uint16_t left, const OutProtocol right) { return right | left; }
}// namespace UBLOX::Packet

#endif//CONFIGPORTPACKET_H
