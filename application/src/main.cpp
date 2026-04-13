#include "gpio.h"


int main() {
    
    gpio::setupGpio();
    
    getchar();
    
    gpio::teardownGpio();
    
    return 0;
}
