#ifndef DRIVER_H
#define DRIVER_H

#include <fstream>

#include <ublox/Device.hpp>

#include <nlohmann/json.hpp>

class Driver : public UBLOX::Device {
public:
    enum class Type : uint8_t { None, Base, Rover };

    struct Config {
        struct Port {
            std::string path{};
            Serial::BaudRate rate{};
            Serial::BaudRate newSetRate{};

            friend void from_json(const nlohmann::json &j, Driver::Config::Port &p);
        };
        struct Device {
            Type type{};

            friend void from_json(const nlohmann::json &j, Driver::Config::Device &d);
        };

        Port port{};
        Device device{};

        friend void from_json(const nlohmann::json &j, Driver::Config &c);
        static Config fromJson(const std::string &path);
    };

    explicit Driver(Config configuration);

private:
    Config config;

    void configureExampleData() const;
    static void printExampleData(std::list<UBLOX::Packet::Base> data);
};

inline void from_json(const nlohmann::json &j, Driver::Config::Port &p) {
    j.at("path").get_to(p.path);
    j.at("rate").get_to(p.rate);
    j.at("newSetRate").get_to(p.newSetRate);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Device &d) { j.at("type").get_to(d.type); }

inline void from_json(const nlohmann::json &j, Driver::Config &c) {
    j.at("port").get_to(c.port);
    j.at("device").get_to(c.device);
}

NLOHMANN_JSON_SERIALIZE_ENUM(Serial::BaudRate, {{Serial::BaudRate::Baud9600, "Baud9600"},
                                                {Serial::BaudRate::Baud19200, "Baud19200"},
                                                {Serial::BaudRate::Baud38400, "Baud38400"},
                                                {Serial::BaudRate::Baud57600, "Baud57600"},
                                                {Serial::BaudRate::Baud115200, "Baud115200"},
                                                {Serial::BaudRate::Baud921600, "Baud921600"}})

NLOHMANN_JSON_SERIALIZE_ENUM(Driver::Type, {{Driver::Type::None, "None"},
                                            {Driver::Type::Base, "Base"},
                                            {Driver::Type::Rover, "Rover"}})

#endif//DRIVER_H
