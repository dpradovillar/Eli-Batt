#include <RtcInput.h>

void RtcClock::setup() {
}

//void RtcClock::adjust(int year, byte month, btye day, byte hour, byte minute, byte second) {
//}

void RtcClock::getTime(int &year, byte &month, byte &day, byte &hour, byte &minute, byte &second) {
    byte y;
    byte dow;
    m_clock.getTime(y, month, day, dow, hour, minute, second);
    year = 2000+y;
}

float RtcClock::getTempAsFloat() {
    return m_clock.getTemperature();
}