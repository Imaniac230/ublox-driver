#ifndef CONFIGSPIPACKET_H
#define CONFIGSPIPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigSPI : public ConfigPort {
    public:
        ConfigSPI(const PortModeSPI mode, const InProtocol inProtocol, const OutProtocol outProtocol,
                  const bool extendedTxTimeout)
            : ConfigPort(PortID::Spi, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGSPIPACKET_H
