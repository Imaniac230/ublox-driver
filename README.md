# Ublox minimal driver

This is a minimal functional interfacing example for experimenting with `ublox` GNSS receiver devices using the `UBX`
protocol.

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

## Dependencies

* Install build dependencies: `sudo apt install ninja-build`.

### External libraries

#### Install

* Install libraries from release packages: `sudo apt install libspdlog-dev nlohmann-json3-dev`,

#### Build from source (required for cross-compilation)

1. Install the vcs tool:
   ```bash
   curl -s https://packagecloud.io/install/repositories/dirk-thomas/vcstool/script.deb.sh | sudo bash
   sudo apt update && sudo apt install python3-vcstool
   ```
2. Import libraries to project for compilation: `vcs import ./ < libs.repos`

## Compilation

1. Add support for any desired messages in `include/ublox/packet/`.
2. Implement your desired configuration and interaction example (such as in `src/Driver.cpp`).
3. Build and run:
    ```
    mkdir -p build/ && cd build/ && cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && ninja && cd ../ && ./build/ublox config.json
    ```
   > NOTE: Release build is currently broken, compile with `-DCMAKE_BUILD_TYPE=Debug` instead.
4. To build the libraries from source on the native machine add option `-DBUILD_LIBS=YES` to the `cmake` command.
5. To cross-compile for `aarch64-linux-gnu`:
   1. Install the target toolchain: `sudo apt install binutils-aarch64-linux-gnu g++-aarch64-linux-gnu`.
   2. Add option `-DCROSS_COMPILE=YES` to the `cmake` command (this will also automatically set `-DBUILD_LIBS=YES`).
   3. To cross-compile for a different architecture you will have to create a new toolchain file in `cmake/` and modify
      the `CMakeLists.txt` to use that file.

## Disclaimer

For actual fully-functional and maintained drivers, you should look into these projects instead:

* https://github.com/ublox-rs/ublox
* https://github.com/KumarRobotics/ublox