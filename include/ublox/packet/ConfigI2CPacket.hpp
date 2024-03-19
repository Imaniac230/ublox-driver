#ifndef CONFIGI2CPACKET_H
#define CONFIGI2CPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigI2C : public ConfigPort {
    public:
        ConfigI2C(const PortModeI2C mode, const InProtocol inProtocol, const OutProtocol outProtocol,
                  const bool extendedTxTimeout)
            : ConfigPort(PortID::I2c, mode, 0, inProtocol, outProtocol, extendedTxTimeout) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGI2CPACKET_H
