#include <Driver.hpp>

int main(int argc, char **argv) {
    std::string configFile = "config.json";
    if (argc >= 2) configFile = argv[1];

    Driver(Driver::Config::fromJson(configFile));
}