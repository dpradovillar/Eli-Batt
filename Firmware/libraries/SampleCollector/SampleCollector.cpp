#include "SampleCollector.h"

#if ENABLED_SAMPLE_COLLECTOR

void SampleCallback::eventDetected(uint32_t current_usecs) {
	// This method is intended to be overriden!
}

void SerialCallback::eventDetected(uint32_t current_usecs) {
	Serial.print("Micros:");
	Serial.print(current_usecs);
	Serial.print(",");

	Serial.print("Millis:");
	Serial.print(current_usecs/1000);
	Serial.print(",");

	Serial.print("Secs:");
	Serial.print(current_usecs/1000000);
	Serial.println();
}

SampleClock::SampleClock(uint32_t fs, const SampleCallback &callback) :
	m_period(1000000/fs),
	m_lastloop(0),
	m_callback(callback)
{
}

void SampleClock::setPeriod(uint32_t uSecs) {
	m_period = uSecs;
}

void SampleClock::loop() {
	loop(micros());
}


void SampleClock::loop(uint32_t current_usecs) {
	// If enough time has passed, trigger callback!
	if (current_usecs - m_lastloop >= m_period) {
		m_callback.eventDetected(current_usecs);
		// Reset passed time, so as to wait another period
		m_lastloop = current_usecs;
	}
}

#endif // ENABLED_SAMPLE_COLLECTOR
