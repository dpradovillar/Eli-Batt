#include <Endpoint.h>

SerialEndpoint::SerialEndpoint() {
    m_hardwareSerialIndex = -1;
}

void SerialEndpoint::setup(int rxPin, int txPin, uint32_t baud) {
    // See http://arduino.cc/en/pmwiki.php?n=Reference/Serial for the pins specification

    // Serial
    if (rxPin==0 && txPin==1) {
        m_hardwareSerialIndex = 0;
        Serial.begin(baud);
    }
#ifdef __AVR_ATmega1280__
    // Serial1
    else if (rxPin==19 && txPin==18) {
        m_hardwareSerialIndex = 1;
        Serial1.begin(baud);
    }

    // Serial2
    else if (rxPin==17 && txPin==16) {
        m_hardwareSerialIndex = 2;
        Serial2.begin(baud);
    }

    // Serial3
    else if (rxPin==15 && txPin==14) {
        m_hardwareSerialIndex = 3;
        Serial3.begin(baud);
    }
#endif
    // SoftwareSerial
    else if (rxPin > 0 || txPin > 1) {
        m_hardwareSerialIndex = -1;
        m_softwareSerial.setup(rxPin, txPin);
        m_softwareSerial.begin(baud);
    } else {
        m_hardwareSerialIndex = -2; // No Serial nor SoftwareSerial
    }
}

void SerialEndpoint::flush() {
    switch(m_hardwareSerialIndex) {
    case -1: m_softwareSerial.flush(); return;
    case 0: Serial.flush(); return;
#ifdef __AVR_ATmega1280__
    case 1: Serial1.flush(); return;
    case 2: Serial2.flush(); return;
    case 3: Serial3.flush(); return;
#endif
    }
}

int SerialEndpoint::available() {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.available();
    case 0: return Serial.available();
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.available();
    case 2: return Serial2.available();
    case 3: return Serial3.available();
#endif
    }
    return 0;
}

void SerialEndpoint::waitForConnection() {
    switch(m_hardwareSerialIndex) {
    case -1: return;
    case 0: while(!Serial); return;
#ifdef __AVR_ATmega1280__
    case 1: while(!Serial1); return;
    case 2: while(!Serial2); return;
    case 3: while(!Serial3); return;
#endif
    }
}

size_t SerialEndpoint::write(byte b) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.write(&b, 1);
    case 0: return Serial.write(&b, 1);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.write(&b, 1);
    case 2: return Serial2.write(&b, 1);
    case 3: return Serial3.write(&b, 1);
#endif
    }
    return 0;
    // TODO(rene): define better what would happen if returns 0 or 1
}

int SerialEndpoint::read() {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.read();
    case 0: return Serial.read();
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.read();
    case 2: return Serial2.read();
    case 3: return Serial3.read();
#endif
    }
    return -1;
}

size_t SerialEndpoint::write(byte *arr, size_t start, size_t len) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.write(arr+start, len);
    case 0: return Serial.write(arr+start, len);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.write(arr+start, len);
    case 2: return Serial2.write(arr+start, len);
    case 3: return Serial3.write(arr+start, len);
#endif
    }
    return 0; // TODO(rene): define better what would happen if returns 0 or 1
}

size_t SerialEndpoint::read(byte *arr, size_t start, size_t len) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.readBytes((char*)arr+start, len);
    case 0: return Serial.readBytes((char*)arr+start, len);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.readBytes((char*)arr+start, len);
    case 2: return Serial2.readBytes((char*)arr+start, len);
    case 3: return Serial3.readBytes((char*)arr+start, len);
#endif
    }
    return -1;
}

size_t SerialEndpoint::print(int x) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print(x);
    case 0: return Serial.print(x);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print(x);
    case 2: return Serial2.print(x);
    case 3: return Serial3.print(x);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(uint16_t x) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print((unsigned int)x);
    case 0: return Serial.print((unsigned long)x);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print((unsigned int)x);
    case 2: return Serial2.print((unsigned int)x);
    case 3: return Serial3.print((unsigned int)x);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(uint32_t x) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print((unsigned long)x);
    case 0: return Serial.print((unsigned long)x);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print((unsigned long)x);
    case 2: return Serial2.print((unsigned long)x);
    case 3: return Serial3.print((unsigned long)x);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(char x) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print(x);
    case 0: return Serial.print(x);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print(x);
    case 2: return Serial2.print(x);
    case 3: return Serial3.print(x);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(float f, int decimals) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print(f, decimals);
    case 0: return Serial.print(f, decimals);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print(f, decimals);
    case 2: return Serial2.print(f, decimals);
    case 3: return Serial3.print(f, decimals);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(const char *s) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print(s);
    case 0: return Serial.print(s);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print(s);
    case 2: return Serial2.print(s);
    case 3: return Serial3.print(s);
#endif
    }
    return 0;
}

size_t SerialEndpoint::print(char *s, int len) {
    uint32_t i = 0;
    size_t counter = 0;
    while(i < len) {
        counter += print(s[i]);
        i++;
    }
    return counter;
}

size_t SerialEndpoint::print(const __FlashStringHelper *s) {
    switch(m_hardwareSerialIndex) {
    case -1: return m_softwareSerial.print(s);
    case 0: return Serial.print(s);
#ifdef __AVR_ATmega1280__
    case 1: return Serial1.print(s);
    case 2: return Serial2.print(s);
    case 3: return Serial3.print(s);
#endif
    }
    return 0;
}

