#include <Debugger.h>

Debugger::Debugger() :
        m_endpoint(NULL)
{
}

void Debugger::setup(SerialEndpoint *endpoint) {
    m_endpoint = endpoint;
}

Debugger& Debugger::write(byte b) {
    if (m_endpoint) {
        m_endpoint->write(b);
    }
    return *this;
}
Debugger& Debugger::write(byte *arr, size_t len) {
    if (m_endpoint) {
        m_endpoint->write(arr, 0, len);
    }
    return *this;
}
Debugger& Debugger::write(byte *arr, size_t start, size_t len) {
    if (m_endpoint) {
        m_endpoint->write(arr, start, len);
    }
    return *this;
}

Debugger& Debugger::print(int x) {
    if (m_endpoint) {
        m_endpoint->print(x);
    }
    return *this;
}
Debugger& Debugger::print(uint16_t x) {
    if (m_endpoint) {
        m_endpoint->print(x);
    }
    return *this;
}
Debugger& Debugger::print(uint32_t x) {
    if (m_endpoint) {
        m_endpoint->print(x);
    }
    return *this;
}
Debugger& Debugger::print(char c) {
    if (m_endpoint) {
        m_endpoint->print(c);
    }
    return *this;
}
Debugger& Debugger::print(const char *s) {
    if (m_endpoint) {
        m_endpoint->print(s);
    }
    return *this;
}
Debugger& Debugger::print(char *s, int len) {
    if (m_endpoint) {
        m_endpoint->print(s, len);
    }
    return *this;
}

Debugger& Debugger::print(const __FlashStringHelper *s) {
    if (m_endpoint) {
        m_endpoint->print(s);
    }
    return *this;
}

Debugger& Debugger::println() {
    if (m_endpoint) {
        m_endpoint->println();
    }
    return *this;
}
Debugger& Debugger::println(int x) {
    if (m_endpoint) {
        m_endpoint->println(x);
    }
    return *this;
}
Debugger& Debugger::println(uint16_t x) {
    if (m_endpoint) {
        m_endpoint->println(x);
    }
    return *this;
}
Debugger& Debugger::println(uint32_t x) {
    if (m_endpoint) {
        m_endpoint->println(x);
    }
    return *this;
}
Debugger& Debugger::println(char x) {
    if (m_endpoint) {
        m_endpoint->println(x);
    }
    return *this;
}
Debugger& Debugger::println(const char *s) {
    if (m_endpoint) {
        m_endpoint->println(s);
    }
    return *this;
}
Debugger& Debugger::println(char *s, int len) {
    if (m_endpoint) {
        m_endpoint->println(s, len);
    }
    return *this;
}

Debugger& Debugger::println(const __FlashStringHelper *s) {
    if (m_endpoint) {
        m_endpoint->println(s);
    }
    return *this;
}

Debugger& Debugger::printInt(byte *buff4bytes) {
    if (m_endpoint) {
        uint32_t val = Utils::toInt32(buff4bytes);
        m_endpoint->print(val);
    }
    return *this;
}

Debugger& Debugger::printHexUInt16(uint16_t value) {
	byte buffer[2];
	Utils::toByte(value, buffer);
	printHexUInt16(buffer);
}

Debugger& Debugger::printHexUInt16(byte *buff2bytes) {
    if (m_endpoint) {
        uint16_t val = Utils::toUInt16(buff2bytes);
        char buffer[6];
        Utils::toHex(buffer+2, val);
        buffer[0] = '0';
        buffer[1] = 'x';
        m_endpoint->print(buffer, 6);
    }
    return *this;
}

Debugger& Debugger::printHexInt(byte *buff4bytes) {
    if (m_endpoint) {
        uint32_t val = Utils::toInt32(buff4bytes);
        char buffer[10];
        Utils::toHex(buffer+2, val);
        buffer[0] = '0';
        buffer[1] = 'x';
        m_endpoint->print(buffer, 10);
    }
    return *this;
}
