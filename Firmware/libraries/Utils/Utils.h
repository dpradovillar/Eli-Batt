#ifndef __H_UTILS_
#define __H_UTILS_

#include <Arduino.h>

#define FLOAT_TO_INT_FACTOR 1000000

class Utils {
public:
    static uint32_t pot10[];

    static uint8_t toHex(char hi, char lo);

    /**
     * Copies the first len bytes from src, to the first len positions of dst.
     */
    static void copyArray(byte *src, byte *dst, size_t len);
    static void copyArray(const char *src, byte *dst, size_t len);

    static void toByte(uint16_t x, byte *buffer);

    static uint32_t toInt32(const char *buffer4bytes);
    static uint32_t toInt32(byte *buff);
    static uint16_t toUInt16(byte *buffer2bytes);
    static void toByte(uint32_t x, byte *buffer4bytes);
    static uint32_t toInt32FromHex(const char *buffer8bytes);

    static bool arrayEquals(byte *src1, byte *src2, size_t len);
    static bool arrayEquals(const char *src1, byte *src2, size_t len);
    static bool arrayEquals(byte *src2, const char *src1, size_t len);

    static void toHex(char *buffer2bytes, byte value);
    static void toHex(char *buffer4bytes, uint16_t value);
    static void toHex(char *buffer8bytes, uint32_t value);

    /**
     * Put at the given address in the given memory pointer, the bytes representation of a given
     * value (by the data type of the value).
     * */
    static void putInMemory(byte *mem, uint16_t address, uint16_t value);

    /**
     * Stops the firmware execution and starts an LED error sequence (SOS).
     */
    static void onFailure(int dbg_pin, const __FlashStringHelper *err, bool doLog=false);

    static void leftPad(uint32_t value, char *buffLenBytes, int len, char toPad = '0');

    static void splitFloat(float value, uint32_t &_int, uint32_t &_dec);

    static void dateToIso(uint16_t year, uint8_t month, uint8_t day, char *buff8);
    static void timeToIso(uint8_t hour, uint8_t minute, uint8_t second, char *buff6);

    static int processInt(char *s, int len);
    static float processFloat(char *s, int len);
    static void processList(char *s, int len, int *firstElement, float *secondAndOn, int size, int *howMany);

    // buffer15bytes should be in the format yyyyMMddThhmmss
    static void parseIsoDate(char *buffer15bytes,
            uint16_t &year, uint8_t &month, uint8_t &day,
            uint8_t &hour, uint8_t &minute, uint8_t &second);
};

#endif // __H_UTILS_
