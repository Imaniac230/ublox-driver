#ifndef DEVICE_H
#define DEVICE_H

#include <iterator>
#include <ostream>
#include <string>

#include <ublox/Registers.hpp>
#include <ublox/packet/Packet.hpp>

#include <util/RingBuffer.hpp>
#include <util/Serde.hpp>
#include <util/Serial.hpp>

namespace UBLOX {
    class Device {
    public:
        explicit Device(const std::string &path, const Serial::BaudRate rate) : serial(Serial(path, rate, 5)) {}
        virtual ~Device() = default;

        [[nodiscard]] bool sendPacket(const Packet::Base &packet) const;
        [[nodiscard]] std::list<Packet::Base> receivePackets();

    private:
        Serial serial;
        //TODO(data-parsing): set size according to packet specifications (max expected size, etc.)
        RingBuffer<uint8_t, std::array<uint8_t, 10000>> buffer{};
    };
}// namespace UBLOX

#endif//DEVICE_H
