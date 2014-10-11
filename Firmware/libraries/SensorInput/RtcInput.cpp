#include <RtcInput.h>

bool RtcClock::setup(SerialEndpoint *debugEndpoint) {
    d.setup(debugEndpoint);

    Wire.begin();
    m_rtc.begin();
    m_ok = m_rtc.isrunning();
    if (!m_ok) {
        d.println(F("RTC is NOT running"));
        return false;
    }

    m_rtc.enable32kHz(false);
    m_rtc.SQWEnable(false);
    m_rtc.BBSQWEnable(false);

    m_rtc.getControlRegisterData();

    EICRA = 0;
    EICRA |= (1 << ISC01);
    EICRA |= (1 << ISC00);
    EIMSK |= (1 << INT0);

    return m_ok;
}

DateTime RtcClock::now() {
    TOGGLE_COUNT = 0;
    if (m_ok) {
        return m_rtc.now();
    } else {
        d.println(F("Returning garbage DateTime because RTC is not setup."));
    }
    return DateTime(0);
}

float RtcClock::getTempAsFloat() {
    if (m_ok) {
        m_rtc.forceTempConv(true); // temperature correction?
        return m_rtc.getTempAsFloat();
    } else {
        d.println(F("Returning garbage float temperature because RTC is not setup."));
    }
    return -300.0;
}

int16_t RtcClock::getTempAsWord() {
    if (m_ok) {
        return m_rtc.getTempAsWord();
    } else {
        d.println(F("Returning garbage word temperature because RTC is not setup."));
    }
    return 0;
}
