#ifndef SPI_H_
#define SPI_H_

#include <string>
#include <vector>
#include <cstdint>

#include "types.h"

struct SpiConfig {
    uint8_t   bitsPerWord:   8,
              clockPolarity: 1, // Polarity of the clock during idle phase
              clockPhase:    1, // Data read/written on rising edge (0) or falling edge (1)
              bitOrder:      1; // LSB first (1) or MSB first (0)

    Frequency clockFrequency;
};

class Spi {
public:

    int write(std::vector<uint8_t> *);
    int read(std::vector<uint8_t> *);

    Spi(std::string, SpiConfig);
    ~Spi();

private:
    int fd;
    std::string path;

};

#endif
