#ifndef __ONEWIRE_INPUT_H_
#define __ONEWIRE_INPUT_H_

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * MCP9808 sensor, using internally Adafruit's library.
 */
class Ds1820Sensor {
private:
    byte m_ok;

    byte m_addr[9];

    OneWire m_one;
    DallasTemperature m_sensors;
    DeviceAddress m_deviceAddress;

public:
    bool setup(int pin);
    float readCelsius();
};

#endif // __ONEWIRE_INPUT_H_
