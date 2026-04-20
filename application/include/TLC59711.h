#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "ILedDriver.hpp"

namespace led_driver {

/**
 * TLC59711 driver using the Linux spidev interface (hardware SPI).
 **/
class TLC59711 : public ILedDriver {
    friend class TLC59711Test;
public:
    explicit TLC59711(std::string spi_device = "/dev/spidev0.0",
                      uint32_t    spi_speed_hz = 1'000'000,
                      int         num_drivers = 1);
    ~TLC59711();

    void start();
    void stop();

    // Non-blocking — copies channels into shared buffer and wakes worker thread.
    void update(const Channels& channels);

    void setBrightness(uint8_t brightness);

private:
    // Blocking I/O loop — woken by update(), owns the spidev fd.
    void worker();

    void buildPacket(std::vector<uint8_t>& buf) const;
    void shiftOut(const std::vector<uint8_t>& buf) const;
    static uint16_t toGS(Brightness b);

    static constexpr int FRAME_TO_GS[NUM_LEDS] = {
        0, 1, 2,
        3, 4, 5,
        6, 7
    };

    static constexpr int CHANNELS_PER_DRIVER = 12;

    std::thread              _thread;
    std::atomic<bool>        _running{false};

    // Shared state between calling thread and worker thread.
    std::mutex               _mutex;
    std::condition_variable  _cv;
    Channels                 _pending{};
    bool                     _dirty{false};

    std::string              _spi_device;
    uint32_t                 _spi_speed_hz{1'000'000};
    int                      _num_drivers{1};
    int                      _spi_fd{-1};   // opened in start(), closed in stop()

    std::atomic<uint8_t>     _brightness{127};

    // Owned exclusively by worker thread — no mutex needed.
    std::vector<uint16_t>    _pwm;
};

}