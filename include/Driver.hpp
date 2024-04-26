#ifndef DRIVER_H
#define DRIVER_H

#include <ublox/Device.hpp>

class Driver : public UBLOX::Device {
public:
    explicit Driver(const std::string &path);
};

#endif//DRIVER_H
