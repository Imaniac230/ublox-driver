#ifndef DRIVER_H
#define DRIVER_H

#include <fstream>

#include <ublox/Device.hpp>

#include <nlohmann/json.hpp>

class Driver : public UBLOX::Device {
public:
    enum class Type : uint8_t { None, Base, Rover };
    enum class ObservationMessage : uint8_t { Msm4, Msm7 };
    enum class BasePositionMode : uint8_t { SurveyIn, Fixed, Rover };

    struct Config {
        struct Port {
            std::string path{};
            Serial::BaudRate rate{};
            Serial::BaudRate newSetRate{};

            friend void from_json(const nlohmann::json &j, Driver::Config::Port &p);
        };
        struct Device {
            struct Base {
                struct SurveyIn {
                    uint32_t minimumDurationSecs = 0;
                    uint32_t accuracyLimitMm = 0;

                    friend void from_json(const nlohmann::json &j, Driver::Config::Device::Base::SurveyIn &s);
                };
                struct FixedPosition {
                    double latitude = 0.0f;
                    double longitude = 0.0f;
                    double altitude = 0.0f;
                    uint32_t accuracyMm = 0;

                    friend void from_json(const nlohmann::json &j, Driver::Config::Device::Base::FixedPosition &f);
                };

                ObservationMessage observationType{};
                BasePositionMode positionMode{};
                SurveyIn surveyIn{};
                FixedPosition fixedPosition{};

                friend void from_json(const nlohmann::json &j, Driver::Config::Device::Base &b);
            };

            Type type{};
            Serial::BaudRate rtcmRate{};
            Base base{};

            friend void from_json(const nlohmann::json &j, Driver::Config::Device &d);
        };
        struct Debug {
            bool satelliteInformation = false;
            bool correctionDataInput = false;

            friend void from_json(const nlohmann::json &j, Driver::Config::Debug &d);
        };

        Port port{};
        Device device{};
        Debug debug{};

        friend void from_json(const nlohmann::json &j, Driver::Config &c);
        static Config fromJson(const std::string &path);
    };

    explicit Driver(Config configuration);

private:
    Config config;

    [[nodiscard]] bool configureRtkDevice() const;
    void configureOutputData() const;
    static void printOutputData(std::list<UBLOX::Packet::Base> data);
};

inline void from_json(const nlohmann::json &j, Driver::Config::Port &p) {
    j.at("path").get_to(p.path);
    j.at("rate").get_to(p.rate);
    j.at("newSetRate").get_to(p.newSetRate);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Device &d) {
    j.at("type").get_to(d.type);
    j.at("rtcmRate").get_to(d.rtcmRate);
    j.at("base").get_to(d.base);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Device::Base &b) {
    j.at("observationMessageType").get_to(b.observationType);
    j.at("positionMode").get_to(b.positionMode);
    j.at("surveyIn").get_to(b.surveyIn);
    j.at("fixedPosition").get_to(b.fixedPosition);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Device::Base::SurveyIn &s) {
    j.at("minimumDurationSeconds").get_to(s.minimumDurationSecs);
    j.at("accuracyLimitMm").get_to(s.accuracyLimitMm);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Device::Base::FixedPosition &f) {
    j.at("latitude").get_to(f.latitude);
    j.at("longitude").get_to(f.longitude);
    j.at("altitude").get_to(f.altitude);
    j.at("accuracyMm").get_to(f.accuracyMm);
}

inline void from_json(const nlohmann::json &j, Driver::Config::Debug &d) {
    j.at("satelliteInformation").get_to(d.satelliteInformation);
    j.at("correctionDataInput").get_to(d.correctionDataInput);
}

inline void from_json(const nlohmann::json &j, Driver::Config &c) {
    j.at("port").get_to(c.port);
    j.at("device").get_to(c.device);
    j.at("debug").get_to(c.debug);
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

NLOHMANN_JSON_SERIALIZE_ENUM(Driver::ObservationMessage, {
                                                                 {Driver::ObservationMessage::Msm4, "Msm4"},
                                                                 {Driver::ObservationMessage::Msm7, "Msm7"},
                                                         })

NLOHMANN_JSON_SERIALIZE_ENUM(Driver::BasePositionMode, {{Driver::BasePositionMode::SurveyIn, "SurveyIn"},
                                                        {Driver::BasePositionMode::Fixed, "Fixed"},
                                                        {Driver::BasePositionMode::Rover, "Rover"}})

#endif//DRIVER_H
