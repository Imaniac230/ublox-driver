#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <cstring>
#include <vector>
#include <iomanip>

namespace Utils {
    template<typename IntType>
    [[nodiscard]] inline static std::vector<uint8_t> serializeLEInt(const IntType &value) {
        std::vector<uint8_t> data{};
        for (size_t nBytes = 0; nBytes <= (sizeof(IntType) - 1); ++nBytes) {
            data.push_back((value >> nBytes * 8) & 0xFF);
        }

        return data;
    }

    template<typename IntType>
    [[nodiscard]] inline static std::vector<uint8_t> serializeBEInt(const IntType &value) {
        std::vector<uint8_t> data{};
        for (int nBytes = (sizeof(IntType) - 1); nBytes >= 0; --nBytes) {
            data.push_back((value >> nBytes * 8) & 0xFF);
        }

        return data;
    }

    template<typename FloatType>
    [[nodiscard]] inline static std::vector<uint8_t> serializeLEFloat(const FloatType &value) {
        uint64_t shadow = 0;
        std::memcpy(&shadow, &value, sizeof(FloatType));

        std::vector<uint8_t> data{};
        switch (sizeof(FloatType)) {
            case sizeof(float):
                data = serializeLEInt<uint32_t>(shadow);
                break;
            case sizeof(double):
                data = serializeLEInt<uint64_t>(shadow);
                break;
            default:
                throw std::runtime_error("Attempting to serialize an unsupported floating-point type.");
        }

        return data;
    }

    template<typename FloatType>
    [[nodiscard]] inline static std::vector<uint8_t> serializeBEFloat(const FloatType &value) {
        uint64_t shadow = 0;
        std::memcpy(&shadow, &value, sizeof(FloatType));

        std::vector<uint8_t> data{};
        switch (sizeof(FloatType)) {
            case sizeof(float):
                data = serializeBEInt<uint32_t>(shadow);
                break;
            case sizeof(double):
                data = serializeBEInt<uint64_t>(shadow);
                break;
            default:
                throw std::runtime_error("Attempting to serialize an unsupported floating-point type.");
        }

        return data;
    }

    template<typename IntType>
    [[nodiscard]] inline static IntType deserializeLEInt(const uint8_t data[]) {
        IntType r = 0;
        for (size_t nBytes = 0; nBytes <= (sizeof(IntType) - 1); ++nBytes) {
            r |= static_cast<IntType>(data[nBytes]) << nBytes * 8;
        }

        return r;
    }

    template<typename IntType>
    [[nodiscard]] inline static IntType deserializeBEInt(const uint8_t data[]) {
        const size_t maxBytes = sizeof(IntType) - 1;
        IntType r = 0;
        for (size_t nBytes = 0; nBytes <= maxBytes; ++nBytes) {
            r |= static_cast<IntType>(data[maxBytes - nBytes]) << nBytes * 8;
        }

        return r;
    }

    template<typename FloatType>
    [[nodiscard]] inline static FloatType deserializeLEFloat(const uint8_t data[]) {
        uint64_t shadow = 0;

        switch (sizeof(FloatType)) {
            case sizeof(float):
                shadow = deserializeLEInt<uint32_t>(data);
                break;
            case sizeof(double):
                shadow = deserializeLEInt<uint64_t>(data);
                break;
            default:
                throw std::runtime_error("Attempting to deserialize an unsupported floating-point type.");
        }

        FloatType r = 0;
        std::memcpy(&r, &shadow, sizeof(FloatType));
        return r;
    }

    template<typename FloatType>
    [[nodiscard]] inline static FloatType deserializeBEFloat(const uint8_t data[]) {
        uint64_t shadow = 0;

        switch (sizeof(FloatType)) {
            case sizeof(float):
                shadow = deserializeBEInt<uint32_t>(data);
                break;
            case sizeof(double):
                shadow = deserializeBEInt<uint64_t>(data);
                break;
            default:
                throw std::runtime_error("Attempting to deserialize an unsupported floating-point type.");
        }

        FloatType r = 0;
        std::memcpy(&r, &shadow, sizeof(FloatType));
        return r;
    }
}// namespace Utils

#endif//UTILS_H