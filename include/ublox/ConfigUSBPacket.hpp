#ifndef CONFIGUSBPACKET_H
#define CONFIGUSBPACKET_H

#include <ublox/ConfigPortPacket.hpp>

namespace UBLOX {
    class ConfigUSBPacket : public ConfigPortPacket {
    public:
        ConfigUSBPacket(const InProtocol inProtocol, const OutProtocol outProtocol)
            : ConfigPortPacket(PortID::Usb, PortMode{}, 0, inProtocol, outProtocol, false) {}
    };
}// namespace UBLOX

#endif//CONFIGUSBPACKET_H
