#include <iostream>

#include <ublox/ConfigUSBPacket.hpp>
#include <ublox/Device.hpp>

#include <util/Utils.hpp>

#include <spdlog/spdlog.h>

bool UBLOX::Device::sendPacket(const Packet &packet) const {
    if (packet.empty()) return false;
    return serial.writeBytes(&*packet.serialized().begin(), packet.size()) == packet.size();
}

std::list<UBLOX::Packet> UBLOX::Device::receivePacket() const {
    std::vector<uint8_t> rxBuffer(100, 0);
    const size_t read = serial.readBytes(100, reinterpret_cast<char *>(rxBuffer.data()));

    //    if ((static_cast<SyncChar>(rxBuffer[0]) == SyncChar::FirstByte) &&
    //        (static_cast<SyncChar>(rxBuffer[1]) == SyncChar::SecondByte)) {
    //        std::cout << "{" << std::hex;
    //        for (size_t i = 0; i < read; ++i) { std::cout << "0x" << static_cast<int>(rxBuffer[i]) << ", "; }
    //        std::cout << "}" << std::dec << std::endl;
    //    } else {
    //        for (size_t i = 0; i < read; ++i) { std::cout << rxBuffer[i]; }
    //    }

    std::list<Packet> packets{};
    for (int i = 0; i < static_cast<int>(read);) {
        if ((static_cast<SyncChar>(rxBuffer[i]) == SyncChar::FirstByte) &&
            (static_cast<SyncChar>(rxBuffer[i + 1]) == SyncChar::SecondByte)) {
            const auto dataSize = Utils::deserializeLEInt<uint16_t>(
                    std::vector<uint8_t>(rxBuffer.begin() + i + 4, rxBuffer.begin() + i + 6).data());
            const Packet rcvPacket(static_cast<MessageClass>(rxBuffer[i + 2]), rxBuffer[i + 3],
                                   std::vector<uint8_t>(rxBuffer.begin() + i + 6, rxBuffer.begin() + i + 6 + dataSize));
            if (rcvPacket.checksumValid(
                        std::array<uint8_t, 2>{rxBuffer[i + 6 + dataSize], rxBuffer[i + 6 + dataSize + 1]})) {
                packets.push_back(rcvPacket);
            } else {
                spdlog::warn("Invalid checksum, dropping packet.");
            }
            i += 6 + dataSize + 2;
        }
    }

    return packets;
}
