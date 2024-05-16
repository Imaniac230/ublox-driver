#include <iostream>

#include <Driver.hpp>
#include <DriverHelpers.hpp>
#include <ublox/packet/Configuration.hpp>
#include <ublox/packet/Monitor.hpp>
#include <ublox/packet/Navigation.hpp>
#include <ublox/packet/Receiver.hpp>

#include <spdlog/spdlog.h>

Driver::Driver(Config configuration)
    : UBLOX::Device(configuration.port.path, configuration.port.rate), config(std::move(configuration)) {
    //Configure communication ports
    if (!sendPacket(UBLOX::Packet::Cfg::UART(
                UBLOX::Packet::Cfg::Port::UARTID::Uart1, UBLOX::Packet::Cfg::Port::CharLength::Bits8,
                UBLOX::Packet::Cfg::Port::Parity::None, UBLOX::Packet::Cfg::Port::StopBits::One,
                toRaw(config.port.newSetRate), UBLOX::Packet::Cfg::Port::InProtocol::Ubx,
                UBLOX::Packet::Cfg::Port::OutProtocol::Ubx, false)))
        SPDLOG_WARN("Failed to send UART config packet for UART1.");
    if (config.port.newSetRate != config.port.rate) reconnect(config.port.path, config.port.newSetRate);
    if (!sendPacket(UBLOX::Packet::Cfg::USB(UBLOX::Packet::Cfg::Port::InProtocol::Ubx,
                                            UBLOX::Packet::Cfg::Port::OutProtocol::Ubx)))
        SPDLOG_WARN("Failed to send USB config packet.");

    //Configure the RTCM port for RTK
    switch (config.device.type) {
        case Driver::Type::Base:
            if (!sendPacket(UBLOX::Packet::Cfg::UART(
                        UBLOX::Packet::Cfg::Port::UARTID::Uart2, UBLOX::Packet::Cfg::Port::CharLength::Bits8,
                        UBLOX::Packet::Cfg::Port::Parity::None, UBLOX::Packet::Cfg::Port::StopBits::One,
                        toRaw(config.port.newSetRate), UBLOX::Packet::Cfg::Port::InProtocol::None,
                        UBLOX::Packet::Cfg::Port::OutProtocol::Rtcm3, false)))
                SPDLOG_WARN("Failed to send UART config packet for UART2.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1005,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1005.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1077,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1077.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1087,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1087.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1097,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1097.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1127,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1127.");
            if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::Rtcm1230,
                                                            UBLOX::Packet::Cfg::MessageRate::Rates{.uart2 = 1})))
                SPDLOG_WARN("Failed to send message rate config packet for rtcm 1230.");
            //            if (!sendPacket(UBLOX::Packet::Cfg::TimeMode(3 * 60, 60000 * 10)))
            //                SPDLOG_WARN("Failed to send time mode config packet for survey-in.");
            if (!sendPacket(UBLOX::Packet::Cfg::TimeMode(
                        UBLOX::Packet::Cfg::TimeMode::PositionECEFOrLLA{.XOrLatitude = 400015427,
                                                                        .YOrLongitude = 119062747,
                                                                        .ZOrAltitude = 480733765,
                                                                        .accuracy = 33220 * 10},
                        UBLOX::Packet::Cfg::TimeMode::HighPrecisionPositionECEFOrLLA{})))
                SPDLOG_WARN("Failed to send time mode config packet for survey-in.");
            break;
        case Driver::Type::Rover:
            if (!sendPacket(UBLOX::Packet::Cfg::UART(
                        UBLOX::Packet::Cfg::Port::UARTID::Uart2, UBLOX::Packet::Cfg::Port::CharLength::Bits8,
                        UBLOX::Packet::Cfg::Port::Parity::None, UBLOX::Packet::Cfg::Port::StopBits::One,
                        toRaw(config.port.newSetRate), UBLOX::Packet::Cfg::Port::InProtocol::Rtcm3,
                        UBLOX::Packet::Cfg::Port::OutProtocol::None, false)))
                SPDLOG_WARN("Failed to send UART config packet for UART2.");
            if (!sendPacket(UBLOX::Packet::Cfg::DifferentialGNSS(UBLOX::Packet::Cfg::DifferentialGNSS::Mode::RtkFixed)))
                SPDLOG_WARN("Failed to send differential GNSS config packet.");
            if (!sendPacket(UBLOX::Packet::Cfg::TimeMode()))
                SPDLOG_WARN("Failed to send time mode config packet for survey-in.");
            break;
        default:
            break;
    }

    //Configure rate
    if (!sendPacket(UBLOX::Packet::Cfg::NavigationRate(25, 3, UBLOX::Packet::Cfg::NavigationRate::TimeReference::Utc)))
        SPDLOG_WARN("Failed to send measurement and navigation rate configuration packet.");

    configureExampleData();

    //Receive data
    while (errno != EINTR) {
        std::list<UBLOX::Packet::Base> packets = receivePackets();
        if (packets.empty()) {
            SPDLOG_WARN("No packets received.");
        } else {
            printExampleData(std::move(packets));
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

void Driver::configureExampleData() const {
    //Configure periodic messages
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavEcefPositionSolution, 10)))
        SPDLOG_WARN("Failed to send message rate config packet for ECEF position.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavGeodeticPositionSolution, 10)))
        SPDLOG_WARN("Failed to send message rate config packet for geodetic position.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavOdometerSolution, 10)))
        SPDLOG_WARN("Failed to send message rate packet for odometer.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavReceiverNavigationStatus, 10)))
        SPDLOG_WARN("Failed to send message rate packet for navigation status.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavSurveyInData, 10)))
        SPDLOG_WARN("Failed to send message rate packet for survey-in data.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavSatelliteInformation, 10)))
        SPDLOG_WARN("Failed to send message rate packet for satellite information.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::NavRelativePositioningInformation, 10)))
        SPDLOG_WARN("Failed to send message rate packet for relative positioning information.");
    //Rxm messages don't output automatically, must also be enabled?
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::RxmDifferentialCorrectionInputStatus, 10)))
        SPDLOG_WARN("Failed to send message rate packet for differential correction input status.");
    if (!sendPacket(UBLOX::Packet::Cfg::MessageRate(UBLOX::Message::RxmRtcmInputStatus, 10)))
        SPDLOG_WARN("Failed to send message rate packet for rtcm input status.");

    //Poll message once
    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::MonReceiverAndSoftwareVersion)))
        SPDLOG_WARN("Failed to send polling packet for receiver and software version.");
    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::MonGnssInformationMessage)))
        SPDLOG_WARN("Failed to send polling packet for gnss information.");
    if (!sendPacket(UBLOX::Packet::Base(UBLOX::Message::CfgGnss)))
        SPDLOG_WARN("Failed to send polling packet for gnss information.");
}

