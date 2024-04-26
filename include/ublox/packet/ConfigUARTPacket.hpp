#ifndef CONFIGUARTPACKET_H
#define CONFIGUARTPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigUART : public ConfigPort {
    public:
        ConfigUART(const UART portId, const CharLength charLength, Parity parity, StopBits stopBits,
                   const uint32_t baudRateBitsps, const InProtocol inProtocol, const OutProtocol outProtocol,
                   const bool extendedTxTimeout)
            : ConfigPort(static_cast<PortID>(portId),
                         PortModeUART(PortModeUARTRaw{.charLen = static_cast<uint32_t>(charLength),
                                                      .parity = static_cast<uint32_t>(parity),
                                                      .stopBits = static_cast<uint32_t>(stopBits)}),
                         baudRateBitsps, inProtocol, outProtocol, extendedTxTimeout) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGUARTPACKET_H
