#include "paramMapper.hpp"

uint8_t ParamMapper::getCC(int index){ // for flex sensors
    switch(index){
        case 0: // filter cutoff
            return 74;
            break;
        case 1: // LFO rate
            return 16;
            break;
        case 2: // LFO depth (depends on button 2 state! This needs to be stored somewhere)
            return 18; // might be wrong (this is for LFO filter depth)
            break;
        default:
            return 0;
    }
}
