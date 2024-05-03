#include <cstring>
#include <iostream>

#include <util/Serial.hpp>

Serial::Serial(const std::string &path, const BaudRate baud, const int minBytes) {
    if ((fileDescriptor = open(path.c_str(), O_RDWR | O_NOCTTY)) < 0)
        throw std::runtime_error{"ERROR -> Failed to open serial port '" + path + "' (errno: " + std::to_string(errno) +
                                 " -> " + strerror(errno) + ")."};

    termios options{};
    memset(&options, 0, sizeof(options));
    if (tcgetattr(fileDescriptor, &options) != 0)
        throw std::runtime_error{"ERROR -> Failed to get serial port options for '" + path +
                                 "' (errno: " + std::to_string(errno) + " -> " + strerror(errno) + ")."};

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CREAD | CLOCAL;

    options.c_lflag &= ~ICANON;
    options.c_lflag &= ~ISIG;
    options.c_lflag &= ~(ECHO | ECHOE | ECHONL);

    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    options.c_oflag &= ~OPOST;
    options.c_oflag &= ~ONLCR;

    options.c_cc[VMIN] = static_cast<cc_t>(minBytes);
    options.c_cc[VTIME] = 1;

    cfsetispeed(&options, static_cast<speed_t>(baud));
    cfsetospeed(&options, static_cast<speed_t>(baud));

    if (tcsetattr(fileDescriptor, TCSANOW, &options) != 0)
        throw std::runtime_error{"ERROR -> Failed to set serial port options for '" + path +
                                 "' (errno: " + std::to_string(errno) + " -> " + strerror(errno) + ")."};

    // flush buffer
    tcflush(fileDescriptor, TCIFLUSH);
}

void Serial::setNonBlocking(const bool nonBlocking) const {
    auto flags = ::fcntl(fileDescriptor, F_GETFL);
    if (nonBlocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }
    ::fcntl(fileDescriptor, F_SETFL, flags);
}
