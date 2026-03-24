#include "TLC59711.h"
#include <lgpio.h>
#include <stdexcept>
#include <cmath>
#include <unistd.h>
#include <cstring>
#include <iostream>

constexpr int TLC59711::FRAME_TO_GS[10];

TLC59711::TLC59711(int data_pin, int clk_pin, int num_drivers)
    : _data_pin(data_pin),
      _clk_pin(clk_pin),
      _num_drivers(num_drivers),
      _pwm(num_drivers * CHANNELS_PER_DRIVER, 0)
{
    _gpio_handle = lgGpiochipOpen(0);
    if (_gpio_handle < 0)
        throw std::runtime_error("TLC59711: cannot open gpiochip0");

    if (lgGpioClaimOutput(_gpio_handle, 0, _data_pin, 0) < 0)
        throw std::runtime_error("TLC59711: cannot claim data pin");

    if (lgGpioClaimOutput(_gpio_handle, 0, _clk_pin, 0) < 0)
        throw std::runtime_error("TLC59711: cannot claim clock pin");
}

TLC59711::~TLC59711() {
    stop();   // safe to call if never started — checks _running
    if (_gpio_handle >= 0) {
        lgGpioWrite(_gpio_handle, _data_pin, 0);
        lgGpioWrite(_gpio_handle, _clk_pin,  0);
        lgGpioFree(_gpio_handle, _data_pin);
        lgGpioFree(_gpio_handle, _clk_pin);
        lgGpiochipClose(_gpio_handle);
    }
}

void TLC59711::start() {
    if (_running) return;

    if (pipe(_pipe_fd) < 0)
        throw std::runtime_error("TLC59711: pipe() failed");

    _running = true;
    _thread  = std::thread(&TLC59711::worker, this);
}

void TLC59711::stop() {
    if (!_running) return;
    _running = false;

    if (_pipe_fd[1] >= 0) {
        close(_pipe_fd[1]);
        _pipe_fd[1] = -1;
    }

    if (_thread.joinable())
        _thread.join();

    if (_pipe_fd[0] >= 0) {
        close(_pipe_fd[0]);
        _pipe_fd[0] = -1;
    }
}

void TLC59711::update(const LEDFrame& frame) {
}

void TLC59711::setBrightness(uint8_t brightness) {
}

void TLC59711::worker() {
    LEDFrame frame{};
}

void TLC59711::buildPacket(std::vector<uint8_t>& buf) const {
}


void TLC59711::shiftOut(const std::vector<uint8_t>& buf) const {
}


uint16_t TLC59711::toGS(Brightness b) {

}