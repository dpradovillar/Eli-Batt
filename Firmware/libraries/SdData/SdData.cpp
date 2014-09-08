#include <SdData.h>

SdNameSequencer::SdNameSequencer() :
        m_next(0) {
}

void SdNameSequencer::next(char *buff12bytes) {
    // Names of the files can't be more than 12 characters long.
    Utils::copyArray("FILE----.CSV", (byte*)buff12bytes, 12);
    Utils::toHex(buff12bytes+4, m_next++);
}

SdWriter::SdWriter() :
    m_chip_select_pin(0),
    m_open(false),
    m_debug_endpoint(NULL) {
}

void SdWriter::setDebugEndpoint(Endpoint *debugEndpoint) {
    m_debug_endpoint = debugEndpoint;
}

bool SdWriter::setup(int chipSelectPin) {
    m_chip_select_pin = chipSelectPin;

    pinMode(10/*m_chip_select_pin*/, OUTPUT);
    if(m_debug_endpoint) {
        m_debug_endpoint->print("begining sd with pin:");
        m_debug_endpoint->println((int)m_chip_select_pin);
    }
    if (!SD.begin(m_chip_select_pin)) {
        if (m_debug_endpoint){
            m_debug_endpoint->println("Failed initialization of SD card. Check wiring.");
        }
        return false;
    }
    return true;
}

bool SdWriter::open() {
    if (m_open) {
        if(!close()) {
            return false;
        }
    }

    if (m_debug_endpoint) {
        m_debug_endpoint->print("getting next filename from sequence");
    }
    char buffer[13];
    m_sequence.next(buffer);
    buffer[12] = 0;

    if (m_debug_endpoint) {
        m_debug_endpoint->print("New file open:");
        m_debug_endpoint->println(buffer);
    }

    m_file = SD.open(buffer, FILE_WRITE);
    m_open = (m_file ? true : false);

    if (m_debug_endpoint) {
        m_debug_endpoint->println(m_open ? "file open!" : "file unopened");
    }

    return m_open;
}

bool SdWriter::close() {
    if(m_open) {
        m_file.close();
        m_open = false;
        if (m_debug_endpoint) {
            m_debug_endpoint->println("closing file.");
        }
        return true;
    } else {
        if (m_debug_endpoint) {
            m_debug_endpoint->println("file already closed! nothing to close");
        }
    }
    return false;
}

size_t SdWriter::write(char *s, size_t n) {
    return m_file.write((byte*)s, n);
}

void SdWriter::writeObject(DataObject *obj) {
    // TODO(rtapiapinchera): implement method
}

void SdWriter::writeAsciiObject(DataObject *obj) {
    // TODO(rtapiapincheira): implement method
}
