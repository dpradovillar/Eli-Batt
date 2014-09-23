#ifndef __DATA_STREAM_H__
#define __DATA_STREAM_H__

#include <Endpoint.h>
#include "Arduino.h"

class DataStreamWriter;
class DataStreamReader;

/**
 * Interface that specifies a custom object definition that knows how to write itself to an stream
 * and knows how to read itself back. This way we can achieve general purpose transport mechanisms
 * by separating the data (re)construction from the sending mechanism.
 */
/*class DataObject {
public:
    virtual ~DataObject();
    virtual size_t writeTo(DataStreamWriter *dsw) = 0;
    virtual size_t writeAsciiTo(DataStreamWriter *dsw) = 0;
    virtual size_t readFrom(DataStreamReader *dsr) = 0;
    virtual uint16_t calculateCrc() = 0;
};*/

/**
 * Wraper for an endpoint, to write abstract datatypes. This class is in charge of the serialization
 * to bytes of the most common datatypes in the C++ language and provides an extensible way of
 * writing custom types.
 */
class DataStreamWriter {
private:
    SerialEndpoint *m_endpoint;
public:
    void setup(SerialEndpoint *endpoint);
    void flush();

    size_t writeByte(byte b);
    size_t writeShort(short s);
    size_t writeString(const String &s);
    size_t writeArray(byte *s, size_t n);
    //size_t writeObject(DataObject *obj);
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
    void setup(SerialEndpoint *endpoint);
    int available();

    byte readByte(bool *ok=NULL);
    short readShort(bool *ok=NULL);
    int32_t readInt(bool *ok=NULL);
    void readFully(byte *buff, size_t len, bool *ok=NULL);
    //void readObject(DataObject *obj, bool *ok=NULL);
};

#endif // __DATA_STREAM_H__
