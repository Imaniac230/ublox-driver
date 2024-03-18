# general
cmake_minimum_required(VERSION 3.5)
project(Ublox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -ldl")

find_package(spdlog)

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
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

add_library(${utils_lib_name} STATIC ${utils_sources})
target_include_directories(${utils_lib_name} PUBLIC include)
target_link_libraries(${utils_lib_name} ${external_libs})

add_library(${core_lib_name} STATIC ${core_sources})
target_include_directories(${core_lib_name} PUBLIC include)
target_link_libraries(${core_lib_name} ${utils_lib_name})

add_executable(${exec_name} src/main.cpp src/Driver.cpp)
target_link_libraries(${exec_name} ${core_lib_name})

# development
if (BUILD_TESTS)
    enable_testing()

    add_subdirectory(test)
endif ()
