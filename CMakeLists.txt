# general
cmake_minimum_required(VERSION 3.21)

if (CROSS_COMPILE)
    # build dependencies from source if cross-compiling
    set(BUILD_LIBS YES)

    #TODO(cross-build): can this be done more elegantly with just a single file (function with triplet name argument)?
#    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/arm-linux-gnueabihf.cmake)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/aarch64-linux-gnu.cmake)
endif ()

project(Ublox)

#set(BUILD_TESTS 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ldl -fconcepts")

if (BUILD_LIBS)
    add_subdirectory(libs)
else ()
    find_package(spdlog REQUIRED)
    find_package(nlohmann_json REQUIRED)
endif ()

set(core_lib_name libublox)
set(utils_lib_name libutils)
set(exec_name ublox)

set(core_sources
        src/ublox/Device.cpp
)

set(utils_sources
        src/util/Serial.cpp
)

set(external_libs
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(${utils_lib_name} STATIC ${utils_sources})
target_include_directories(${utils_lib_name} PUBLIC include)
target_link_libraries(${utils_lib_name} ${external_libs})

add_library(${core_lib_name} STATIC ${core_sources})
target_include_directories(${core_lib_name} PUBLIC include)
target_link_libraries(${core_lib_name} ${utils_lib_name})

# example driver application
add_executable(${exec_name} src/main.cpp src/Driver.cpp src/DriverHelpers.cpp)
target_link_libraries(${exec_name} ${core_lib_name})

# development
if (BUILD_TESTS)
    enable_testing()

    add_subdirectory(test)
endif ()
