#include "adc-driver.h"
#include "types.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>

AdcDriver::AdcDriver(std::shared_ptr<Spi> spi, AdcSettings settings) : spi(spi) {
    auto device = this->spi->addDevice();
    if (!device.has_value()) { std::cerr << "Not enough SPI devices to instantiate ADC\n"; exit(-1); }
    
    this->spiDevice = device.value();
    
    uint8_t statusVal = 0x00, ioVal = 0x00;
    statusVal |= (settings.lsbFirst & 0x01) << 3;
    statusVal |= (settings.autoCalibration & 0x01) << 2; 
    statusVal |= (settings.analogueInputBuffer & 0x01) << 1;
    
    uint8_t muxVal = 0x11; // +'ve channel 0 selected, -'ve AINCOM selected
    
    uint8_t adconVal = (settings.logGain & 0b00000111); 
    
    this->writeRegister(statusVal, Ads1256Register::STATUS);
    this->writeRegister(muxVal,    Ads1256Register::MUX);
    this->writeRegister(adconVal,  Ads1256Register::ADCON);
    this->writeRegister(ioVal,     Ads1256Register::IO);
}

void AdcDriver::writeRegister(uint8_t value, Ads1256Register reg) {
    std::array<uint8_t, 4> buf;
    buf[0] = 0b0101000 | (0x0f & reg); 
    buf[1] = 0x00; // writing 1 byte
    buf[2] = 0x00;
    buf[3] = value;
    
    std::vector<uint8_t> vec(buf.begin(), buf.end()); 
    this->spi.get()->write(vec, this->spiDevice);
}
