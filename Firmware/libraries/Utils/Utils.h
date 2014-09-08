#ifndef __H_UTILS_
#define __H_UTILS_

#include "Arduino.h"

class Utils {
public:
    /**
     * Copies the first len bytes from src, to the first len positions of dst.
     */
    static void copyArray(byte *src, byte *dst, size_t len);
    static void copyArray(const char *src, byte *dst, size_t len);

    static uint16_t toShort(byte hi, byte lo);
    static void toByte(uint16_t x, byte *buffer);

    static uint32_t toInt32(byte *buff);
    static void toByte(uint32_t x, byte *buffer4bytes);

    static bool arrayEquals(byte *src1, byte *src2, size_t len);
    static bool arrayEquals(const char *src1, byte *src2, size_t len);
    static bool arrayEquals(byte *src2, const char *src1, size_t len);

    static void toHex(char *buffer2bytes, byte value);
    static void toHex(char *buffer4bytes, uint16_t value);
    static void toHex(char *buffer8bytes, uint32_t value);
};

#endif // __H_UTILS_