void Driver::printExampleData(std::list<UBLOX::Packet::Base> packets) {
    for (auto &p: std::move(packets)) {
        switch (p.message()) {
            case UBLOX::Message::NavEcefPositionSolution: {
                UBLOX::Packet::Nav::PositionECEF positionEcef(std::move(p));
                if (!positionEcef.toData()) SPDLOG_WARN("Could not parse raw data to ECEF position.");
                std::cout << "\tNavEcefPositionSolution:" << std::endl;
                std::cout << "iTOW: " << positionEcef.getData().iTOWTimestampMillis
                          << " ms, X: " << positionEcef.getData().XCm << " cm, Y: " << positionEcef.getData().YCm
                          << " cm, Z: " << positionEcef.getData().ZCm
                          << " cm, accuracy: " << positionEcef.getData().positionAccuracyCm << " cm" << std::endl;
            } break;
            case UBLOX::Message::NavGeodeticPositionSolution: {
                UBLOX::Packet::Nav::PositionLLH positionLlh(std::move(p));
                if (!positionLlh.toData()) SPDLOG_WARN("Could not parse raw data to LLH position.");
                std::cout << "\tNavGeodeticPositionSolution:" << std::endl;
                std::cout << "iTOW: " << positionLlh.getData().iTOWTimestampMillis
                          << " ms, lon: " << std::setprecision(9)
                          << static_cast<float>(positionLlh.getData().longitude) * 1e-7
                          << " deg, lat: " << std::setprecision(9)
                          << static_cast<float>(positionLlh.getData().latitude) * 1e-7
                          << " deg, ellipsoid height: " << positionLlh.getData().ellipsoidHeightMm
                          << " mm, MSL height: " << positionLlh.getData().MSLHeightMm
                          << " mm, horizontal accuracy: " << positionLlh.getData().horizontalAccuracyMm
                          << " mm, vertical accuracy: " << positionLlh.getData().verticalAccuracyMm << " mm"
                          << std::endl;
            } break;
            case UBLOX::Message::MonReceiverAndSoftwareVersion: {
                UBLOX::Packet::Mon::ReceiverAndSoftwareVersion versions(std::move(p));
                if (!versions.toData()) SPDLOG_WARN("Could not parse raw data to version information.");
                std::cout << "\tMonReceiverAndSoftwareVersion:" << std::endl;
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
            case UBLOX::Message::NavOdometerSolution: {
                UBLOX::Packet::Nav::Odometer odometer(std::move(p));
                if (!odometer.toData()) SPDLOG_WARN("Could not parse raw data to odometer.");
                std::cout << "\tNavOdometerSolution:" << std::endl;
                std::cout << "iTOW: " << odometer.getData().iTOWTimestampMillis
                          << " ms, since last reset: " << odometer.getData().resetGroundDistanceM
                          << " m, since last cold start: " << odometer.getData().coldStartGroundDistanceM
                          << " m, accuracy (std): " << odometer.getData().resetGroundDistanceStd << " m" << std::endl;
            } break;
            case UBLOX::Message::NavReceiverNavigationStatus: {
                UBLOX::Packet::Nav::Status status(std::move(p));
                if (!status.toData()) SPDLOG_WARN("Could not parse raw data to navigation status.");
                std::cout << "\tNavReceiverNavigationStatus:" << std::endl;
                std::cout << "iTOW: " << status.getData().iTOWTimestampMillis
                          << " ms, fix type: " << status.getData().fixType
                          << ", flags: " << flagsStr(status.getData().flags)
                          << ", fix status: " << status.getData().fixStatus << ", flags2: " << status.getData().flags2
                          << ", time to 1st fix: " << status.getData().timeToFirstFixMillis
                          << " ms, elapsed since start: " << status.getData().elapsedSinceStartupMillis << " ms"
                          << std::endl;
            } break;
            case UBLOX::Message::NavSurveyInData: {
                UBLOX::Packet::Nav::SurveyInData data(std::move(p));
                if (!data.toData()) SPDLOG_WARN("Could not parse raw data to survey-in data.");
                std::cout << "\tNavSurveyInData:" << std::endl;
                std::cout << "iTOW: " << data.getData().iTOWTimestampMillis
                          << " ms, duration: " << data.getData().observationDurationSecs
                          << " s, mean X: " << data.getData().meanXEcefCm
                          << " cm, mean Y: " << data.getData().meanYEcefCm
                          << " cm, mean Z: " << data.getData().meanZEcefCm
                          << " cm, mean HP X: " << static_cast<float>(data.getData().meanHighPrecisionXEcef) * 0.01
                          << " cm, mean HP Y: " << static_cast<float>(data.getData().meanHighPrecisionYEcef) * 0.01
                          << " cm, mean HP Z: " << static_cast<float>(data.getData().meanHighPrecisionZEcef) * 0.01
                          << " cm, mean accuracy: " << static_cast<float>(data.getData().meanPositionAccuracy) * 0.01
                          << " cm, observations: " << data.getData().numberOfUsedObservations
                          << ", valid: " << data.getData().valid << ", in progress: " << data.getData().inProgress
                          << std::endl;
            } break;
            case UBLOX::Message::NavSatelliteInformation: {
                UBLOX::Packet::Nav::SatelliteInfo info(std::move(p));
                if (!info.toData()) SPDLOG_WARN("Could not parse raw data to satellite information.");
                std::cout << "\tNavSatelliteInformation:" << std::endl;
                std::cout << "iTOW: " << info.getData().iTOWTimestampMillis
                          << " ms, satellites used: " << static_cast<int>(info.getData().numberOfSatellites)
                          << " -> { ";
                for (const auto &s: info.getData().satellites) { std::cout << "\n " << s; }
                std::cout << " }" << std::endl;
            } break;
            case UBLOX::Message::MonGnssInformationMessage: {
                UBLOX::Packet::Mon::GNSSInformation info(std::move(p));
                if (!info.toData()) SPDLOG_WARN("Could not parse raw data to gnss information.");
                std::cout << "\tMonGnssInformationMessage:" << std::endl;
                std::cout << "supported: " << info.getData().supportedGnss
                          << ", default: " << info.getData().defaultGnss << ", enabled: " << info.getData().enabledGnss
                          << std::endl;
            } break;
            case UBLOX::Message::NavRelativePositioningInformation: {
                UBLOX::Packet::Nav::RelativePositionNED info(std::move(p));
                if (!info.toData()) SPDLOG_WARN("Could not parse raw data to relative positioning information.");
                std::cout << "\tNavRelativePositioningInformation:" << std::endl;
                std::cout << "iTOW: " << info.getData().iTOWTimestampMillis
                          << " ms, base id: " << info.getData().referenceStationId
                          << ", north: " << info.getData().northCm << " cm, east: " << info.getData().eastCm
                          << " cm, down: " << info.getData().downCm
                          << " cm, vector length: " << info.getData().vectorLengthCm
                          << " cm, vector heading: " << static_cast<float>(info.getData().vectorHeading) * 1e-5
                          << " deg, north HP: " << static_cast<float>(info.getData().highPrecisionNorth) * 0.01
                          << " cm, east HP: " << static_cast<float>(info.getData().highPrecisionEast) * 0.01
                          << " cm, down HP: " << static_cast<float>(info.getData().highPrecisionDown) * 0.01
                          << " cm, vector length HP: "
                          << static_cast<float>(info.getData().highPrecisionVectorLength) * 0.01
                          << " cm, north accuracy: " << static_cast<float>(info.getData().accuracyNorth) * 0.01
                          << " cm, east accuracy: " << static_cast<float>(info.getData().accuracyEast) * 0.01
                          << " cm, down accuracy: " << static_cast<float>(info.getData().accuracyDown) * 0.01
                          << " cm, vector length accuracy: "
                          << static_cast<float>(info.getData().accuracyVectorLength) * 0.01
                          << " cm, vector heading accuracy: "
                          << static_cast<float>(info.getData().accuracyVectorHeading) * 1e-5
                          << " deg, flags: " << info.getData().flags << std::endl;
            } break;
            case UBLOX::Message::RxmDifferentialCorrectionInputStatus: {
                UBLOX::Packet::Rxm::DifferentialCorrectionsStatus status(std::move(p));
                if (!status.toData()) SPDLOG_WARN("Could not parse raw data to differential correction input status.");
                SPDLOG_INFO("Differential correction message parsed:");
                std::cout << "\tRxmDifferentialCorrectionInputStatus:" << std::endl;
                std::cout << "status: " << status.getData().status << ", type: " << status.getData().type
                          << ", subtype: " << status.getData().subType << std::endl;
            } break;
            case UBLOX::Message::RxmRtcmInputStatus: {
                UBLOX::Packet::Rxm::RTCMStatus status(std::move(p));
                if (!status.toData()) SPDLOG_WARN("Could not parse raw data to rtcm input status.");
                SPDLOG_INFO("RTCM message parsed:");
                std::cout << "\tRxmRtcmInputStatus:" << std::endl;
                std::cout << "status: " << status.getData().status << ", subtype: " << status.getData().subType
                          << ", base id: " << status.getData().referenceStationId << std::endl;
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
