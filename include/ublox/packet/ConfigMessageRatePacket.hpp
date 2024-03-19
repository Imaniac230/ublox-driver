#ifndef CONFIGMESSAGERATEPACKET_H
#define CONFIGMESSAGERATEPACKET_H

#include <ublox/packet/ConfigPortPacket.hpp>

namespace UBLOX::Packet {
    class ConfigMessageRate : public Base {
    public:
        ConfigMessageRate(const Message message, const uint8_t rate)
            : Base(Message::CfgMessageRate, {toRawClass(message), toRawId(message), rate}) {}
    };
}// namespace UBLOX::Packet

#endif//CONFIGMESSAGERATEPACKET_H
