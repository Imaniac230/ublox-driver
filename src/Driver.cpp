#include <iostream>

#include <Driver.hpp>

#include <ublox/packet/ConfigMessageRatePacket.hpp>
#include <ublox/packet/ConfigUSBPacket.hpp>
#include <ublox/packet/MonitorReceiverAndSoftwareVersionPacket.hpp>
#include <ublox/packet/NavPositionECEFPakcet.hpp>

#include <spdlog/spdlog.h>

Driver::Driver(const std::string &path) : UBLOX::Device(path) {
    if (!sendPacket(UBLOX::Packet::ConfigUSB(UBLOX::Packet::ConfigPort::InProtocol::Ubx,
                                             UBLOX::Packet::ConfigPort::OutProtocol::Ubx |
                                                     UBLOX::Packet::ConfigPort::OutProtocol::Rtcm3))) {
        spdlog::warn("Failed to send packet.");
    }
    if (acknowledged(UBLOX::Message::CfgPort) == AckResult::Acknowledged) {
        spdlog::info("USB parameters set correctly.");
    } else {
        spdlog::warn("USB parameters not set.");
    }

    if (!sendPacket(UBLOX::Packet::ConfigMessageRate(UBLOX::Message::NavEcefPositionSolution, 0x01)))
        spdlog::warn("Failed to send packet.");
    if (acknowledged(UBLOX::Message::CfgMessageRate) == AckResult::Acknowledged) {
        spdlog::info("Message rate parameters set correctly.");
    } else {
        spdlog::warn("Message rate parameters not set.");
    }

    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::CfgHighNavigationRate, {0x10, 0x00, 0x00, 0x00}))) {
        spdlog::warn("Failed to send packet.");
    }
    if (acknowledged(UBLOX::Message::CfgHighNavigationRate) == AckResult::Acknowledged) {
        spdlog::info("High navigation rate set correctly.");
    } else {
        spdlog::warn("High navigation rate not set.");
    }

    while (errno != EINTR) {
        if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::NavEcefPositionSolution)))
            spdlog::warn("Failed to send packet");
        std::list<UBLOX::Packet::Base> packets = receivePacket();
        if (packets.empty()) spdlog::warn("No packet received.");
        for (auto &p: packets) {
            if (p.message() == UBLOX::Message::NavEcefPositionSolution) {
                UBLOX::Packet::NavPositionECEF positionEcef(std::move(p));
                if (!positionEcef.toData()) spdlog::warn("Could not parse raw data to ECEF position");
                std::cout << "iTOW: " << positionEcef.getData().iTOWTimestampMillis
                          << " ms, X: " << positionEcef.getData().XCm << " cm, Y: " << positionEcef.getData().YCm
                          << " cm, Z: " << positionEcef.getData().ZCm
                          << " cm, accuracy: " << positionEcef.getData().positionAccuracyCm << " cm" << std::endl;
            } else {
                std::cout << p.message() << ", DATA: {" << std::hex;
                for (const auto &d: p.rawData()) { std::cout << "0x" << static_cast<int>(d) << ", "; }
                std::cout << std::dec << "}" << std::endl;
            }
        }

        if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::MonReceiverAndSoftwareVersion)))
            spdlog::warn("Failed to send packet");
        std::list<UBLOX::Packet::Base> packets2 = receivePacket();
        if (packets.empty()) spdlog::warn("No packet received.");
        for (UBLOX::Packet::Base &p: packets2) {
            if (p.message() == UBLOX::Message::MonReceiverAndSoftwareVersion) {
                UBLOX::Packet::MonitorReceiverAndSoftwareVersion versions(std::move(p));
                if (!versions.toData()) spdlog::warn("Could not parse raw data to version information");
                std::cout << "SW version: "
                          << std::string(reinterpret_cast<const char *>(versions.getData().swVersion.data()))
                          << ", HW version: "
                          << std::string(reinterpret_cast<const char *>(versions.getData().hwVersion.data()))
                          << ", extensions: ";
                for (const auto &e: versions.getData().extensions) {
                    if (!e.empty()) std::cout << std::string(reinterpret_cast<const char *>(e.data())) << ", ";
                }
                std::cout << std::endl;
            } else {
                std::cout << p.message() << ", DATA: {" << std::hex;
                for (const auto &d: p.rawData()) { std::cout << static_cast<int>(d) << ", "; }
                std::cout << std::dec << "}" << std::endl;
            }
        }
    }
}

Driver::AckResult Driver::acknowledged(const UBLOX::Message message) const {
    const std::list<UBLOX::Packet::Base> ack = receivePacket();
    if (ack.empty()) {
        spdlog::warn("No packet received.");
        return AckResult::None;
    }
    for (const auto &p: ack) {
        const std::vector<uint8_t> data = p.rawData();
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