#include "spi.h"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <optional>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

void checkError(int result) {
    if (result < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(result);
    }
}

Spi::Spi(std::string path, SpiConfig config) {
    this->fd = open(path.c_str(), O_RDWR);
    if (fd < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(fd);
    }

    uint8_t mode = (config.clockPolarity << 1) & (config.clockPhase);
    uint8_t bitsPerWord = config.bitsPerWord;
    uint8_t bitOrder = config.bitOrder;

    checkError(ioctl(fd, SPI_IOC_WR_MODE,          &mode));
    checkError(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bitsPerWord));
    checkError(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,  &config.clockFrequency));
    checkError(ioctl(fd, SPI_IOC_WR_LSB_FIRST,     &bitOrder));
}

Spi::~Spi() {
    checkError(close(this->fd));
}

int Spi::read(std::vector<uint8_t> *dest, SpiDevice device) {
    (void) device; // TODO: add something to handle chip select
    
    uint8_t *internalBuf = dest->data();

    return read(fd, internalBuf, dest->size());
}

int Spi::write(std::vector<uint8_t> *src, SpiDevice device) {
    (void) device; // TODO: add something to handle chip select
    
    uint8_t *internalBuf = src->data();

    return write(fd, internalBuf, src->size());
}

std::optional<SpiDevice> Spi::addDevice() {
    if (this->nDevices >= SPI_MAX_DEVICES) {
        return {};
    }
    
    auto spiDevice = (SpiDevice) this->nDevices;
    this->nDevices++;
    
    return spiDevice;
}
