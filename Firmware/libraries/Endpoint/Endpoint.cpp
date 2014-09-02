#include <Endpoint.h>

size_t Endpoint::write(byte *arr, size_t len) {
    write(arr, 0, len);
}
size_t Endpoint::read(byte *arr, size_t len) {
    read(arr, 0, len);
}

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
#else
    // SoftwareSerial
    else {
        m_softwareSerial.setup(rxPin, txPin);
        m_softwareSerial.begin(baud);
    }
#endif
}

void SerialEndpoint::flush() {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: Serial.flush(); return;
        case 1: Serial1.flush(); return;
        case 2: Serial2.flush(); return;
        case 3: Serial3.flush(); return;
        }
#else
        Serial.flush();
#endif
    } else {
        m_softwareSerial.flush();
    }
}

int SerialEndpoint::available() {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.available();
        case 1: return Serial1.available();
        case 2: return Serial2.available();
        case 3: return Serial3.available();
        }
#else
        return Serial.available();
#endif
    }
    return m_softwareSerial.available();
}

void SerialEndpoint::waitForConnection() {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: while(!Serial); return;
        case 1: while(!Serial1); return;
        case 2: while(!Serial2); return;
        case 3: while(!Serial3); return;
        }
#else
        while(!Serial);
#endif
    }
}

size_t SerialEndpoint::write(byte b) {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.print(b);
        case 1: return Serial1.print(b);
        case 2: return Serial2.print(b);
        case 3: return Serial3.print(b);
        }
#else
        return Serial.print(b);
#endif
    } else {
        return m_softwareSerial.print(b);
    }
}

int SerialEndpoint::read() {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.read();
        case 1: return Serial1.read();
        case 2: return Serial2.read();
        case 3: return Serial3.read();
        }
#else
        return Serial.read();
#endif
    } else {
        return m_softwareSerial.read();
    }
}

size_t SerialEndpoint::write(byte *arr, size_t start, size_t len) {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.write(arr+start, len);
        case 1: return Serial1.write(arr+start, len);
        case 2: return Serial2.write(arr+start, len);
        case 3: return Serial3.write(arr+start, len);
        }
#else
        return Serial.write(arr+start, len);
#endif
    } else {
        return m_softwareSerial.write(arr+start, len);
    }
}

size_t SerialEndpoint::read(byte *arr, size_t start, size_t len) {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.readBytes((char*)arr+start, len);
        case 1: return Serial1.readBytes((char*)arr+start, len);
        case 2: return Serial2.readBytes((char*)arr+start, len);
        case 3: return Serial3.readBytes((char*)arr+start, len);
        }
#else
        return Serial.readBytes((char*)arr+start, len);
#endif
    } else {
        return m_softwareSerial.readBytes((char*)arr+start, len);
    }
}

size_t SerialEndpoint::print(int x) {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.print(x);
        case 1: return Serial1.print(x);
        case 2: return Serial2.print(x);
        case 3: return Serial3.print(x);
        }
#else
        return Serial.print(x);
#endif
    } else {
        return m_softwareSerial.print(x);
    }
}

size_t SerialEndpoint::print(char x) {
    if (m_hardwareSerialIndex >= 0) {
#ifdef __AVR_ATmega1280__
        switch(m_hardwareSerialIndex) {
        case 0: return Serial.print(x);
        case 1: return Serial1.print(x);
        case 2: return Serial2.print(x);
        case 3: return Serial3.print(x);
        }
#else
        return Serial.print(x);
#endif
    } else {
        return m_softwareSerial.print(x);
    }
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

size_t SerialEndpoint::print(const char *s, int x) {
    return print(s) + print(x);
}

size_t SerialEndpoint::println() {
    return print("\n");
}

size_t SerialEndpoint::println(int x) {
    return print(x) + print("\n");
}

size_t SerialEndpoint::println(char c) {
    return print(c) + print("\n");
}

size_t SerialEndpoint::println(const char *s) {
    return print(s) + print("\n");
}

size_t SerialEndpoint::println(char *s, int len) {
    return print(s, len) + print("\n");
}

size_t SerialEndpoint::println(const char *s, int x) {
    return print(s, x) + print("\n");
}

