#include <EepromId.h>

void EepromId::read(byte *buff) {
    byte addr = ID_EEPROM_OFFSET;
    byte hiCrc = EEPROM.read(addr++);
    byte loCrc = EEPROM.read(addr++);
    uint16_t eepromCrc = Utils::toShort(hiCrc, loCrc);

    for(byte i = 0; i < ID_LENGTH; i++){
        buff[i] = EEPROM.read(addr++);
    }

    uint16_t crc = SimpleCrc::crc16(buff, ID_LENGTH);
    if (crc != eepromCrc) {
        Utils::copyArray(ID_UNKNOWN, buff, ID_LENGTH);
        return;
    }
}

bool EepromId::write(byte *newId) {
    byte addr = ID_EEPROM_OFFSET;
    // Calculate the checksum for the id being written
    uint16_t crcToWrite = SimpleCrc::crc16(newId, ID_LENGTH);
    byte hi, lo;
    Utils::toByte(crcToWrite, &hi, &lo);

    // Write into first 2 bytes of the EEPROM, the 2 bytes of the crc
    EEPROM.write(addr++, hi);
    EEPROM.write(addr++, lo);

    // write the ID_LENGTH bytes of the id.
    for(int i = 0; i < ID_LENGTH; i++) {
        EEPROM.write(addr++, newId[i]);
    }
    return true;
}

bool EepromId::update(byte *newId) {
    byte eId[ID_LENGTH];
    for(byte i = 0; i < ID_LENGTH; i++){
        eId[i] = EEPROM.read(ID_EEPROM_OFFSET+2+i);
        Serial.print("read from eeprom:");
        Serial.println(eId[i], DEC);
    }
    Serial.println("Checking unequality");
    if (!Utils::arrayEquals(eId, newId, ID_LENGTH)) {
    	Serial.println("not equals!");
        //return write(newId);
    } else {
    	Serial.println("equals!");
    }
    return false;
}

bool EepromId::writeId(uint32_t id) {
    byte buffer[4];
    Utils::toByte(id, buffer);
    return update(buffer);
}
uint32_t EepromId::readId() {
    byte buffer[4];
    //read(buffer);
    return Utils::toInt32(buffer);
}
