#ifndef __ANALOG_INPUT_H_
#define __ANALOG_INPUT_H_

#include "Arduino.h"

class AnalogInput {
private:
    int m_pin;

public:
    AnalogInput();
    ~AnalogInput();

    void setup(int pin);

    int read();
};

#endif // __ANALOG_INPUT_H_
