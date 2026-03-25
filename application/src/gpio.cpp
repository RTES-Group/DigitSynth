#include "gpio.h"
#include <cstring>
#include <gpiod.hpp>
#include <iostream>
#include <thread>

static gpiod::chip *chip;

static std::thread t; 

void gpio::setupGpio() {
    chip = new gpiod::chip("/dev/gpiochip0");
    if (chip == nullptr) {
        std::cerr << "Could not open GPIO device\n";
        exit(-1);
    }
}

void gpio::setPin(int pin, bool value) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth output")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
    request.set_value(pin, gpiod::line::value(value));
}

bool gpio::getPin(int pin) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth input")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
    bool value = (bool) request.get_value(pin);
    
    return value;
}

void gpio::registerCallback(int pin, gpiod::line::edge edge, GpioCallback callback) {
    if (chip == nullptr) {
        std::cerr << "GPIO chip not set up\n";
        exit(-1);
    }   
    
    gpiod::line_config line_config; 
    line_config.add_line_settings(pin, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT).set_edge_detection(edge));
    auto builder = chip->prepare_request();
    builder
        .set_consumer("digitsynth callback")
        .set_line_config(line_config);
    
    auto request = gpiod::line_request(builder.do_request());
}
