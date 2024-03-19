#include <iostream>

#include <ublox/Device.hpp>
#include <ublox/packet/ConfigUSBPacket.hpp>

#include <util/Utils.hpp>

#include <spdlog/spdlog.h>

bool UBLOX::Device::sendPacket(const Packet::Base &packet) const {
    if (packet.empty()) return false;
    return serial.writeBytes(&*packet.serialized().begin(), packet.size()) == packet.size();
}

std::list<UBLOX::Packet::Base> UBLOX::Device::receivePacket() const {
    std::vector<uint8_t> rxBuffer(1000, 0);
    const size_t read = serial.readBytes(1000, reinterpret_cast<char *>(rxBuffer.data()));

    //        std::cout << "read bytes: " << read << std::endl;
    //        if ((rxBuffer[0] == SyncChar::FirstByte) && (rxBuffer[1] == SyncChar::SecondByte)) {
    //            std::cout << "{" << std::hex;
    //            for (size_t i = 0; i < read; ++i) { std::cout << "0x" << static_cast<int>(rxBuffer[i]) << ", "; }
    //            std::cout << "}" << std::dec << std::endl;
    //        } else {
    //            for (size_t i = 0; i < read; ++i) { std::cout << rxBuffer[i]; }
    //        }

    std::list<Packet::Base> packets{};
    for (int i = 0; i < static_cast<int>(read);) {
        if ((rxBuffer[i] == SyncChar::FirstByte) && (rxBuffer[i + 1] == SyncChar::SecondByte)) {
            const auto dataSize = Utils::deserializeLEInt<uint16_t>(
                    std::vector<uint8_t>(rxBuffer.begin() + i + 4, rxBuffer.begin() + i + 6).data());
            Packet::Base rcvPacket(static_cast<MessageClass>(rxBuffer[i + 2]), rxBuffer[i + 3],
                                   std::vector<uint8_t>(rxBuffer.begin() + i + 6, rxBuffer.begin() + i + 6 + dataSize));
            if (rcvPacket.checksumValid(
                        std::array<uint8_t, 2>{rxBuffer[i + 6 + dataSize], rxBuffer[i + 6 + dataSize + 1]})) {
                packets.push_back(std::move(rcvPacket));
            } else {
                spdlog::warn("Invalid checksum, dropping packet.");
            }
            i += 6 + dataSize + 2;
        }
    }

    return packets;
}
