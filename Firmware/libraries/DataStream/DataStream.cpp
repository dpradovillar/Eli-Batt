#include <DataStream.h>

// TODO(rtapiapincheira): add timeout mechanism.

DataObject::~DataObject() {
}

void DataStreamWriter::setup(Endpoint *endpoint) {
    m_endpoint = endpoint;
}

void DataStreamWriter::flush() {
    m_endpoint->flush();
}

size_t DataStreamWriter::writeByte(byte b) {
     return m_endpoint->write(b);
}

size_t DataStreamWriter::writeShort(short s) {
     byte hi = (byte)(s >> 8);
     byte lo = (byte)(s >> 0);
     return m_endpoint->write(hi) + m_endpoint->write(lo);
}

size_t DataStreamWriter::writeString(const String &s) {
    size_t n = s.length();
    return writeShort(n) + writeArray((byte*)s.c_str(), n);
}

size_t DataStreamWriter::writeArray(byte *s, size_t n) {
    return m_endpoint->write(s, n);
}

size_t DataStreamWriter::writeObject(DataObject *obj) {
    return obj->writeTo(this);
}

void DataStreamReader::setup(Endpoint *endpoint) {
    m_endpoint = endpoint;
}

int DataStreamReader::available() {
    return m_endpoint->available();
}

byte DataStreamReader::readByte(bool *ok) {
    while(m_endpoint->available() < 1);
    int16_t code = m_endpoint->read();
    if(code < 0 && ok) {
        *ok = false;
    }
    return (byte)code;
}

short DataStreamReader::readShort(bool *ok) {
    while(!m_endpoint->available() < 2);
    int16_t hi = m_endpoint->read();
    int16_t lo = m_endpoint->read();
    if (ok && (hi < 0 || lo < 0)) {
        *ok = false;
    }
    return (hi << 8 | lo << 0) & 0xFFFF;
}

int32_t DataStreamReader::readInt(bool *ok) {
    short hi = readShort(ok);
    short lo = readShort(ok);
    if (ok && !*ok) {
        return 0;
    }
    return (hi << 16 | lo << 0) & 0xFFFFFFFF;
}

void DataStreamReader::readFully(byte *buff, size_t len, bool *ok) {
    size_t i = 0;
    while(i < len) {
        int av = m_endpoint->available();
        av = min(len, av);
        m_endpoint->read(buff, i, len-i);
        i += av;
    }
}

String DataStreamReader::readString(bool *ok) {
	uint16_t num = readShort(ok);
    if (ok && !*ok) {
        return String();
    }
    String res;
    res.reserve(num);
    size_t i = 0;
    while(i<num) {
        if (m_endpoint->available()) {
            res[i++] = readByte(ok);
            if (ok && !*ok) {
                return String();
            }
        }
    }
    return res;
}
/*
float DataStreamReader::readFloat(bool *ok) {
    // TODO(rtapiapincheira): maybe flip back the bytes, because of the endianess
    char buff[4];
    readFully(buff, 4, ok);
    if (ok && !*ok) {
        return 0.0f;
    }
    return *((float*)buff);
}

double DataStreamReader::readDouble(bool *ok) {
    // TODO(rtapiapincheira): maybe flip back the bytes, because of the endianess
    char buff[8];
    readFully(buff, 8, ok);
    if (ok && !*ok) {
        return 0.0f;
    }
    return *((double*)buff);
}
*/
void DataStreamReader::readObject(DataObject *obj, bool *ok) {
    size_t s = obj->readFrom(this);
    // We're using 2^16-1 as indicator of failure with unsigned ints.
    // Strictly speaking, any value above 1024 may be used as indicator of failure, as any object
    // size is less than 1Kb due to memory constraints.
    if (s == -1 && ok) {
        *ok = false;
    }
}
