#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include "Arduino.h"

#include <ArduinoSoftwareSerial.h>

#define CR_LF "\r\n"

// To enable it on the ATMEGA1280
#define __AVR_ATmega1280__ true

/**
 * Supports communication from a Serial connection. Automatically detects if it's a hardware or software serial
 * connection by looking at the pins used to initialize the object.
 */
class SerialEndpoint {
private:
    int8_t m_hardwareSerialIndex;
    SoftwareSerial m_softwareSerial;
    SerialEndpoint *m_secondaryOutput;

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
    size_t print(float f, int decimals=4);
    size_t print(const char *s);
    size_t print(char *s, int len);
    size_t print(const __FlashStringHelper *s);
    size_t printFloating(int integral_part, int decimal_part);
    size_t printSimpleDate(int year, int month, int day);
    size_t printSimpleTime(int hour, int minute, int second);

    size_t println();
    size_t println(int x);
    size_t println(uint16_t x);
    size_t println(uint32_t x);
    size_t println(char x);
    size_t println(float f, int decimals=4);
    size_t println(const char *s);
    size_t println(char *s, int len);
    size_t println(const __FlashStringHelper *s);
    size_t printlnFloating(int integral_part, int decimal_part);
    size_t printlnSimpleDate(int year, int month, int day);
    size_t printlnSimpleTime(int hour, int minute, int second);
};

/**
 * Composite pattern for SerialEndpoint that allows to send data to 2 different parallel Serial connections, using for 
 * both the same SerialEndpoint interface specification. Beware that bytes will be read only from one SerialEndpoint,
 * as it's complex to merge bytes coming from 2 sources.
 */
class DualSerialEndpoint {
private:
    SerialEndpoint *m_a;
    SerialEndpoint *m_b;

public:
    DualSerialEndpoint();

    /**
     * The first SerialEnpoint connection will perform read/write operations while the second one only write operations.
     * This is useful for sending debug and monitoring data to a secondary source that will only consume bytes (not send
     * a thing back to this).
     */
    void setup(SerialEndpoint *readWrite,
            SerialEndpoint *writeOnly=NULL);

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
    size_t printFloating(int integral_part, int decimal_part);
    size_t printSimpleDate(int year, int month, int day);
    size_t printSimpleTime(int hour, int minute, int second);

    size_t println();
    size_t println(int x);
    size_t println(uint16_t x);
    size_t println(uint32_t x);
    size_t println(char x);
    size_t println(const char *s);
    size_t println(char *s, int len);
    size_t println(const __FlashStringHelper *s);
    size_t printlnFloating(int integral_part, int decimal_part);
    size_t printlnSimpleDate(int year, int month, int day);
    size_t printlnSimpleTime(int hour, int minute, int second);
};

#endif // __ENDPOINT_H__
