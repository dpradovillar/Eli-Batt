#include <EepromWriter.h>

void EepromWriter::read(byte *buff) {
    byte addr = ID_EEPROM_OFFSET;
    byte buffer[2];
	buffer[0] = EEPROM.read(addr++);
	buffer[1] = EEPROM.read(addr++);
    uint16_t eepromCrc = Utils::toUInt16(buffer);

    for(byte i = 0; i < ID_LENGTH; i++){
        buff[i] = EEPROM.read(addr++);
    }

    uint16_t crc = SimpleCrc::crc16(buff, ID_LENGTH);

    if (crc != eepromCrc) {
        Utils::copyArray(ID_UNKNOWN, buff, ID_LENGTH);
        return;
    }
}

bool EepromWriter::write(byte *newId) {
    byte addr = ID_EEPROM_OFFSET;
    // Calculate the checksum for the id being written
    uint16_t crcToWrite = SimpleCrc::crc16(newId, ID_LENGTH);

    byte buff[2];
    Utils::toByte(crcToWrite, buff);

    // Write into first 2 bytes of the EEPROM, the 2 bytes of the crc
    EEPROM.write(addr++, buff[0]);
    EEPROM.write(addr++, buff[1]);

    // write the ID_LENGTH bytes of the id.
    for(int i = 0; i < ID_LENGTH; i++) {
        EEPROM.write(addr++, newId[i]);
    }
    return true;
}

bool EepromWriter::update(byte *newId) {
    byte eId[ID_LENGTH];
    for(byte i = 0; i < ID_LENGTH; i++){
        eId[i] = EEPROM.read(ID_EEPROM_OFFSET+2+i);
    }
    if (!Utils::arrayEquals(eId, newId, ID_LENGTH)) {
        return write(newId);
    }
    return false;
}

bool EepromWriter::writeId(uint32_t id) {
    byte buffer[4];
    Utils::toByte(id, buffer);
    return update(buffer);
}
uint32_t EepromWriter::readId() {
    byte buffer[4];
    read(buffer);
    return Utils::toInt32(buffer);
}
