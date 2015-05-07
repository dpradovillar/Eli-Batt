#include <RtcInput.h>

bool RtcClock::setup(SerialEndpoint *debugEndpoint) {
    d.setup(debugEndpoint);

    //Wire.begin();
    m_clock.begin();
    m_ok = m_clock.isrunning();
    if (!m_ok) {
        d.println(F("RTC is NOT running"));
        return false;
    }

    m_clock.enable32kHz(false);
    m_clock.SQWEnable(false);
    m_clock.BBSQWEnable(false);

    m_clock.getControlRegisterData();

    EICRA = 0;
    EICRA |= (1 << ISC01);
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);

    return m_ok;

}

void RtcClock::adjust(const DateTime &dt) {
    m_clock.adjust(dt);
}

DateTime RtcClock::now() {
    return m_clock.now();
}

float RtcClock::getTempAsFloat() {
    return m_clock.getTempAsFloat();
}

int16_t RtcClock::getTempAsWord() {
    return m_clock.getTempAsWord();
}
