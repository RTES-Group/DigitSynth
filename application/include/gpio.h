#ifndef _GPIO_H
#define _GPIO_H

#include <vector>
namespace gpio {
    void setupGpio();
    
    void setPin(const char *pin, bool value);
    bool getPin(const char *pin);
}

#endif
