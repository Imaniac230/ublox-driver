#ifndef SERIAL_H
#define SERIAL_H

#include <fcntl.h>
#include <string>
#include <termios.h>
#include <unistd.h>

class Serial {
public:
    enum class BaudRate : speed_t {
        Baud9600 = B9600,
        Baud19200 = B19200,
        Baud38400 = B38400,
        Baud57600 = B57600,
        Baud115200 = B115200,
        Baud230400 = B230400,
        Baud460800 = B460800,
        Baud921600 = B921600,
    };

    Serial(const std::string &path, BaudRate baud, int minBytes);
    //FIXME(dtor): can't have the destructor implemented as it would break assignments
    //    virtual ~Serial() { close(); }

    [[nodiscard]] inline ssize_t readBytes(unsigned bytesToRead, char *buffer) const {
        return read(fileDescriptor, buffer, bytesToRead);
    }
    [[nodiscard]] inline ssize_t writeBytes(const uint8_t *buffer, uint8_t bufferLength) const {
        return write(fileDescriptor, buffer, bufferLength);
    }

    void setNonBlocking(bool nonBlocking) const;
    void close() {
        if (fileDescriptor != -1) {
            //TODO: handle errors?
            ::close(fileDescriptor);
            fileDescriptor = -1;
        }
    }

    [[nodiscard]] int getFileDescriptor() const { return fileDescriptor; }

private:
    int fileDescriptor = -1;
};

inline static constexpr uint32_t toRaw(const Serial::BaudRate baudRate) {
    switch (baudRate) {
        case Serial::BaudRate::Baud9600:
            return 9600;
        case Serial::BaudRate::Baud19200:
            return 19200;
        case Serial::BaudRate::Baud38400:
            return 38400;
        case Serial::BaudRate::Baud57600:
            return 57600;
        case Serial::BaudRate::Baud115200:
            return 115200;
        case Serial::BaudRate::Baud230400:
            return 230400;
        case Serial::BaudRate::Baud460800:
            return 460800;
        case Serial::BaudRate::Baud921600:
            return 921600;
        default:
            throw std::runtime_error{"Trying to convert an unsupported baud rate."};
    }
}

#endif//SERIAL_H
