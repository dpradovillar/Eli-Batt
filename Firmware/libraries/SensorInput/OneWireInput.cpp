#include <OneWireInput.h>

bool Ds1820Sensor::setup(int pin) {
    m_one.setup(pin);
    m_sensors.setup(&m_one);
    m_sensors.begin();
    return (m_ok = true);
}

float Ds1820Sensor::readCelsius() {
    if (!m_ok) {
        return -300.0f;
    }
    m_sensors.requestTemperatures();
    return m_sensors.getTempCByIndex(0);
}
