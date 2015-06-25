#include <BankData.h>

void BankData::forceFlush(char *buff13) {
    if (!m_sd_writer.isOpen()) {
        buff13[12] = 0;
        m_sd_writer.open(buff13);
        d.println(F("Writing header into blank file!"));
        m_sd_writer.writeHeader(m_registered_ids, m_count_ids);
    }

    if (m_bit_set == 0) {
        d.println(F("None of the registered devices have data, consecutive forceFlush()'es"));
        d.println(F(" will be omitted (no empty rows on the resulting file)."));
        return;
    }
    d.println(F("Flushing data of slaves into disk!"));
    // Write timestamp
    m_sd_writer.writeNewline();
    m_sd_writer.writeDatetime(m_year, m_month, m_day, m_hour, m_minute, m_second);
    for (byte i=0; i<m_count_ids; i++) {
        m_sd_writer.writeTuple(m_buffer[i][ID_TEMP], m_buffer[i][ID_CURR], m_buffer[i][ID_VOLT]);
    }

    m_current_duration++;
    m_bit_set = 0;

    if (m_current_duration == m_file_duration) {
        // Force a renewal of the file!
        m_sd_writer.close();
        m_current_duration = 0;
    }
}

int BankData::ready() {
	int readyCount = 0;
     for (byte i=0; i<m_count_ids; i++) {
         if (m_bit_set & (1UL << i) ) {
             readyCount++;
         }
     }
    return readyCount;
}

byte BankData::findId(uint32_t id) {
    for(byte i=0; i<m_count_ids; i++) {
        if (m_registered_ids[i] == id) {
            return i;
        }
    }
    return (byte)255; // not found == -1
}

BankData::BankData() :
        m_count_ids(0),
        m_bit_set(0),

        m_year(0),
        m_month(0),
        m_day(0),

        m_hour(0),
        m_minute(0),
        m_second(0),

        m_current_duration(0),
        m_file_duration(0)
{
}

bool BankData::setup(int chipSelectPin, uint32_t fileDuration, SerialEndpoint *dbgEndpoint) {
    // setup underlying sd writer
    d.setup(dbgEndpoint);
    m_file_duration = fileDuration;

    if (!m_sd_writer.setup(chipSelectPin, dbgEndpoint)) {
        return false;
    }

    return true;
}

bool BankData::registerId(uint32_t id) {
    m_bit_set = 0;
    byte pos = findId(id);
    if (pos == 255) {
        byte buff[4];
        Utils::toByte(id, buff);
        if (m_count_ids == 0) {
            d.print(F("Registering master with ID ")).printHexInt(buff).println(F(": Ok"));
        } else {
            d.print(F("Registering slave with ID ")).printHexInt(buff).print(F(": Ok"));
        }
        m_registered_ids[m_count_ids++] = id;
        return true;
    }
    return false;
}

void BankData::unregisterId(uint32_t id) {
    if (m_count_ids > 0) {
        // search and delete the id from the list of available ids
        byte pos = findId(id);
        if (pos != 255) {
            m_registered_ids[pos] = m_registered_ids[m_count_ids-1];
            m_count_ids--;
        }
    }
    m_bit_set = 0;
}

byte BankData::registeredIdCount() {
	return m_count_ids;
}

uint32_t BankData::registeredIdAt(byte pos) {
	if (pos >= m_count_ids) {
		return -1;
	}
	return m_registered_ids[pos];
}

void BankData::setTime(uint16_t year, uint8_t month, uint8_t day,  uint8_t hour, uint8_t minute, uint8_t second) {
    // set the date/time for the current row
    m_year = year;
    m_month = month;
    m_day = day;

    m_hour = hour;
    m_minute = minute;
    m_second = second;

    d.print(F("Setting date:"))
        .print((int)year).print(F("/")).print((int)month).print(F("/")).print(day).print(F(" "))
        .print((int)hour).print(F(":")).print((int)minute).print(F(":")).println((int)second);
}

int BankData::addData(uint32_t id, uint16_t temp, uint16_t current, uint16_t voltage) {
    byte pos = findId(id);
    if (pos == 255) {
        byte buffer[4];
        Utils::toByte(id, buffer);
        d.print(F("Warning, id: ")).printHexInt(buffer).println(F(" not found!"));
    } else {
        m_buffer[pos][ID_TEMP] = temp;
        m_buffer[pos][ID_CURR] = current;
        m_buffer[pos][ID_VOLT] = voltage;
        m_bit_set |= (1UL << pos);
    }
	int readyCount = ready();
    if (readyCount == m_count_ids) {
        static char buff_filename[] = "xxxxxxxx.CSV";
        forceFlush(buff_filename);
    }
    return (m_count_ids - readyCount);
}

int BankData::addData(uint32_t id, byte *buffer6bytes) {
    return addData(id,
            Utils::toUInt16(buffer6bytes),
            Utils::toUInt16(buffer6bytes+2),
            Utils::toUInt16(buffer6bytes+4));
}


