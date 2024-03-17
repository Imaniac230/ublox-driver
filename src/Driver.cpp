#include <iostream>

#include <Driver.hpp>

#include <ublox/ConfigMessageRatePacket.hpp>
#include <ublox/ConfigUSBPacket.hpp>

#include <spdlog/spdlog.h>

Driver::Driver(const std::string &path) : UBLOX::Device(path) {
    if (!sendPacket(UBLOX::ConfigUSBPacket(UBLOX::ConfigPortPacket::InProtocol::Ubx,
                                           UBLOX::ConfigPortPacket::OutProtocol::Ubx |
                                                   UBLOX::ConfigPortPacket::OutProtocol::Rtcm3))) {
        spdlog::warn("Failed to send packet.");
    }
    if (acknowledged(UBLOX::Message::CfgPort) == AckResult::Acknowledged) {
        spdlog::info("USB parameters set correctly.");
    } else {
        spdlog::warn("USB parameters not set.");
    }

    if (!sendPacket(UBLOX::ConfigMessageRatePacket(UBLOX::Message::NavEcefPositionSolution, 0x01)))
        spdlog::warn("Failed to send packet.");
    if (acknowledged(UBLOX::Message::CfgMessageRate) == AckResult::Acknowledged) {
        spdlog::info("Message rate parameters set correctly.");
    } else {
        spdlog::warn("Message rate parameters not set.");
    }

    if (!sendPacket(UBLOX::Packet(UBLOX::Message::CfgHighNavigationRate, {0x10, 0x00, 0x00, 0x00}))) {
        spdlog::warn("Failed to send packet.");
    }
    if (acknowledged(UBLOX::Message::CfgHighNavigationRate) == AckResult::Acknowledged) {
        spdlog::info("High navigation rate set correctly.");
    } else {
        spdlog::warn("High navigation rate not set.");
    }

    while (errno != EINTR) {
        if (!sendPacket(UBLOX::Packet(UBLOX::Message::NavHighPrecisionEcefPositionSolution)))
            spdlog::warn("Failed to send packet");
        const std::list<UBLOX::Packet> packets = receivePacket();
        if (packets.empty()) spdlog::warn("No packet received.");
        for (const auto &p: packets) {
            std::cout << "{";
            for (const auto &d: p.data()) { std::cout << static_cast<int>(d) << ", "; }
            std::cout << "}" << std::endl;
        }

        if (!sendPacket(UBLOX::Packet(UBLOX::Message::MonExtendedHardwareStatus)))
            spdlog::warn("Failed to send packet");
        const std::list<UBLOX::Packet> packets2 = receivePacket();
        if (packets.empty()) spdlog::warn("No packet received.");
        for (const auto &p: packets2) {
            std::cout << "{";
            for (const auto &d: p.data()) { std::cout << static_cast<int>(d) << ", "; }
            std::cout << "}" << std::endl;
        }
    }
}

Driver::AckResult Driver::acknowledged(const UBLOX::Message message) const {
    const std::list<UBLOX::Packet> ack = receivePacket();
    if (ack.empty()) {
        spdlog::warn("No packet received.");
        return AckResult::None;
    }
    for (const auto &p: ack) {
        const std::vector<uint8_t> data = p.data();
        switch (p.message()) {
            case UBLOX::Message::AckAcknowledged:
                if (UBLOX::toMessage(data[0], data[1]) == message) return AckResult::Acknowledged;
                break;
            case UBLOX::Message::AckNotAcknowledged:
                if (UBLOX::toMessage(data[0], data[1]) == message) return AckResult::NotAcknowledged;
                break;
            default:
                break;
        }
    }

    return AckResult::None;
}