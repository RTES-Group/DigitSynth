#ifndef SynthController_hpp
#define SynthController_hpp

#include "ModeManager.hpp"
#include "paramMapper.hpp"
#include "MidiScaler.hpp"
#include "ChordManager.hpp"

class SynthController {
public:
    void onButtonEvent(int index); //callback for a single button press
    void onFlexEvent(int index, float value); //callback for a flex sensor update
    void onAllButtonsPressed(); //callback for every button pressed at same time
private:
    ModeManager modeManager;
    ChordManager chordManager;
    ParamMapper paramMapper;
    MidiScaler midiScaler;
};

#endif /* SynthController_hpp */
