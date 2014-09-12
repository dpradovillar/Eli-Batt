#include <I2cInput.h>
/*
I2cInput::I2cInput() {
}

I2cInput::~I2cInput() {
}

void I2cInput::setup(int pin) {
    m_pin = pin;
}

int I2cInput::read() {
    return
}
*/
void Mcp9808Sensor::setup() {
    m_ok = m_ada.begin();
}

float Mcp9808Sensor::readCelsius() {
    return m_ada.readTempC();
}

float Mcp9808Sensor::readFahrenheit() {
    return (m_ada.readTempC() * 9.0f) / 5.0f + 32.0f;
}

uint16_t Mcp9808Sensor::readDigital() {
    return m_ada.read16(MCP9808_REG_AMBIENT_TEMP);
}

bool Mcp9808Sensor::isAllSetUp () {
    return m_ok;
}

