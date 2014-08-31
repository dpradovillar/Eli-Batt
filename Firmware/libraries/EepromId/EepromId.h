#ifndef __EEPROM_ID_H__
#define __EEPROM_ID_H__

#include "Arduino.h"

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
class EepromId {
private:
	static char toUpper(char c);

public:
	/**
	 * Creates an object that can read, write and update the internal EEPROM memory, with a value
	 * for the unique serial number for a microcontroller.
	 * <p/>
	 * By default uses a length of 8  hexadecimal ascii characters. See the top of this file for
	 * other parameters.
	 */
	EepromId();

	/**
	 * Reads the serial number from EEPROM and returns it. In case of any error, checksum invalid,
	 * can't read, etc, ID_UNKNOW is returned instead.
	 */
	String read();

	/**
	 * Writes the serial number to the EEPROM and returns true if everything went ok. The first
	 * ID_LENGTH or fewer characters are taken.
	 * <p/>
	 * Returns true if the value was written correctly, otherwise false.
	 */
	bool write(const String &newId);

	/**
	 * Updates the serial number in the EEPROM by first looking at its value, and performing the
	 * write operation only if the value is different. This is the recommended way of writing values
	 * to the EEPROM memory.
	 */
	bool update(const String &newId);
};

#endif // __EEPROM_ID_H__
