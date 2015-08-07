#include <DataStream.h>

#define TARGET_DEBUG 0

// TODO(rtapiapincheira): add timeout mechanism.

DataStreamWriter::DataStreamWriter() :
    m_endpoint(NULL)
{
}

void DataStreamWriter::setup(DualSerialEndpoint *endpoint) {
    m_endpoint = endpoint;
}

void DataStreamWriter::flush() {
    m_endpoint->flush();
}

size_t DataStreamWriter::writeByte(byte b) {
#if TARGET_DEBUG
    Serial.println("Here!");
    Serial.print(" ");
    Serial.print((uint16_t)b);
#endif
    return m_endpoint->write(b);
}

size_t DataStreamWriter::writeInt16(uint16_t x) {
    byte buffer[2];
    Utils::toByte(x, buffer);
#if TARGET_DEBUG
    Serial.print(" ");
    Serial.print(x);
#endif
    return m_endpoint->write(buffer, 0, 2);
}

size_t DataStreamWriter::writeInt32(uint32_t x) {
#if TARGET_DEBUG
    Serial.print(" ");
    Serial.print(x);
#endif
    byte buffer[4];
    Utils::toByte(x, buffer);
    return m_endpoint->write(buffer, 0, 4);
}

size_t DataStreamWriter::writeArray(byte *s, size_t n) {
#if TARGET_DEBUG
    Serial.print("{");
    for (size_t i = 0; i < n; i++) {
        Serial.print(" ");
        Serial.print((uint16_t)s[i]);
    }
    Serial.print(" }");
#endif
    return m_endpoint->write(s, 0, n);
}

DataStreamReader::DataStreamReader() :
    m_endpoint(NULL)
{
}

void DataStreamReader::setup(DualSerialEndpoint *endpoint) {
    m_endpoint = endpoint;
}

int DataStreamReader::available() {
    return m_endpoint ? m_endpoint->available() : 0;
}

byte DataStreamReader::readByte(bool *ok) {
    if (!m_endpoint) {
        if (ok) *ok = false;
        return 0;
    }
    if (ok) *ok = true;
    while(m_endpoint->available() < 1);

    int16_t code = m_endpoint->read();
    if(code < 0 && ok) {
        *ok = false;
    }
    return (byte)code;
}

uint16_t DataStreamReader::readInt16(bool *ok) {
    if (!m_endpoint) {
        if (ok) *ok = false;
        return 0;
    }
    byte buffer[2];
    readFully(buffer, 2, ok);
    return Utils::toUInt16(buffer);
}

uint32_t DataStreamReader::readInt32(bool *ok) {
    if (!m_endpoint) {
        if (ok) *ok = false;
        return 0;
    }
    byte buffer[4];
    readFully(buffer, 4, ok);
    return Utils::toInt32(buffer);
}

void DataStreamReader::readFully(byte *buff, size_t len, bool *ok) {
    if (!m_endpoint) {
        if (ok) *ok = false;
        return;
    }
    if (ok) *ok = true;
    size_t i = 0;
    while(i < len) {
        if (m_endpoint->available()){
            int16_t code = m_endpoint->read();
            if (code == -1) {
                if(ok) {
                    *ok = false;
                }
                return;
            }
            buff[i] = (byte)code;
            i++;
        }
    }
}
