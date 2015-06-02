#ifndef __RTC_INPUT_H_
#define __RTC_INPUT_H_

#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <Endpoint.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

/**
 * Wrapper for the RTC ChronoDot library.
 */
class RtcClock {
private:
    RTC_DS3231 m_clock;
    byte m_ok;

    Debugger d;
public:
    RtcClock();

    bool setup(SerialEndpoint *debugEndpoint=NULL);

    void adjust(const DateTime &dt);

    DateTime now();

    float getTempAsFloat();
    int16_t getTempAsWord();

    bool isAllSetUp();
};

#endif // __RTC_INPUT_H_
