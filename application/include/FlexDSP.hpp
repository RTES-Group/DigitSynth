#pragma once
#pragma once
#include "types.h"
#include "flex-sensor.h"
#include <array>
#include <functional>
#include <optional>
#include <iir/Butterworth.h>

using FilteredCallback = std::function<void(std::array<ExtensionData, 4>)>;

class FlexDSP {
public:

    // sampleRate = how often your ADC fires in Hz (check your ADS1115 config)
    // cutoffHz   = frequency above which noise is attenuated (5Hz is a good start)
    FlexDSP(FlexSensor& sensor, float sampleRate = 108.7f, float cutoffHz = 5.0f);

    // called by SynthController 
    void registerCallback(FilteredCallback callback);

    // reset filter state 
    void reset();

private:
    // one 4th order Butterworth low pass per sensor channel
    std::array<Iir::Butterworth::LowPass<4>, 4> filters;
    std::optional<FilteredCallback> callback;

};
