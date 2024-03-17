#ifndef CONFIGSPIPACKET_H
#define CONFIGSPIPACKET_H

#include <ublox/ConfigPortPacket.hpp>

namespace UBLOX {
    class ConfigSPIPacket : public ConfigPortPacket {
    public:
        ConfigSPIPacket(const PortModeSPI mode, const InProtocol inProtocol, const OutProtocol outProtocol,
                        const bool extendedTxTimeout)
            : ConfigPortPacket(PortID::Spi, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };
}// namespace UBLOX

#endif//CONFIGSPIPACKET_H
