#include <OneWireInput.h>

bool Ds1820Sensor::setup(int pin) {
    m_one.setup(pin);
    m_dallasTemperature.setup(&m_one);
    m_dallasTemperature.setResolution(9);
    m_dallasTemperature.setWaitForConversion(false);
    m_dallasTemperature.begin();
    return (m_ok = true);
}

float Ds1820Sensor::readCelsius() {
    if (!m_ok) {
        return -300.0f;
    }
    m_dallasTemperature.requestTemperatures();
    return m_dallasTemperature.getTempCByIndex(0);
}
