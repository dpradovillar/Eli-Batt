#ifndef __EEPROM_WRITER_H__
#define __EEPROM_WRITER_H__

#include "Arduino.h"

#include <SimpleCrc.h>
#include <EEPROM.h>
#include <Utils.h>

// This should'n be bigger than 8 or 12 bytes as the EEPROM is very small!
#define ID_LENGTH         4
#define ID_UNKNOWN        "\0\0\0\0"
#define ID_EEPROM_OFFSET  0

/**
 * Allows any application to retrieve, set and update an unique serial number into the Arduino's
 * builtin EEPROM memory.
 * <p/>
 * In the EEPROM, at ID_EEPROM_OFFSET position, can be found 2 bytes, for the CRC, 1 byte for the
 * length of the id (IDLEN) and IDLEN ascii characters in the range [0-9a-fA-F] for the unique id.
 * If any error occur, the checksum does not match, etc. ID_UNKNOW is returned instead.
 * <p/>
 * As the EEPROM memory manipulation is a heavy operation, it should be treated with care (writing
 * only at the setup) and using delays to avoid overloading the memory access).
 */
class EepromWriter {
public:
    /**
     * Reads the serial number from EEPROM and returns it. In case of any error, checksum invalid,
     * can't read, etc, ID_UNKNOW is returned instead.
     */
    void read(byte *buff);

    /**
     * Writes the serial number to the EEPROM and returns true if everything went ok. The first
     * ID_LENGTH or fewer characters are taken.
     * <p/>
     * Returns true if the value was written correctly, otherwise false.
     */
    bool write(byte *newId);

    /**
     * Updates the serial number in the EEPROM by first looking at its value, and performing the
     * write operation only if the value is different. This is the recommended way of writing values
     * to the EEPROM memory.
     */
    bool update(byte *newId);

    /**
     *
     */
    bool writeId(uint32_t id);

    /**
     *
     */
    uint32_t readId();
};

#endif // __EEPROM_WRITER_H__
