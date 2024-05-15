#ifndef DRIVER_HELPERS_H
#define DRIVER_HELPERS_H

#include <fstream>
#include <iostream>

#include <ublox/packet/Navigation.hpp>
#include <ublox/packet/Monitor.hpp>
#include <ublox/packet/Receiver.hpp>

//NOTE: custom data parsing for prints, implemented here, as it's only valid for this example and doesn't
//  need to bloat the message definition

std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::FixType &fix);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::FixStatus &status);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::Status::Data::Flag2 &flag);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::SatelliteInfo::Data::Satellite &satellite);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Mon::GNSSInformation::Data::Flag &flag);
std::ostream &operator<<(std::ostream &stream,
                         const UBLOX::Packet::Rxm::DifferentialCorrectionsStatus::Data::Status &status);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Rxm::RTCMStatus::Data::Status &status);
std::ostream &operator<<(std::ostream &stream, const UBLOX::Packet::Nav::RelativePositionNED::Data::Flags &flags);

std::string flagsStr(uint8_t flags);

#endif//DRIVER_HELPERS_H
