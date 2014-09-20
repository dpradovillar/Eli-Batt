#include <BankData.h>

void BankData::forceFlush() {
	if (!m_sd_writer.isOpen()) {
		m_sd_writer.open();
		d.println("Writing header into blank file!");
		m_sd_writer.writeHeader(m_registered_ids, m_count_ids);
	}

	if (m_bit_set == 0) {
		d.println("None of the registered devices have data, consecutive forceFlush()'es");
		d.println(" will be omitted (no empty rows on the resulting file).");
		return;
	}
	d.println("Flushing data of slaves into disk!");
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

bool BankData::ready() {
     for (byte i=0; i<m_count_ids; i++) {
         if ( ! (m_bit_set & (1UL << i) ) ) {
             return false;
         }
     }
    return true;
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

    if (m_sd_writer.setup(chipSelectPin, dbgEndpoint)) {

    } else {
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
    	d.print("Registering slave with ID:").printHexInt(buff).println();
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

void BankData::setTime(uint16_t year, uint8_t month, uint8_t day,  uint8_t hour, uint8_t minute, uint8_t second) {
    // set the date/time for the current row
    m_year = year;
    m_month = month;
    m_day = day;

    m_hour = hour;
    m_minute = minute;
    m_second = second;
}

void BankData::addData(uint32_t id, uint16_t temp, uint16_t current, uint16_t voltage) {
    byte pos = findId(id);
    if (pos == 255) {
        byte buffer[4];
        Utils::toByte(id, buffer);
        d.print("Warning, id: ").printHexInt(buffer).println(" not found!");
    } else {
        m_buffer[pos][ID_TEMP] = temp;
        m_buffer[pos][ID_CURR] = current;
        m_buffer[pos][ID_VOLT] = voltage;
        m_bit_set |= (1UL << pos);
    }

    if (ready()) {
        forceFlush();
    }
}

