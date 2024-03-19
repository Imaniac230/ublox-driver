#ifndef CONFIGUSBPACKET_H
#define CONFIGUSBPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigUSB : public ConfigPort {
    public:
        ConfigUSB(const InProtocol inProtocol, const OutProtocol outProtocol)
            : ConfigPort(PortID::Usb, PortMode{}, 0, inProtocol, outProtocol, false) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGUSBPACKET_H
