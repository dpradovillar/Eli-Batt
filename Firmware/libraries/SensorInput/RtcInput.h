#ifndef __RTC_INPUT_H_
#define __RTC_INPUT_H_

#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <Endpoint.h>
#include <RTClib.h>
#include <RTC_DS3231.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

extern volatile long TOGGLE_COUNT;

/**
 * Wrapper for the RTC ChronoDot library.
 */
class RtcClock {
private:
    RTC_DS3231 m_rtc;
    byte m_ok;

    Debugger d;

public:
    bool setup(SerialEndpoint *debugEndpoint=NULL);

    DateTime now();

    float getTempAsFloat();
    int16_t getTempAsWord();
};

#endif // __RTC_INPUT_H_
