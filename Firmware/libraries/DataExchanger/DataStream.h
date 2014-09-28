#ifndef __DATA_STREAM_H__
#define __DATA_STREAM_H__

#include <Endpoint.h>
#include <Utils.h>
#include "Arduino.h"

class DataStreamWriter;
class DataStreamReader;

/**
 * Wraper for an endpoint, to write abstract datatypes. This class is in charge of the serialization
 * to bytes of the most common datatypes in the C++ language and provides an extensible way of
 * writing custom types.
 */
class DataStreamWriter {
private:
    SerialEndpoint *m_endpoint;
public:
    DataStreamWriter();

    void setup(SerialEndpoint *endpoint);
    void flush();

    size_t writeByte(byte b);
    size_t writeInt16(uint16_t x);
    size_t writeInt32(uint32_t x);
    size_t writeArray(byte *s, size_t n);
};

/**
 * Wrapper for an endpoint, to read abstract datatypes. This class is in charge of the
 * deserialization from bytes of the most common datatypes in the C++ language and provides an
 * extensible way of reading custom types.
 */
class DataStreamReader {
private:
    SerialEndpoint *m_endpoint;
public:
    DataStreamReader();

    void setup(SerialEndpoint *endpoint);
    int available();

    byte readByte(bool *ok=NULL);
    uint16_t readInt16(bool *ok=NULL);
    uint32_t readInt32(bool *ok=NULL);
    void readFully(byte *buff, size_t len, bool *ok=NULL);
};

#endif // __DATA_STREAM_H__
