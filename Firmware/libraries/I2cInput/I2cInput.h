#ifndef __I2C_INPUT_H_
#define __I2C_INPUT_H_

#include "Arduino.h"
#include <Adafruit_MCP9808.h>

#define SDA_PIN A4
#define SCL_PIN A5

/**
 * This class needs a lot of device specific logic, use it only as a model.
 */
/*
class I2cInput {

public:
    I2cInput();
    ~I2cInput();

    // connect sda=A4 and scl=A5 on UNO!
    void setup();

    int read();
};
*/

/**
 * MCP9808 sensor, using internally Adafruit's library.
 */
class Mcp9808Sensor {
private:
    byte m_ok;
    Adafruit_MCP9808 m_ada;
public:
    void setup();
    float readCelsius();
    float readFahrenheit();
};

#endif // __I2C_INPUT_H_
