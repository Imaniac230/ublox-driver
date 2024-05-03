#include <iostream>

#include <Driver.hpp>

#include <ublox/packet/ConfigMessageRatePacket.hpp>
#include <ublox/packet/ConfigUARTPacket.hpp>
#include <ublox/packet/ConfigUSBPacket.hpp>
#include <ublox/packet/MonitorReceiverAndSoftwareVersionPacket.hpp>
#include <ublox/packet/NavPositionECEFPakcet.hpp>

#include <spdlog/spdlog.h>

Driver::Driver(Config configuration) : UBLOX::Device(configuration.portPath), config(std::move(configuration)) {
    //Configure communication ports
    if (!sendPacket(UBLOX::Packet::ConfigUART(
                UBLOX::Packet::ConfigUART::UART::Uart1, UBLOX::Packet::ConfigUART::CharLength::Bits8,
                UBLOX::Packet::ConfigUART::Parity::None, UBLOX::Packet::ConfigUART::StopBits::One, 115200,
                UBLOX::Packet::InProtocol::Ubx, UBLOX::Packet::OutProtocol::Ubx, false)))
        SPDLOG_WARN("Failed to send packet.");
    if (!sendPacket(UBLOX::Packet::ConfigUSB(UBLOX::Packet::InProtocol::Ubx, UBLOX::Packet::OutProtocol::Ubx)))
        SPDLOG_WARN("Failed to send packet.");
    if (!sendPacket(UBLOX::Packet::ConfigUART(
                UBLOX::Packet::ConfigUART::UART::Uart2, UBLOX::Packet::ConfigUART::CharLength::Bits8,
                UBLOX::Packet::ConfigUART::Parity::None, UBLOX::Packet::ConfigUART::StopBits::One, 115200,
                UBLOX::Packet::InProtocol::Rtcm3, UBLOX::Packet::OutProtocol::None, false)))
        SPDLOG_WARN("Failed to send packet.");

    //Configure periodic messages
    if (!sendPacket(UBLOX::Packet::ConfigMessageRate(UBLOX::Message::NavEcefPositionSolution, 0x01)))
        SPDLOG_WARN("Failed to send packet.");
    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::CfgHighNavigationRate, {0x10, 0x00, 0x00, 0x00})))
        SPDLOG_WARN("Failed to send packet.");

    //Poll message once
    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::MonReceiverAndSoftwareVersion)))
        SPDLOG_WARN("Failed to send packet");
    //    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::NavEcefPositionSolution)))
    //        SPDLOG_WARN("Failed to send packet");

    //Receive data
    while (errno != EINTR) {
        std::list<UBLOX::Packet::Base> packets = receivePackets();
        if (packets.empty()) SPDLOG_WARN("No packets received.");
        for (auto &p: packets) {
            switch (p.message()) {
                case UBLOX::Message::NavEcefPositionSolution: {
                    UBLOX::Packet::NavPositionECEF positionEcef(std::move(p));
                    if (!positionEcef.toData()) SPDLOG_WARN("Could not parse raw data to ECEF position");
                    std::cout << "iTOW: " << positionEcef.getData().iTOWTimestampMillis
                              << " ms, X: " << positionEcef.getData().XCm << " cm, Y: " << positionEcef.getData().YCm
                              << " cm, Z: " << positionEcef.getData().ZCm
                              << " cm, accuracy: " << positionEcef.getData().positionAccuracyCm << " cm" << std::endl;
                } break;
                case UBLOX::Message::MonReceiverAndSoftwareVersion: {
                    UBLOX::Packet::MonitorReceiverAndSoftwareVersion versions(std::move(p));
                    if (!versions.toData()) SPDLOG_WARN("Could not parse raw data to version information");
                    std::cout << "SW version: "
                              << std::string(reinterpret_cast<const char *>(versions.getData().swVersion.data()))
                              << ", HW version: "
                              << std::string(reinterpret_cast<const char *>(versions.getData().hwVersion.data()))
                              << ", extensions: ";
                    for (const auto &e: versions.getData().extensions) {
                        if (!e.empty()) std::cout << std::string(reinterpret_cast<const char *>(e.data())) << ", ";
                    }
                    std::cout << std::endl;
                } break;
                case UBLOX::Message::AckAcknowledged: {
                    const std::vector<uint8_t> data = p.rawData();
                    SPDLOG_INFO("Message {} acknowledged.", UBLOX::asStr(UBLOX::toMessage(data[0], data[1])));
                } break;
                case UBLOX::Message::AckNotAcknowledged: {
                    const std::vector<uint8_t> data = p.rawData();
                    SPDLOG_ERROR("Message {} NOT acknowledged.", UBLOX::asStr(UBLOX::toMessage(data[0], data[1])));
                } break;
                default: {
                    std::cout << "unknown message: " << p.message() << ", DATA: {" << std::hex;
                    for (const auto &d: p.rawData()) { std::cout << "0x" << static_cast<int>(d) << ", "; }
                    std::cout << std::dec << "}" << std::endl;
                }
            }
        }
    }
}

Driver::Config Driver::Config::fromJson(const std::string &path) {
    std::ifstream configFile(path);
    nlohmann::json configJson;
    try {
        configFile >> configJson;
    } catch (...) { throw std::runtime_error{"Could not parse config file '" + path + "', invalid input."}; }

    Config cfg;
    from_json(configJson, cfg);
    return cfg;
}