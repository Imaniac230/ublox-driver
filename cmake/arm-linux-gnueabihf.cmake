set(CMAKE_HOST_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_NAME ${CMAKE_HOST_SYSTEM_NAME})
set(CMAKE_SYSTEM_PROCESSOR arm)

# optional
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH /usr/lib/arm-linux-gnueabihf/)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY_CMAKE_FIND_ROOT_PATH)
set(CMAKE_PREFIX_PATH /usr/lib/arm-linux-gnueabihf/)
set(CMAKE_SYSROOT /usr/lib/arm-linux-gnueabihf/)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
