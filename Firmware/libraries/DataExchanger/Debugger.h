#ifndef __DEBUGGER_H_
#define __DEBUGGER_H_

#include "Arduino.h"

#include <Endpoint.h>
#include <Utils.h>

/**
 * Wrapper for the Endpoint class that allows to optionally direct output to a given endpoint.
 * <p/>
 * All calls to this class are delegated into an (optional) instance set up before doing the calls.
 */
class Debugger {
private:
    SerialEndpoint *m_endpoint;
public:
    Debugger();

    void setup(SerialEndpoint *endpoint);

    Debugger& write(byte b);
    Debugger& write(byte *arr, size_t len);
    Debugger& write(byte *arr, size_t start, size_t len);

    Debugger& print(int x);
    Debugger& print(uint16_t x);
    Debugger& print(uint32_t x);
    Debugger& print(char c);
    Debugger& print(const char *s);
    Debugger& print(char *s, int len);

    Debugger& println();
    Debugger& println(int x);
    Debugger& println(uint16_t x);
    Debugger& println(uint32_t x);
    Debugger& println(char x);
    Debugger& println(const char *s);
    Debugger& println(char *s, int len);

    Debugger& printInt(byte *buff4bytes);
    Debugger& printHexUInt16(uint16_t value);
    Debugger& printHexUInt16(byte *buff2bytes);
    Debugger& printHexInt(byte *buff4bytes);
};

#endif // __DEBUGGER_H_
