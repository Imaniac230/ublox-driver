#ifndef DEVICE_H
#define DEVICE_H

#include <iterator>
#include <ostream>
#include <string>

#include <ublox/Packet.hpp>
#include <ublox/Registers.hpp>

#include <util/Serial.hpp>
#include <util/Utils.hpp>

namespace UBLOX {
    class Device {
    public:
        explicit Device(const std::string &path) : serial(Serial(path, Serial::BaudRate::Baud115200, 5)) {}
        virtual ~Device() = default;

        [[nodiscard]] bool sendPacket(const Packet &packet) const;
        [[nodiscard]] std::list<Packet> receivePacket() const;

    private:
        Serial serial;
    };
}// namespace UBLOX

#endif//DEVICE_H
