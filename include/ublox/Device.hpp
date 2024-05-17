#ifndef DEVICE_H
#define DEVICE_H

#include <iterator>
#include <ostream>
#include <string>

#include <ublox/Registers.hpp>
#include <ublox/packet/Base.hpp>
#include <util/RingBuffer.hpp>
#include <util/Serde.hpp>
#include <util/Serial.hpp>

namespace UBLOX {
    class Device {
    public:
        //TODO(constants): set min bytes using defined constants in packet base
        explicit Device(const std::string &path, const Serial::BaudRate rate) : serial(Serial(path, rate, 5)) {}
        virtual ~Device() = default;

        [[nodiscard]] bool sendPacket(const Packet::Base &packet) const;
        [[nodiscard]] std::list<Packet::Base> receivePackets();

        void disconnect() { serial.close(); }
        void reconnect(const std::string &path, const Serial::BaudRate rate) {
            //FIXME(dtor): can't rely on the destructor here and instead must manually close the old port before assignment,
            // destructor would actually close the new port (called on the new instance??)
            disconnect();
            serial = Serial(path, rate, 5);
        }

    private:
        Serial serial;
        //TODO(constants): set size using defined constants in packet base (max expected size, etc.)
        RingBuffer<uint8_t, std::array<uint8_t, 10000>> buffer{};
    };
}// namespace UBLOX

#endif//DEVICE_H
