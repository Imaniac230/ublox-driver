#ifndef DRIVER_H
#define DRIVER_H

#include <ublox/Device.hpp>

class Driver : public UBLOX::Device {
public:
    enum class AckResult { None, Acknowledged, NotAcknowledged };

    explicit Driver(const std::string &path);

    [[nodiscard]] AckResult acknowledged(UBLOX::Message) const;
};

#endif//DRIVER_H
