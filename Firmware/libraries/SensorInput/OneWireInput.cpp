#include <OneWireInput.h>

bool Ds1820Sensor::readInternalTemperature() {
    byte addr[8];
    if (!m_sensor.search(addr)) {
        m_sensor.reset_search();
        //delay(250);
        return false;
    }
  
    if (OneWire::crc8(addr, 7) != addr[7]) {
        Serial.println("CRC is not valid!");
        return false;
    }
 
    byte type_s;
    // the first ROM byte indicates which chip
    switch (addr[0]) {
    case 0x10:
        type_s = 1;
        break;
    case 0x28:
        type_s = 0;
        break;
    case 0x22:
        type_s = 0;
        break;
    default:
        return false;
    } 

    m_sensor.reset();
    m_sensor.select(addr);
    m_sensor.write(0x44, 1);        // start conversion, with parasite power on at the end
  
    delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
  
    byte present = m_sensor.reset();
    m_sensor.select(addr);    
    m_sensor.write(0xBE);         // Read Scratchpad

    byte data[12];
    for (int i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = m_sensor.read();
    }

    int16_t raw = (data[1] << 8) | data[0];
    if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
    }
    m_celsius = (float)raw / 16.0;
    //m_fahrenheit = m_celsius * 1.8 + 32.0;
    return true;
}

bool Ds1820Sensor::setup(int pin) {
    m_sensor.setup(pin);
    return m_ok = true;
}

float Ds1820Sensor::readCelsius() {
    if (!readInternalTemperature()) {
        return -300.0f;
    }
    return m_celsius;
}
/*
float Ds1820Sensor::readFahrenheit() {
    if (!readInternalTemperature()) {
        return -300.0f;
    }
    return m_fahrenheit;
}*/

