
#include "SynthController.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    TLC59711 tlc(17, 27);
    SynthController synth(tlc);
    MidiDriver md; 
    
    
    getchar();
    
    
    
    
    return 0;
}
