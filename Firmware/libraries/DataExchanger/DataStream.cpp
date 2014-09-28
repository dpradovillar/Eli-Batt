#include <DataStream.h>

// TODO(rtapiapincheira): add timeout mechanism.

DataStreamWriter::DataStreamWriter() :
	m_endpoint(NULL)
{
}

void DataStreamWriter::setup(SerialEndpoint *endpoint) {
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
    size_t r = 0;
    r += m_endpoint->write(hi);
    r += m_endpoint->write(lo);
    return r;
}

size_t DataStreamWriter::writeString(const String &s) {
    size_t n = s.length();
    return writeShort(n) + writeArray((byte*)s.c_str(), n);
}

size_t DataStreamWriter::writeArray(byte *s, size_t n) {
    return m_endpoint->write(s, 0, n);
}

DataStreamReader::DataStreamReader() :
    m_endpoint(NULL)
{
}

void DataStreamReader::setup(SerialEndpoint *endpoint) {
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

short DataStreamReader::readShort(bool *ok) {
	if (!m_endpoint) {
		if (ok) *ok = false;
		return 0;
	}
	if (ok) *ok = true;
    while(m_endpoint->available() < 2);

    int16_t hi = m_endpoint->read();
    int16_t lo = m_endpoint->read();
    if (ok && (hi < 0 || lo < 0)) {
        *ok = false;
    }
    return (short)(hi << 8 | lo << 0);
}

int32_t DataStreamReader::readInt(bool *ok) {
	if (!m_endpoint) {
		if (ok) *ok = false;
		return 0;
	}
    short hi = readShort(ok);
    if (ok && !*ok) {
		return 0;
	}
    short lo = readShort(ok);
    if (ok && !*ok) {
        return 0;
    }
    return (int32_t)(hi << 16 | lo << 0);
}

void DataStreamReader::readFully(byte *buff, size_t len, bool *ok) {
	if (!m_endpoint) {
		if (ok) *ok = false;
		return;
	}
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
