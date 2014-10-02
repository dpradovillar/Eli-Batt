#include <I2cInput.h>

bool Mcp9808Sensor::setup() {
    return m_ok = m_ada.begin();
}

float Mcp9808Sensor::readCelsius() {
    return m_ok ? m_ada.readTempC() : -300.0f;
}

float Mcp9808Sensor::readFahrenheit() {
    return m_ok ? (m_ada.readTempC() * 9.0f) / 5.0f + 32.0f : -300.0f;
}

uint16_t Mcp9808Sensor::readDigital() {
    return m_ok ? m_ada.read16(MCP9808_REG_AMBIENT_TEMP) : -8190;
}

bool Mcp9808Sensor::isAllSetUp () {
    return m_ok;
}

