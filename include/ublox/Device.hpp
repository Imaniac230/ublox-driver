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
        //FIXME(baud-rate): export baud rate to config (default for UART1 is 38400)
        explicit Device(const std::string &path) : serial(Serial(path, Serial::BaudRate::Baud115200, 5)) {}
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
