#ifndef __SIMPLE_CRC_H__
#define __SIMPLE_CRC_H__

#include "Arduino.h"

/**
 * Provides easy-to-use utilitary functions for checksum calculation.
 */
class SimpleCrc {
public:
    /**
     * Taken from: http://stackoverflow.com/a/23726131, slightly modified to operate on Arduino's
     * builtin datatypes.
     */
    static uint16_t crc16(byte *inputBuffer, size_t len);

    /**
     * Taken from: http://en.wikipedia.org/wiki/Fletcher's_checksum, slightly modified to operate on
     * Arduino's builtin datatypes.
     */
    static uint16_t fletcher16(byte *inputBuffer, size_t len);
};

#endif // __SIMPLE_CRC_H__
