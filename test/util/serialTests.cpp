#include <gtest/gtest.h>
#include <thread>

#include <util/Serial.hpp>

/**
 * These tests require a serial port to be available,
 * can be hardware,
 * or virtual: socat -d -d -L /tmp/socat0 pty,rawer,echo=0,link=/tmp/ttyV0 pty,rawer,echo=0,link=/tmp/ttyV1
 */
static constexpr auto TEST_SERIAL_0 = "/tmp/ttyV0";
static constexpr auto TEST_SERIAL_1 = "/tmp/ttyV1";

TEST(serialTests, SerialOpening) {
    const auto maybePort0 = std::getenv("TEST_SERIAL_0");

    Serial validSerial((!maybePort0) ? TEST_SERIAL_0 : maybePort0, Serial::BaudRate::Baud115200, 5);
    EXPECT_GT(validSerial.getFileDescriptor(), 0);

    bool exceptionThrown = false;
    try {
        Serial invalidSerial("invalid_port", Serial::BaudRate::Baud115200, 5);
    } catch (const std::runtime_error &e) { exceptionThrown = true; }
    ASSERT_TRUE(exceptionThrown);
}

//We're passing a message multiple times from one port to another in blocking mode and verifying its correctness.
TEST(serialTests, SendReceiveBlocking) {
    const auto maybePort0 = std::getenv("TEST_SERIAL_0");
    const auto maybePort1 = std::getenv("TEST_SERIAL_1");
    const auto msg = "Message from sender";
    const int testIterations = 50;
    int receivedMessages = 0;

    std::thread([&]() {
        Serial sender((!maybePort0) ? TEST_SERIAL_0 : maybePort0, Serial::BaudRate::Baud115200, 5);

        for (int i = 0; i < testIterations; ++i) {
            ASSERT_EQ(sender.writeBytes(reinterpret_cast<const uint8_t *>(msg), strlen(msg)), strlen(msg));
        }
    }).detach();

    Serial receiver((!maybePort1) ? TEST_SERIAL_1 : maybePort1, Serial::BaudRate::Baud115200, 5);
    receiver.setNonBlocking(false);

    for (int i = 0; i < testIterations; ++i) {
        std::vector<char> buffer(strlen(msg), 0);

        ASSERT_EQ(receiver.readBytes(strlen(msg), buffer.data()), strlen(msg));
        ASSERT_EQ(strcmp(buffer.data(), msg), 0);
        ++receivedMessages;
    }
    ASSERT_EQ(receivedMessages, testIterations);
}

//We're passing a message multiple times from one port to another in non-blocking mode and verifying its correctness.
TEST(serialTests, SendReceiveNonblocking) {
    const auto maybePort0 = std::getenv("TEST_SERIAL_0");
    const auto maybePort1 = std::getenv("TEST_SERIAL_1");
    const auto msg = "Message from sender";
    const std::chrono::milliseconds senderPeriod(2);
    const int testIterations = 50;
    int receivedMessages = 0;

    std::thread([&]() {
        Serial receiver((!maybePort1) ? TEST_SERIAL_1 : maybePort1, Serial::BaudRate::Baud115200, 5);
        receiver.setNonBlocking(true);
        std::vector<char> buffer(strlen(msg), 0);

        for (int i = 0; i < testIterations; ++i) {
            if (receiver.readBytes(strlen(msg), buffer.data()) == static_cast<int>(strlen(msg))) {
                ASSERT_EQ(strcmp(buffer.data(), msg), 0);
                ++receivedMessages;
            }
            std::this_thread::sleep_for(senderPeriod / 2);
        }
    }).detach();


    Serial sender((!maybePort0) ? TEST_SERIAL_0 : maybePort0, Serial::BaudRate::Baud115200, 5);

    for (int i = 0; i < testIterations; ++i) {
        ASSERT_EQ(sender.writeBytes(reinterpret_cast<const uint8_t *>(msg), strlen(msg)), strlen(msg));
        std::this_thread::sleep_for(senderPeriod);
    }
    ASSERT_LT(receivedMessages, testIterations);
    ASSERT_GE(receivedMessages, testIterations / 2);
}

TEST(serialTests, ReceiveMinBytes) {
    const auto maybePort0 = std::getenv("TEST_SERIAL_0");
    const auto maybePort1 = std::getenv("TEST_SERIAL_1");
    const uint8_t msg = 1;
    const int testIterations = 50;

    for (int iteration = 1; iteration <= testIterations; ++iteration) {
        //Sender will send single byte messages one by one
        std::thread([&]() {
            Serial sender((!maybePort0) ? TEST_SERIAL_0 : maybePort0, Serial::BaudRate::Baud115200, iteration);
            for (int i = 0; i < iteration; ++i) { ASSERT_EQ(sender.writeBytes(&msg, 1), 1); }
        }).detach();

        //Receiver should block until the minimum number of bytes is received in a single read
        Serial receiver((!maybePort1) ? TEST_SERIAL_1 : maybePort1, Serial::BaudRate::Baud115200, iteration);
        receiver.setNonBlocking(false);
        std::vector<char> buffer(iteration, 0);

        ASSERT_EQ(receiver.readBytes(iteration, buffer.data()), iteration);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
