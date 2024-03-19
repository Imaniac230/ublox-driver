#ifndef CONFIGUARTPACKET_H
#define CONFIGUARTPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigUART : public ConfigPort {
    public:
        ConfigUART(const UART portId, const PortModeUART mode, const uint32_t baudRateBitsps,
                   const InProtocol inProtocol, const OutProtocol outProtocol, const bool extendedTxTimeout)
            : ConfigPort(static_cast<PortID>(portId), mode, baudRateBitsps, inProtocol, outProtocol,
                         extendedTxTimeout) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGUARTPACKET_H
