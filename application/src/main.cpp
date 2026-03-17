#include <cmath>
#include <cstring>
#include <iostream>
#include <thread>
#include <wiringPi.h>
#include "adc-driver.h"
#include "foo.h"
#include "spi.h"


int main(int argc, char **argv) {
    std::cout << "\nStarting DigitSynth application\nArguments: \n";
    
    wiringPiSetupGpio();

    for (int i = 0; i < argc; i++) {
    	std::cout << "\t" << argv[i] << "\n"; 
    }

    SpiSettings spiSettings{};
    spiSettings.bitOrder = 0;
    spiSettings.bitsPerWord = 8;
    spiSettings.clockFrequency = 1'000'000;
    spiSettings.clockPolarity = 0;
    spiSettings.clockPhase = 1; 
    Spi spi("/dev/spidev0.0", spiSettings);
    
    AdcSettings adcSettings{};
    adcSettings.analogueInputBuffer = false;
    adcSettings.autoCalibration = false;
    adcSettings.clockOut = false;
    adcSettings.clockRate = AdsClockRate::R30000;
    adcSettings.logGain = 0;
    adcSettings.lsbFirst = true;
    AdcDriver adc(&spi, adcSettings);
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    while (true) {
        std::cout << "_______________________________\n";
        for (int i = 0; i < 8; i++) {
            adc.readChannel(i, [i] (AdcData data) {
                if (data.has_value()) { std::cout << i << ": val: " << ((double) data.value() / pow(2.0, 24.0) ) << std::endl; }
                else { 
                    std::cout << i << ": no value: " << std::strerror(errno) << std::endl; 
                }
            }); 
        }
    }
    
   std::this_thread::sleep_for(std::chrono::seconds(2));
    
    
    return 0;
}
