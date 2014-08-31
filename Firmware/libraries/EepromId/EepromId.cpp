#include <EepromId/EepromId.h>

EepromId::EepromId() {
}

String EepromId::read() {
	byte addr = ID_EEPROM_OFFSET;
	byte hiCrc = EEPROM.read(addr++);
	byte loCrc = EEPROM.read(addr++);

	byte len   = EEPROM.read(addr++);
	if (len > 12){
		return ID_UNKNOWN;
	}
	String result;
	result.reserve(len);
	for(int i = 0; i < len; i++){
		result.setCharAt(i, EEPROM.read(addr++);
	}
	result = result.substring(0, len);

	// Only check checksum, if the crc module is enabled.
	byte buffer[2];
	SimpleCrc(result).getCrc16(buffer);
	if (buffer[0]!=hiCrc || buffer[1]!=loCrc) {
		return ID_UNKNOWN;
	}
	return result;
}

bool EepromId::write(const String &newId) {
	// Make a serial of the form '0000000...000'
	String toWrite;
	toWrite.reserve(ID_LENGTH);

	// Put the first ID_LENGTH or less characters of newId into toWrite, aligned to the right. For
	// example, if newId is "13b4", then, the resulting toWrite will be "??...??13B4"
	int len = min(ID_LENGTH, newId.length());
	int toPad = ID_LENGTH-len; // Number of trailing zeros at the left.
	for (int i = 0; i < len; i++){
		toWrite.setCharAt(toPad+i, toUpper(newId[i]));
	}
	// Replace the first characters with zeros.
	for(int i = 0; i < toPad; i++) {
		toWrite.setCharAt(i, '0');
	}

	byte addr = ID_EEPROM_OFFSET;
	// Calculate the checksum for the id being written
	byte buffer[2];
	SimpleCrc(toWrite).getCrc16(buffer);

	// Write into first 3 bytes of the EEPROM, the 2 bytes of the crc
	EEPROM.write(addr++, buffer[0]);
	EEPROM.write(addr++, buffer[1]);
	// and the length of the id
	EEPROM.write(addr++, (byte)ID_LENGTH);

	// write the ID_LENGTH bytes of the id.
	for(int i = 0; i < ID_LENGTH; i++) {
		EEPROM.write(addr++, toWrite[i]);
	}
}

bool EepromId::update(const String &newId) {
	String toWrite = newId.length()>ID_LENGTH ? newId.substring(0, ID_LENGTH) : newId;
	if (read() != toWrite) {
		write(toWrite);
	}
}
