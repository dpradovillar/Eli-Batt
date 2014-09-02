#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <ArduinoSoftwareSerial.h>
#include "Arduino.h"

#define CR_LF "\r\n"

/**
 * Interface to write/read bytes from a common endpoint.
 */
class Endpoint {
public:
    virtual ~Endpoint();
    virtual void flush() = 0;
    virtual int available() = 0;
    virtual void waitForConnection() = 0;

    virtual size_t write(byte b) = 0;
    virtual int read() = 0;

    size_t write(byte *arr, size_t len);
    size_t read(byte *arr, size_t len);

    virtual size_t write(byte *arr, size_t start, size_t len) = 0;
	virtual size_t read(byte *arr, size_t start, size_t len) = 0;
};

/**
 * Specialization of the Endpoint class, that supports communication from a Serial connection.
 * Automatically detects if it's a hardware or software serial connection by looking at the pins
 * used to initialize the object.
 */
class SerialEndpoint : public Endpoint {
private:
    int8_t m_hardwareSerialIndex;
    SoftwareSerial m_softwareSerial;
public:
    SerialEndpoint();

    void setup(int rxPin, int txPin, uint32_t baud);

    void flush();
    int available();
    void waitForConnection();

    size_t write(byte b);
    int read();

    size_t write(byte *arr, size_t start, size_t len);
    size_t read(byte *arr, size_t start, size_t len);

    size_t print(int x);
    size_t print(char c);
    size_t print(const char *s);
    size_t print(char *s, int len);

    size_t println();
    size_t println(int x);
    size_t println(char x);
    size_t println(const char *s);
    size_t println(char *s, int len);
};

#endif // __ENDPOINT_H__
