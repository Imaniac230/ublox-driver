#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <cstdint>
#include <optional>

#include <util/Traits.h>

//FIXME(buffer-traits): incompatible with random-access lists or empty iterables for now
template<typename T, Traits::IterableWith<T> Data>
class RingBuffer {
public:
    RingBuffer() = default;
    explicit RingBuffer(Data data) : buffer(std::move(data)) {}
    RingBuffer(RingBuffer &&other) noexcept
        : buffer(std::move(other.buffer)), activeElements(other.activeElements), readIndex(other.readIndex),
          writeIndex(other.writeIndex) {}

    [[nodiscard]] uint16_t size() const { return buffer.size(); }
    [[nodiscard]] uint16_t capacity() const { return size() - activeElements; }
    [[nodiscard]] uint16_t getActiveElements() const { return activeElements; }
    [[nodiscard]] bool full() const { return activeElements == size(); }
    [[nodiscard]] bool empty() const { return activeElements == 0; }

    //FIXME(buffer-traits): incompatible with random-access lists for now
    template<Traits::IterableWith<T> InputData>
    [[nodiscard]] inline bool write(InputData data) {
        if (data.size() > capacity()) return false;
        if (data.size() == 0) return false;

        const uint16_t overflowLimit = size() - writeIndex;
        if (data.size() <= overflowLimit) {
            std::move(data.begin(), data.end(), buffer.begin() + writeIndex);
        } else {
            std::move(data.begin(), data.begin() + overflowLimit, buffer.begin() + writeIndex);
            std::move(data.begin() + overflowLimit, data.end(), buffer.begin());
        }

        writeIndex = (writeIndex + data.size()) % size();
        activeElements += data.size();
        return true;
    }
    //FIXME(buffer-traits): incompatible with static arrays for now
    template<Traits::IterableWith<T> OutputSlice>
    [[nodiscard]] inline std::optional<OutputSlice> read(uint16_t range) {
        if (range > activeElements) return std::nullopt;
        if (range == 0) return std::nullopt;

        OutputSlice data{};
        if (data.size() != range) data.resize(range);
        const uint16_t overflowLimit = size() - readIndex;
        if (range <= overflowLimit) {
            std::move(buffer.begin() + readIndex, buffer.begin() + readIndex + range, data.begin());
        } else {
            std::move(buffer.begin() + readIndex, buffer.begin() + readIndex + overflowLimit, data.begin());
            std::move(buffer.begin(), buffer.begin() + range - overflowLimit, data.begin());
        }

        readIndex = (readIndex + range) % size();
        activeElements -= range;
        return data;
    }
    [[nodiscard]] std::optional<T> tailOffset(uint16_t offset = 0) const {
        if (offset > activeElements) return std::nullopt;

        return buffer[(readIndex + offset) % size()];
    }

private:
    Data buffer;

    uint16_t writeIndex = 0, readIndex = 0, activeElements = 0;
};

#endif//RINGBUFFER_H