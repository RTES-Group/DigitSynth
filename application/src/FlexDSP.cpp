#include "FlexDSP.hpp"

FlexDSP::FlexDSP(float sampleRate, float cutoffHz) {
    // Butterworth expects normalised frequency: cutoff / sampleRate
    // must be between 0 and 0.5 (i.e. cutoffHz must be less than sampleRate/2)
    double normalisedCutoff = cutoffHz / sampleRate;
    for (auto& f : filters) {
        f.setup(2, normalisedCutoff);
    }
}

void FlexDSP::onFlexData(std::array<ExtensionData, 4> rawData) {
    if (!callback.has_value()) { return; }

   std::array<ExtensionData, 4> filtered;
    for (int i = 0; i < 4; i++) {
        filtered[i] = (float)filters[i].filter(rawData[i]);
    }
    callback.value()(filtered);
}

// config

void FlexDSP::registerCallback(FilteredCallback cb) {
    callback = cb;
}

void FlexDSP::reset() {
    for (auto& f : filters) {
        f.reset();
    }
}
