#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include "Arduino.h"

#include <ArduinoSoftwareSerial.h>

#define CR_LF "\r\n"

/**
 * Specialization of the Endpoint class, that supports communication from a Serial connection.
 * Automatically detects if it's a hardware or software serial connection by looking at the pins
 * used to initialize the object.
 */
class SerialEndpoint {
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
    size_t print(uint16_t x);
    size_t print(uint32_t x);
    size_t print(char c);
    size_t print(const char *s);
    size_t print(char *s, int len);
    size_t print(const __FlashStringHelper *s);

    size_t println();
    size_t println(int x);
    size_t println(uint16_t x);
    size_t println(uint32_t x);
    size_t println(char x);
    size_t println(const char *s);
    size_t println(char *s, int len);
    size_t println(const __FlashStringHelper *s);
};

#endif // __ENDPOINT_H__
