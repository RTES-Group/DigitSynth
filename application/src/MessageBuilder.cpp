#include "MessageBuilder.hpp"

MessageBuilder::MessageBuilder() {
    builders[0] = [this](uint8_t v){ return buildCC(74, v); };
    builders[1] = [this](uint8_t v){ return buildNRPN(6, v); };
    builders[2] = [this](uint8_t v){ return buildNRPN(18, v); };
    builders[3] = [this](uint8_t v){ return buildPitchBend(v); };
}

std::vector<midi_message> MessageBuilder::buildMessages(int index, uint8_t value) {
    if (index < 0 || index >= static_cast<int>(builders.size())) return {};
    return builders[index](value);
}

std::vector<midi_message> MessageBuilder::buildCC(uint8_t cc, uint8_t value) {
    return {{ 0xB0, cc, value }};
}

std::vector<midi_message> MessageBuilder::buildNRPN(uint8_t lsb, uint8_t value) {
    return {
        { 0xB0, 99, 0    },
        { 0xB0, 98, lsb  },
        { 0xB0,  6, value},
        { 0xB0, 38, 0    },
    };
}

std::vector<midi_message> MessageBuilder::buildPitchBend(uint8_t value) {
    return {{ 0xE0, 0, value }};
}
