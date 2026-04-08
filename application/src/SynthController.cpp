#include "SynthController.hpp"

SynthController::SynthController(TLC59711& tlc)
    : _tlc(tlc)
{

}

void SynthController::onButtonEvent(int index){
    if (modeManager.getCurrentMode() == CHORD){
        int previousChord = chordManager.getCurrentChord();
        chordManager.updateChord(index);
        int currentChord = chordManager.getCurrentChord();
        if (currentChord != previousChord){ // i.e. we are switching to a new chord
            for (int i = 0; i < 4; i++){
		    /*
                midi_message msg;
                msg.status = 0x90; // Note On message
                msg.data_1 = chordManager.getNote(i);
                msg.data_2 = 127;
		*/
                //midiDriver.noteOnCallback(msg);
            }
        }
        else{ //same chord -> do nothing
            return;
        }
    }
    else {
        modeManager.updateMode(index);
        switch(modeManager.getCurrentMode()){
            case EQ:
                switch (modeManager.getPreviousMode()) {
                    case EQ:
                        break;
                    case SOURCE_EQ: // sourceEQ -> EQ
                        stopRipple();
                        startFade();
                        break;
                    case DETUNE: // Detune -> EQ
                        stopRipple(); // ripple for now
                        startFade();
                        break;
                    case CHORD:
                        break;
                    default:
                        break;
                }
                break;
            case SOURCE_EQ:
                switch (modeManager.getPreviousMode()) {
                    case EQ: // EQ -> sourceEQ
                        stopFade();
                        startRipple();
                        break;
                    case SOURCE_EQ:
                        break;
                    case DETUNE:
                        stopRipple(); // ripple placeholder
                        startRipple();
                        break;
                    case CHORD:
                        break;
                    default:
                        break;
                }
                break;
            case DETUNE:
                switch (modeManager.getPreviousMode()) {
                    case EQ:
                        stopFade();
                        startRipple(); // placeholder for now
                        break;
                    case SOURCE_EQ: // sourceEQ -> Detune
                        stopRipple();
                        startRipple();
                        break;
                    case DETUNE:
                        break;
                    case CHORD:
                        break;
                    default:
                        break;
                }
                break;
            case CHORD:
                break;
        }
    }
}

void SynthController::onFlexEvent(int index, float value){
    uint8_t scaled_value = midiScaler.scaleValue(value);
    ControlMode currentMode = modeManager.getCurrentMode();
    ControlMode prevMode = modeManager.getPreviousMode();
    uint8_t cc_number;
    if (currentMode == CHORD){
        cc_number = paramMapper.getCC(index, prevMode);
    }
    else {
        cc_number = paramMapper.getCC(index, currentMode);
    }
    midi_message msg;
    msg.status = 0xB0; // Control Change
    msg.data_1 = cc_number;
    msg.data_2 = scaled_value;
    lastCC = msg; // for testing
    //midiDriver.ccCalback(msg);
}

void SynthController::onAllButtonsPressed(){
    ControlMode currentMode = modeManager.getCurrentMode();
    if (currentMode != CHORD){ // under normal operation, do nothing
        return;
    }
    else { // in chord mode: exit chord mode and return to what we were doing before
        currentMode = modeManager.getPreviousMode();
        int mode_index;
        switch(currentMode){
            case EQ:
                mode_index = 0;
                break;
            case SOURCE_EQ:
                mode_index = 1;
                break;
            case DETUNE:
                mode_index = 2;
                break;
            case CHORD:
                mode_index = 3;
                break;
        }
        modeManager.updateMode(mode_index);
    }
}

ControlMode SynthController::getCurrentMode(){
    return modeManager.getCurrentMode();
}

uint8_t SynthController::getCurrentChord(){
    return chordManager.getCurrentChord();
}

midi_message SynthController::getLastCC(){
    return lastCC;
}

void SynthController::startRipple() {
    // PatternRipple runs in its own thread (course Ch. 3) and calls
    // tlc.update() internally on each frame — SynthController just
    // starts and stops it as mode changes dictate.
    if (!_ripple) {
        _ripple = std::make_unique<PatternRipple>(_tlc);
        _ripple->start();
    }
}

void SynthController::stopRipple() {
    if (_ripple) {
        // stop() sets _running=false and joins the thread (course Ch. 3.3.3).
        _ripple->stop();
        _ripple.reset();
    }
}

void SynthController::startFade() {
    stopFade(); // temp: create a new instance every time
    if (!_fade) {
        _fade = std::make_unique<PatternFade>(_tlc);
        _fade->start();
    }
}

void SynthController::stopFade() {
    if (_fade) {
        _fade->stop();
        _fade.reset();
    }
}
