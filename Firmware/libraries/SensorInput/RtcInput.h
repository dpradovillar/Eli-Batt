#ifndef __RTC_INPUT_H_
#define __RTC_INPUT_H_

#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

class MyDate {
public:
    uint16_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    MyDate() {
    }
};

/**
 * Wrapper for the RTC ChronoDot library.
 */
class RtcClock {
private:
    byte m_ok;

    Debugger d;
public:
    RtcClock();

    bool setup(SerialEndpoint *debugEndpoint=NULL);

    void adjust(const MyDate &dt);

    MyDate now();

    float getTempAsFloat();
    int16_t getTempAsWord();

    bool isAllSetUp();
};

#endif // __RTC_INPUT_H_
