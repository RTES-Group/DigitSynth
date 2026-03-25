#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <thread>
#include "adc-driver.h"
#include "foo.h"
#include "gpio.h"
#include "spi.h"


int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
    
    gpio::setupGpio();
    
    while (1) {
        gpio::setPin("PIN_22", 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        gpio::setPin("PIN_22", 1);
    }
    
    return 0;
}
