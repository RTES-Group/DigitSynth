//
//  ModeManager.hpp
//  digitSynth
//
//  Created by Finn McConville on 30/03/2026.
//

#ifndef ModeManager_hpp
#define ModeManager_hpp

#include <stdio.h>
#include <MidiTypes.h>

class ModeManager {
public:
    ControlMode getCurrentMode();
    ControlMode getPreviousMode();
    void updateMode(int index);
private:
    ControlMode current_controlMode;
    ControlMode previous_controlMode;
};

#endif /* ModeManager_hpp */
