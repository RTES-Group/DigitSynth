#pragma once
#include <cstdint>
#include <vector>
#include <mutex>
#include "types.h"



struct LEDFrame {
    Brightness channels[10];
};

class TLC59711 {
    friend class TLC59711Test;
public:
    TLC59711(int data_pin, int clk_pin, int num_drivers = 1);
    ~TLC59711();

    // Setter — converts frame to PWM and shifts out immediately.
    // Safe to call from any single thread.
    void update(const LEDFrame& frame);

    void setBrightness(uint8_t brightness);

private:
    void buildPacket(std::vector<uint8_t>& buf) const;
    void shiftOut(const std::vector<uint8_t>& buf) const;
    static uint16_t toGS(Brightness b);

    static constexpr int FRAME_TO_GS[10] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9
    };

    static constexpr int  CHANNELS_PER_DRIVER = 12;
    static constexpr int  NUM_LEDS            = 10;
    static constexpr long HALF_PERIOD_NS      = 5000L;

    int _gpio_handle{-1};
    int _data_pin{0};
    int _clk_pin{0};
    int _num_drivers{1};

    uint8_t _brightness{127};

    std::vector<uint16_t> _pwm;
};