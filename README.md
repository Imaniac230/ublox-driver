# Ublox minimal driver

This is a minimal functional interfacing example for experimenting with `ublox` GNSS receiver devices using the `UBX` protocol.

It was developed and tested mainly with the hardware configuration of `simpleRTK2B` modules, using `ZED-F9P` receivers:
* https://www.ardusimple.com/product/simplertk2b/
* https://www.ardusimple.com/product/simplertk2blite/

Strict compliance with any particular firmware version is not guaranteed here, the used configuration was:
```
EXT CORE 1.00 (0fa0ae)
HW version: 00190000
ROM BASE 0x118B2060
FWVER=HPG 1.32
PROTVER=27.31
MOD=ZED-F9P
GPS;GLO;GAL;BDS
SBAS;QZSS`
```

## Usage
1. Install dependencies: `sudo apt install libspdlog-dev nlohmann-json3-dev ninja-build`.
2. Add support for any desired messages in `include/ublox/packet/`.
3. Implement your desired configuration and interaction example (such as in `src/Driver.cpp`).
4. Build and run:
    ```
    mkdir -p build/ && cd build/ && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && ninja && cd ../ && ./build/ublox config.json
    ```

## Disclaimer
For actual fully-functional and maintained drivers, you should look into these projects instead:
* https://github.com/ublox-rs/ublox
* https://github.com/KumarRobotics/ublox