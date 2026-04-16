#include "SynthController.hpp"
#include <iostream>

SynthController::SynthController(TLC59711& tlc)
: ledController(tlc)
{
    this->buttonDriver.registerSingleButtonCallback([this] (int index) {
        if (modeManager.getCurrentMode() == NORMAL){
            switch(index){
                case 0: // enter chord mode
                    modeManager.updateMode();
                    break;
                case 1: // enable/disable the LFO
                    lfoManager.toggle();
                    break;
                case 2: // cycle through the LFO shapes
                    lfoManager.cycleShape();
                    break;
                case 3: // turn off/on
                    
                    break;
            }
        }
        else { // chord mode
            if (index == chordManager.getCurrentChord()){
                modeManager.updateMode(); // exit chord mode, back to normal
            }
            else {
                chordManager.updateChord(index);
            }
        }
    });
    
    this->flexSensor.registerCallback([this] (std::array<ExtensionData, 4> values){
    });

    this->flexSensor.begin();
}

SynthController::~SynthController() {
    std::cout << this->flexSensor.getNSamples() << std::endl;
}

void SynthController::registerMidiCallback(MidiCallback callback){
    midiCallback = callback;
}
