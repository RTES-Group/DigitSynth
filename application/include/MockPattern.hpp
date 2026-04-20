#ifndef MockPattern_hpp
#define MockPattern_hpp

#include "ILedDriver.hpp"
#include "patterns.h"

class MockPattern : public Pattern {
public:
    MockPattern(led_driver::ILedDriver &ledDriver) : Pattern(ledDriver) {}

    void start(DoneCallback onDone = nullptr) override {
        (void) onDone; 
	    startCalled = true;
    }
    void stop() override { stopCalled = true; }
    bool startCalled = false;
    bool stopCalled = false;
protected:
    void run() override {}
};

#endif /* MockPattern_hpp */
