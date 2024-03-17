#ifndef CONFIGI2CPACKET_H
#define CONFIGI2CPACKET_H

#include <ublox/ConfigPortPacket.hpp>

namespace UBLOX {
    class ConfigI2CPacket : public ConfigPortPacket {
    public:
        ConfigI2CPacket(const PortModeI2C mode, const InProtocol inProtocol, const OutProtocol outProtocol,
                        const bool extendedTxTimeout)
            : ConfigPortPacket(PortID::I2c, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };
}// namespace UBLOX

#endif//CONFIGI2CPACKET_H
