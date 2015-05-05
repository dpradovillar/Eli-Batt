#ifndef __ONEWIRE_INPUT_H_
#define __ONEWIRE_INPUT_H_

#include <Arduino.h>
#include <OneWire.h>

/**
 * MCP9808 sensor, using internally Adafruit's library.
 */
class Ds1820Sensor {
private:
    byte m_ok;
    OneWire m_sensor;
    float m_celsius;
    //float m_fahrenheit;
    
    bool readInternalTemperature();

public:
    bool setup(int pin);
    float readCelsius();
    //float readFahrenheit();
};

#endif // __ONEWIRE_INPUT_H_
