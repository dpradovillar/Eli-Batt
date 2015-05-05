#ifndef __RTC_INPUT_H_
#define __RTC_INPUT_H_

#include <Adafruit_MCP9808.h>
#include <Arduino.h>
#include <ArduinoSoftwareSerial.h>
#include <Debugger.h>
#include <DS3231.h>
#include <Endpoint.h>
#include <SPI.h>
#include <Utils.h>
#include <Wire.h>

/**
 * Wrapper for the RTC ChronoDot library.
 */
class RtcClock {
private:
    DS3231 m_clock;
public:
    void setup();

//    void adjust(int year, byte month, btye day, byte hour, byte minute, byte second);

    void getTime(int &year, byte &month, byte &day, byte &hour, byte &minute, byte &second);

    float getTempAsFloat();
};

#endif // __RTC_INPUT_H_
