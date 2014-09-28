#include <AnalogInput.h>

AnalogInput::AnalogInput()
    : m_pin(-1) {
}

AnalogInput::~AnalogInput() {
}

void AnalogInput::setup(int pin) {
    m_pin = pin;
}

int AnalogInput::read() {
    return analogRead(m_pin);
}
