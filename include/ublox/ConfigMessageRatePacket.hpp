#ifndef CONFIGMESSAGERATEPACKET_H
#define CONFIGMESSAGERATEPACKET_H

#include <ublox/ConfigPortPacket.hpp>

namespace UBLOX {
    class ConfigMessageRatePacket : public Packet {
    public:
        ConfigMessageRatePacket(const Message message, const uint8_t rate)
            : Packet(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rate}) {}
    };
}// namespace UBLOX

#endif//CONFIGMESSAGERATEPACKET_H
