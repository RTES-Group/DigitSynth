#include "SynthController.hpp"

void SynthController::onButtonEvent(int index){
    if (modeManager.current_controlMode == CHORD){
        int previousChord = chordManager.getCurrentChord();
        int currentChord = chordManager.updateChord(index);
        if (currentChord != previousChord){ // i.e. we are switching to a new chord
            for (int i = 0; i < 4; i++){
                midi_message msg;
                msg.status = 0x90; // Note On message
                msg.data_1 = chordManager.getNote(i);
                msg.data_2 = 127;
                //midiDriver.noteOnCallback(msg);
            }
        }
        else{ //same chord -> do nothing
            return;
        }
    }
    else {
        modeManager.updateMode(index);
    }
}

void SynthController::onFlexEvent(int index, float value){
    
}

void SynthController::onAllButtonsPressed(){
    
}
