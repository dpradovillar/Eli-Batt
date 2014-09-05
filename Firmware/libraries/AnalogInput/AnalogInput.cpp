#include <AnalogInput.h>

AnalogInput::AnalogInput() {
}

AnalogInput::~AnalogInput() {
}

void AnalogInput::setup(int pin) {
    m_pin = pin;
}

int AnalogInput::read() {
    return analogRead(m_pin);
}
