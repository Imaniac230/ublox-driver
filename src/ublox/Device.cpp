#include <iostream>

#include <ublox/Device.hpp>
#include <util/Serde.hpp>

#include <spdlog/spdlog.h>

bool UBLOX::Device::sendPacket(const Packet::Base &packet) const {
    if (packet.empty()) return false;
    return static_cast<size_t>(serial.writeBytes(&*packet.serialized().begin(), packet.size())) == packet.size();
}

std::list<UBLOX::Packet::Base> UBLOX::Device::receivePackets() {
    //FIXME(data-parsing): the accumulation logic should be extracted out into a separate method and this should only create the,
    // packets based on the result from accumulate()
    if (buffer.full()) SPDLOG_WARN("Buffer full, dropping packet!");

    //FIXME(data-parsing): this is very counter-productive - we're creating a new vector and probably making a copy each time,
    // we should probably (also) give access to the buffer directly instead
    std::vector<uint8_t> rxBuffer(buffer.capacity(), 0);
    const ssize_t read = serial.readBytes(buffer.capacity(), reinterpret_cast<char *>(rxBuffer.data()));
    if (read < 0) return {};

    if (!buffer.write(std::vector<uint8_t>{rxBuffer.begin(), rxBuffer.begin() + static_cast<int>(read)}))
        SPDLOG_ERROR("Failed to write data to buffer!");

    //    SPDLOG_INFO("read bytes: {}", read);
    //    if ((rxBuffer[0] == SyncChar::FirstByte) && (rxBuffer[1] == SyncChar::SecondByte)) {
    //        std::cout << "{" << std::hex;
    //        for (size_t i = 0; i < read; ++i) { std::cout << "0x" << static_cast<int>(rxBuffer[i]) << ", "; }
    //        std::cout << "}" << std::dec << std::endl;
    //    } else {
    //        for (size_t i = 0; i < read; ++i) { std::cout << rxBuffer[i]; }
    //    }

    std::list<Packet::Base> packets{};
    //TODO(data-parsing): define constants (HEADER_BYTES, CHECKSUM_BYTES, etc)
    bool accumulating = false;
    while ((buffer.getActiveElements() >= 6) && !accumulating) {
        accumulating = false;
        if ((buffer.tailOffset(0).value_or(0) == SyncChar::FirstByte) &&
            (buffer.tailOffset(1).value_or(0) == SyncChar::SecondByte)) {
            const auto dataSize = Serde::deserializeLEInt<uint16_t>(
                    std::array<uint8_t, 2>{buffer.tailOffset(4).value_or(0), buffer.tailOffset(5).value_or(0)}.data());
            if (buffer.getActiveElements() >= (6 + dataSize + 2)) {
                //FIXME(data-parsing): this is messy, we should probably (also) provide direct access to the buffer, or at least implement
                // a Base packet constructor from raw bytes
                const auto pckt = buffer.read<std::vector<uint8_t>>(6 + dataSize + 2);
                if (pckt.has_value()) {
                    Packet::Base rcvPacket(
                            static_cast<MessageClass>(pckt.value()[2]), pckt.value()[3],
                            std::vector<uint8_t>(pckt.value().begin() + 6, pckt.value().begin() + 6 + dataSize));
                    if (rcvPacket.checksumValid(
                                std::array<uint8_t, 2>{pckt.value()[6 + dataSize], pckt.value()[6 + dataSize + 1]})) {
                        packets.push_back(std::move(rcvPacket));
                    } else {
                        SPDLOG_WARN("Invalid checksum, dropping packet.");
                    }
                } else {
                    SPDLOG_WARN("Failed to read data from buffer!");
                }
            } else {
                accumulating = true;
            }
        } else {
            SPDLOG_WARN("No valid packet found, popping one data element from buffer.");
            buffer.read<std::list<uint8_t>>(1).has_value();
        }
    }

    return packets;
}
