#ifndef paramMapper_hpp
#define paramMapper_hpp

#include <cstdint>
#include "MidiTypes.hpp"

class ParamMapper {
public:
    uint8_t getCC(int index);
};

#endif /* paramMapper_hpp */
