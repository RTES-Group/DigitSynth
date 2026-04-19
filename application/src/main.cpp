
#include "SynthController.hpp"
#include "TLC59711.h"
#include "adc-driver.h"
#include "button-driver.h"
#include "flex-sensor.h"
#include "gpio.h"
#include "midi-driver.hpp"
#include "voltage-scaler.h"
#include <iostream>

int main() {
    gpio::setupGpio();
    
    auto bd = new button_driver::ButtonDriver(); 
    auto adc = new adc_driver::Ads1115Driver();
    auto vs = new voltage_scaler::VoltageScaler();
    auto fs = new flex_sensor::FlexSensor(static_cast<adc_driver::IAdcDriver *>(adc), static_cast<voltage_scaler::IVoltageScaler *>(vs));
    adc = nullptr;
    vs = nullptr; 
    auto md = new midi_driver::MidiDriver();
    TLC59711 tlc(17, 27);
    tlc.start();
    SynthController synth(
        tlc, 
        static_cast<button_driver::IButtonDriver *>(bd), 
        static_cast<flex_sensor::IFlexSensor *>(fs),
        static_cast<midi_driver::IMidiDriver *>(md)
    );
    bd = nullptr; 
    fs = nullptr; 
    md = nullptr;
    getchar();
    std::cout <<"stop\n";
    
    
    
    return 0;
}
