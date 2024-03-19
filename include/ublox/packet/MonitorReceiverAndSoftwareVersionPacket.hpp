#ifndef MONITORPORTINFORMATIONPACKET_H
#define MONITORPORTINFORMATIONPACKET_H

#include <ublox/packet/Packet.hpp>

namespace UBLOX::Packet {
    class MonitorReceiverAndSoftwareVersion : public Base {
    public:
        static constexpr Message MESSAGE = Message::MonReceiverAndSoftwareVersion;
        struct Data {
            std::array<uint8_t, 30> swVersion{};
            std::array<uint8_t, 10> hwVersion{};
            std::vector<std::array<uint8_t, 30>> extensions{};
        };

        explicit MonitorReceiverAndSoftwareVersion(Base &&raw) : Base(std::move(raw)) {}

        [[nodiscard]] inline bool toData() {
            if (message() != MESSAGE) return false;

            const std::vector<uint8_t> raw = rawData();
            data = Data{};
            std::move(raw.begin(), raw.begin() + 30, data.swVersion.begin());
            std::move(raw.begin() + 30, raw.begin() + 30 + 10, data.hwVersion.begin());
            int dataStart = 30 + 10;
            while (dataStart < dataSize()) {
                std::array<uint8_t, 30> extension{};
                std::move(raw.begin() + dataStart, raw.begin() + dataStart + 30, extension.begin());
                data.extensions.emplace_back(extension);
                dataStart += 30;
            }
            return true;
        }
        [[nodiscard]] inline const Data &getData() const { return data; }

    private:
        Data data{};
    };
}// namespace UBLOX::Packet

#endif//MONITORPORTINFORMATIONPACKET_H
