#ifndef MessageBuilder_hpp
#define MessageBuilder_hpp

#include <cstdint>
#include <vector>
#include <array>
#include <functional>
#include "MidiTypes.hpp"

class MessageBuilder {
public:
    MessageBuilder();
    std::vector<midi_message> buildMessages(int index, uint8_t value);
private:
    std::vector<midi_message> buildCC(uint8_t cc, uint8_t value);
    std::vector<midi_message> buildNRPN(uint8_t lsb, uint8_t value);
    std::vector<midi_message> buildPitchBend(uint8_t value);

    std::vector<std::function<std::vector<midi_message>(uint8_t)>> builders;
};

#endif
