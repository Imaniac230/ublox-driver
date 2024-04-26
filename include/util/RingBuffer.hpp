#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <cstdint>
#include <optional>

#include <util/Traits.h>

template<typename T, Traits::IterableWith<T> Data>
class RingBuffer {
public:
    RingBuffer() = default;
    explicit RingBuffer(Data data) : buffer(std::move(data)) {}
    RingBuffer(RingBuffer &&other) noexcept
        : buffer(std::move(other.buffer)), activeElements(other.activeElements), readIndex(other.readIndex),
          writeIndex(other.writeIndex) {}

    [[nodiscard]] uint16_t capacity() const { return buffer.size() - activeElements; }
    [[nodiscard]] uint16_t getActiveElements() const { return activeElements; }
    [[nodiscard]] bool full() const { return activeElements == buffer.size(); }
    [[nodiscard]] bool empty() const { return activeElements == 0; }

    template<Traits::IterableWith<T> InputData>
    [[nodiscard]] inline bool write(InputData data) {
        if (data.size() > capacity()) return false;

        const uint16_t overflowLimit = buffer.size() - writeIndex;
        if (data.size() <= overflowLimit) {
            std::move(data.begin(), data.end(), buffer.begin() + writeIndex);
        } else {
            std::move(data.begin(), data.begin() + overflowLimit, buffer.begin() + writeIndex);
            std::move(data.begin() + overflowLimit, data.end(), buffer.begin());
        }

        writeIndex = (writeIndex + data.size()) % buffer.size();
        activeElements += data.size();
        return true;
    }
    //FIXME: incompatible with static arrays for now
    template<Traits::IterableWith<T> OutputSlice>
    [[nodiscard]] inline std::optional<OutputSlice> read(uint16_t range) {
        if (range > activeElements) return std::nullopt;

        OutputSlice data{};
        if (data.size() != range) data.resize(range);
        const uint16_t overflowLimit = buffer.size() - readIndex;
        //TODO: decide if it is really safe to move out of the buffer here
        if (range <= overflowLimit) {
            std::move(buffer.begin() + readIndex, buffer.begin() + readIndex + range, data.begin());
            //            data.insert(data.end(), buffer.begin() + readIndex, buffer.begin() + readIndex + range);
        } else {
            std::move(buffer.begin() + readIndex, buffer.begin() + readIndex + overflowLimit, data.begin());
            std::move(buffer.begin(), buffer.begin() + range - overflowLimit, data.begin());
            //            data.insert(data.end(), buffer.begin() + readIndex, buffer.begin() + readIndex + overflowLimit);
            //            data.insert(data.end(), buffer.begin(), buffer.begin() + range - overflowLimit);
        }

        readIndex = (readIndex + range) % buffer.size();
        activeElements -= range;
        return data;
    }
    [[nodiscard]] std::optional<T> tailOffset(uint16_t offset = 0) const {
        if (offset > activeElements) return std::nullopt;

        return buffer[(readIndex + offset) % buffer.size()];
    }

private:
    Data buffer;

    uint16_t writeIndex = 0, readIndex = 0, activeElements = 0;
};

#endif//RINGBUFFER_H