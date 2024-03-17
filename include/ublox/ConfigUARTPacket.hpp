#ifndef CONFIGUARTPACKET_H
#define CONFIGUARTPACKET_H

#include <ublox/ConfigPortPacket.hpp>

namespace UBLOX {
    class ConfigUARTPacket : public ConfigPortPacket {
    public:
        ConfigUARTPacket(const UART portId, const PortModeUART mode, const uint32_t baudRateBitsps,
                         const InProtocol inProtocol, const OutProtocol outProtocol, const bool extendedTxTimeout)
            : ConfigPortPacket(static_cast<PortID>(portId), mode, baudRateBitsps, inProtocol, outProtocol,
                               extendedTxTimeout) {}
    };
}// namespace UBLOX

#endif//CONFIGUARTPACKET_H