size_t SerialEndpoint::printFloating(int integral_part, int decimal_part) {
    return print(integral_part) + print('.') + print(decimal_part);
}

size_t SerialEndpoint::printSimpleDate(int year, int month, int day) {
    return print(year) + print('/') + print(month) + print('/') + print(day);
}

size_t SerialEndpoint::printSimpleTime(int hour, int minute, int second) {
    return print(hour) + print(':') + print(minute) + print(':') + print(second);
}

// --------------------------- .*ln() methods ---------------------------

size_t SerialEndpoint::println() {
    return print(CR_LF);
}

size_t SerialEndpoint::println(int x) {
    return print(x) + print(CR_LF);
}

size_t SerialEndpoint::println(uint16_t x) {
    return print(x) + print(CR_LF);
}

size_t SerialEndpoint::println(uint32_t x) {
    return print(x) + print(CR_LF);
}

size_t SerialEndpoint::println(char c) {
    return print(c) + print(CR_LF);
}

size_t SerialEndpoint::println(float f, int decimals) {
    return println(f, decimals) + print(CR_LF);
}

size_t SerialEndpoint::println(const char *s) {
    return print(s) + print(CR_LF);
}

size_t SerialEndpoint::println(char *s, int len) {
    return print(s, len) + print(CR_LF);
}

size_t SerialEndpoint::println(const __FlashStringHelper *s) {
    return print(s) + print(CR_LF);
}

size_t SerialEndpoint::printlnFloating(int integral_part, int decimal_part) {
    return printFloating(integral_part, decimal_part) + print(CR_LF);
}

size_t SerialEndpoint::printlnSimpleDate(int year, int month, int day) {
    return printSimpleDate(year, month, day) + print(CR_LF);
}

size_t SerialEndpoint::printlnSimpleTime(int hour, int minute, int second) {
    return printSimpleTime(hour, minute, second) + print(CR_LF);
}

// -------------------------------------------------------------------------------------------------

DualSerialEndpoint::DualSerialEndpoint() :
        m_a(NULL),
        m_b(NULL) {
}

void DualSerialEndpoint::setup(SerialEndpoint *readWrite,
        SerialEndpoint *writeOnly) {
    m_a = readWrite;
    m_b = writeOnly;
}

void DualSerialEndpoint::flush() {
    m_a->flush();
    if (m_b) m_b->flush();
}
int DualSerialEndpoint::available() {
    return m_a->available();
}
void DualSerialEndpoint::waitForConnection() {
    m_a->waitForConnection();
    if (m_b) m_b->waitForConnection();
}

size_t DualSerialEndpoint::write(byte b) {
    size_t r = m_a->write(b);
    if (m_b) m_b->write(b);
    return r;
}
int DualSerialEndpoint::read() {
    return m_a->read();
}

size_t DualSerialEndpoint::write(byte *arr, size_t start, size_t len) {
    size_t r = m_a->write(arr, start, len);
    if (m_b) m_b->write(arr, start, len);
    return r;
}
size_t DualSerialEndpoint::read(byte *arr, size_t start, size_t len) {
    return m_a->read(arr, start, len);
}

size_t DualSerialEndpoint::print(int x) {
    size_t r = m_a->print(x);
    if (m_b) m_b->print(x);
    return r;
}
size_t DualSerialEndpoint::print(uint16_t x) {
    size_t r = m_a->print(x);
    if (m_b) m_b->print(x);
    return r;
}
size_t DualSerialEndpoint::print(uint32_t x) {
    size_t r = m_a->print(x);
    if (m_b) m_b->print(x);
    return r;
}
size_t DualSerialEndpoint::print(char c) {
    size_t r = m_a->print(c);
    if (m_b) m_b->print(c);
    return r;
}
size_t DualSerialEndpoint::print(const char *s) {
    size_t r = m_a->print(s);
    if (m_b) m_b->print(s);
    return r;
}
size_t DualSerialEndpoint::print(char *s, int len) {
    size_t r = m_a->print(s, len);
    if (m_b) m_b->print(s, len);
    return r;
}
size_t DualSerialEndpoint::print(const __FlashStringHelper *s) {
    size_t r = m_a->print(s);
    if (m_b) m_b->print(s);
    return r;
}

size_t DualSerialEndpoint::println() {
    size_t r = m_a->println();
    if (m_b) m_b->println();
    return r;
}
size_t DualSerialEndpoint::println(int x) {
    size_t r = m_a->println(x);
    if (m_b) m_b->println(x);
    return r;
}
size_t DualSerialEndpoint::println(uint16_t x) {
    size_t r = m_a->println(x);
    if (m_b) m_b->println(x);
    return r;
}
size_t DualSerialEndpoint::println(uint32_t x) {
    size_t r = m_a->println(x);
    if (m_b) m_b->println(x);
    return r;
}
size_t DualSerialEndpoint::println(char x) {
    size_t r = m_a->println(x);
    if (m_b) m_b->println(x);
    return r;
}
size_t DualSerialEndpoint::println(const char *s) {
    size_t r = m_a->println(s);
    if (m_b) m_b->println(s);
    return r;
}
size_t DualSerialEndpoint::println(char *s, int len) {
    size_t r = m_a->println(s, len);
    if (m_b) m_b->println(s, len);
    return r;
}
size_t DualSerialEndpoint::println(const __FlashStringHelper *s) {
    size_t r = m_a->println(s);
    if (m_b) m_b->println(s);
    return r;
}

