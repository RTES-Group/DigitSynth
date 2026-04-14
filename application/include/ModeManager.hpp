#ifndef ModeManager_hpp
#define ModeManager_hpp

#include <stdio.h>
#include "MidiTypes.hpp"

class ModeManager {
public:
    ControlMode getCurrentMode();
    ControlMode getPreviousMode();
    ControlMode getActiveLedPattern();
    void updateMode(int index);
private:
    ControlMode current_controlMode = EQ;
    ControlMode previous_controlMode = EQ;
    ControlMode active_led_pattern = EQ;
};

#endif /* ModeManager_hpp */
