<<<<<<< Updated upstream
=======
#include <array>
#include <chrono>
>>>>>>> Stashed changes
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <gpiod.hpp>
#include <iostream>
#include "button-driver.h"
#include "flex-sensor.h"
#include "foo.h"
#include "gpio.h"
#include "types.h"
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ads1115rpi.h>

void callback(float f) {
    std::cout << f << std::endl;
    fflush(stdout);
} 

/**
 * @returns woohoo
 */
int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 

<<<<<<< Updated upstream
=======
    ButtonDriver bd;
    TLC59711 leds(17, 27);
    FlexSensor flexSensor;
    // TLC59711::Channels channels;
    
    /* 
    flexSensor.registerCallback([&channels, &leds] (std::array<ExtensionData, 4> data) {
        channels[0] = data[0];
        leds.update(channels);
    });
    */
    
    auto start = std::chrono::high_resolution_clock::now();
    flexSensor.registerCallback([start] (std::array<ExtensionData, 4> data) {
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << ",";
        for (int i = 0; i < 4; i++)
            printf("%.10f,", data[i]);
        std::cout << std::endl;
    });   
    
    flexSensor.begin();
    getchar();
>>>>>>> Stashed changes
    
    return 0;
}