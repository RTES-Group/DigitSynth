#include "gpio.h"
#include <cstring>
#include <gpiod.h>
#include <iostream>

static gpiod_chip *chip;

void gpio::setupGpio() {
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (chip == nullptr) {
        std::cerr << "Could not open GPIO device\n";
        exit(-1);
    }
}

void gpio::setPin(const char *pin, bool value) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }
    
    auto line = gpiod_chip_find_line(chip, pin);
    gpiod_line_request_output(line, "digitsynth output", 0);
    gpiod_line_set_value(line, value);
    gpiod_line_release(line);
}

bool gpio::getPin(const char *pin) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    auto line = gpiod_chip_find_line(chip, pin);
    gpiod_line_request_input(line, "digitsynth input");
    bool value = gpiod_line_get_value(line);
    gpiod_line_release(line);
    
    return value;
}
