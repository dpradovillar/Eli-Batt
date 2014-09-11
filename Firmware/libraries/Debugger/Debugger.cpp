#include <Debugger.h>

Debugger::Debugger() :
        m_endpoint(NULL)
{
}

void Debugger::setup(Endpoint *endpoint) {
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
        m_endpoint->write(arr, len);
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

Debugger& Debugger::printInt(byte *buff4bytes) {
    if (m_endpoint) {
    	uint32_t val = Utils::toInt32(buff4bytes);
        m_endpoint->print(val);
    }
    return *this;
}
