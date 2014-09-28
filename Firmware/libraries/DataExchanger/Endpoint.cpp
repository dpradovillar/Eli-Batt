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
    case -1: while(!Serial); return;
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
    case -1: return Serial.write(&b, 1);
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

size_t SerialEndpoint::print(const char *s) {
    uint32_t i = 0;
    size_t counter = 0;
    while(s[i]) {
        counter += print(s[i]);
        i++;
    }
    return counter;
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

size_t SerialEndpoint::println(const char *s) {
    return print(s) + print(CR_LF);
}

size_t SerialEndpoint::println(char *s, int len) {
    return print(s, len) + print(CR_LF);
}

