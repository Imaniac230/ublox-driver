#include <array>
#include <list>

#include <gtest/gtest.h>

#include <util/RingBuffer.hpp>


struct TestType {
    uint8_t value1 = 0;
    uint16_t value2 = 0;

    TestType &operator++() {
        ++value1;
        ++value2;
        return *this;
    }
};

TEST(RIngBufferTests, basicWorks) {
    //TODO(tests): finish implementing with better though out tests
    static constexpr uint8_t BUFFER_SIZE = 10;
    //    RingBuffer<TestType, std::array<TestType, BUFFER_SIZE>> buffer{};
    RingBuffer<TestType, std::vector<TestType>> buffer{{TestType{}, TestType{}, TestType{}, TestType{}, TestType{},
                                                        TestType{}, TestType{}, TestType{}, TestType{}, TestType{}}};

    // Initial buffer should be empty
    ASSERT_TRUE(buffer.empty());
    ASSERT_FALSE(buffer.full());
    ASSERT_EQ(buffer.capacity(), buffer.size());
    ASSERT_EQ(buffer.getActiveElements(), 0);
    ASSERT_FALSE(buffer.tailOffset(1).has_value());

    // Reading or writing empty iterables should fail explicitly
    ASSERT_FALSE(buffer.read<std::list<TestType>>(0).has_value());
    ASSERT_FALSE(buffer.write(std::vector<TestType>{}));

    // Buffer state should not change after failed read/write attempts
    ASSERT_FALSE(buffer.read<std::list<TestType>>(1).has_value());
    ASSERT_TRUE(buffer.empty());
    ASSERT_FALSE(buffer.full());
    ASSERT_EQ(buffer.capacity(), buffer.size());
    ASSERT_EQ(buffer.getActiveElements(), 0);
    ASSERT_FALSE(buffer.tailOffset(1).has_value());


    ASSERT_FALSE(buffer.write(std::array<TestType, BUFFER_SIZE + 1>{}));
    TestType initialValue{};
    ASSERT_TRUE(buffer.write(std::vector<TestType>{initialValue, ++initialValue, ++initialValue, ++initialValue}));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}