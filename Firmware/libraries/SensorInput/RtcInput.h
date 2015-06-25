#ifndef __RTC_INPUT_H_
#define __RTC_INPUT_H_

#include <Arduino.h>
#include <SPI.h>

class MyDate {
public:
    uint16_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    MyDate();
};

/**
 * Wrapper for the RTC ChronoDot library.
 */
class RtcClock {
private:
    byte m_ok;
    MyDate m_lastDate;
    int m_pin;

private:
    void RTC_init();
    void RTC_setTimeDate(const MyDate &d);
    void RTC_readTimeDate();

public:
    RtcClock();

    bool setup(int pin);

    void adjust(const MyDate &dt);

    MyDate readDate();

    MyDate getLastDate();

    bool isAllSetUp();
};

#endif // __RTC_INPUT_H_
