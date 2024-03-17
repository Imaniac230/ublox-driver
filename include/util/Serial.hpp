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
    };

    Serial(const std::string &path, BaudRate baud, int minBytes);
    virtual ~Serial() {
        if (fileDescriptor != -1) close(fileDescriptor);
    }

    [[nodiscard]] inline size_t readBytes(unsigned bytesToRead, char *buffer) const {
        return read(fileDescriptor, buffer, bytesToRead);
    }
    [[nodiscard]] inline size_t writeBytes(const uint8_t *buffer, uint8_t bufferLength) const {
        return write(fileDescriptor, buffer, bufferLength);
    }

    void setNonBlocking(bool nonBlocking) const;

    [[nodiscard]] int getFileDescriptor() const { return fileDescriptor; }

private:
    int fileDescriptor = -1;
};


#endif//SERIAL_H
