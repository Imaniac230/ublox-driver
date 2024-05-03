#ifndef DRIVER_H
#define DRIVER_H

#include <fstream>

#include <nlohmann/json.hpp>

#include <nlohmann/json_fwd.hpp>
#include <ublox/Device.hpp>

class Driver : public UBLOX::Device {
public:
    struct Config {
        std::string portPath{};

        friend void from_json(const nlohmann::json &j, Driver::Config &c);
        static Config fromJson(const std::string &path);
    };

    explicit Driver(Config configuration);

private:
    Config config;
};

inline void from_json(const nlohmann::json &j, Driver::Config &c) { j.at("portPath").get_to(c.portPath); }

#endif//DRIVER_H
