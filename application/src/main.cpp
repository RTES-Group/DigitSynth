
#include "SynthController.hpp"
#include "TLC59711.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include "patterns.h"
#include "types.h"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    TLC59711 tlc(17, 27);
    FlexSensor fx;
    fx.begin();
    tlc.start();
    SynthController synth(tlc, fx);
    
    getchar();
    
    std::cout <<"stop\n";
    
    
    
    return 0;
}
