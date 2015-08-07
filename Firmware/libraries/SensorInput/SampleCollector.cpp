#include <SampleCollector.h>

SampleCallback::~SampleCallback() {

}

void SampleCallback::eventDetected(uint32_t current_usecs) {
    // This method is intended to be overriden!
}

SerialCallback::~SerialCallback(){

}

void SerialCallback::eventDetected(uint32_t current_usecs) {
    Serial.print(F("Micros:"));
    Serial.print(current_usecs);
    Serial.print(F(","));

    Serial.print(F("Millis:"));
    Serial.print(current_usecs/1000);
    Serial.print(F(","));

    Serial.print(F("Secs:"));
    Serial.print(current_usecs/1000000);
    Serial.println();
}

LedBlinkCallback::LedBlinkCallback() :
    m_pin(0), m_state(LOW) {
}

LedBlinkCallback::~LedBlinkCallback() {
}

void LedBlinkCallback::setup(int pin) {
    m_pin = pin;
    pinMode(m_pin, OUTPUT);
}

void LedBlinkCallback::eventDetected(uint32_t current_usecs) {
    m_state = !m_state;
    digitalWrite(m_pin, m_state);
}

SampleClock::SampleClock() :
    m_period(0),
    m_lastloop(0),
    m_callback(NULL)
{}

void SampleClock::setup(uint32_t fs, SampleCallback *callback) {
    m_period = 1000000/fs;
    m_lastloop = 0;
    m_callback = callback;
}

void SampleClock::setPeriod(uint32_t uSecs) {
    m_period = uSecs;
}

void SampleClock::loop() {
    loop(micros());
}

void SampleClock::loop(uint32_t current_usecs) {
    // If enough time has passed, trigger callback!
    if (current_usecs-m_lastloop >= m_period) {
        m_callback->eventDetected(current_usecs);
        // Reset passed time, so as to wait another period
        m_lastloop = current_usecs;
    }
}
