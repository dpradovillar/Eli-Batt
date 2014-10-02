#include <SdData.h>

SdNameSequencer::SdNameSequencer() :
        m_next(0) {
}

void SdNameSequencer::next(char *buff12bytes) {
    // Names of the files can't be more than 12 characters long.
    Utils::copyArray("FILE----.CSV", (byte*)buff12bytes, 12);
    Utils::toHex(buff12bytes+4, m_next++);
}

void SdNameSequencer::setStart(uint16_t start) {
    m_next = start;
}

SdWriter::SdWriter() :
    m_chip_select_pin(0),
    m_open(false)
{
}

bool SdWriter::setup(int chipSelectPin, SerialEndpoint *debugEndpoint) {
    m_chip_select_pin = chipSelectPin;
    d.setup(debugEndpoint);
#if TARGET_BOARD == TARGET_UNO
    pinMode(10, OUTPUT);
#endif
    d.print(F("  begining SD with pin:")).println((int)m_chip_select_pin);

    if (!SD.begin(m_chip_select_pin)) {
        d.println(F("Failed initialization of SD card. Check wiring."));
        return false;
    }

    uint16_t filesCount = countFilesInSd();
    d.print(F("  there are ")).print(filesCount).println(F(" files in the SD card."));
    m_sequence.setStart(filesCount+1);

    return true;
}

bool SdWriter::open() {
    if (m_open) {
        d.println(F("File already open, trying to close"));
        bool could = close();
        if(could) {
            d.println(F("It closed the file"));
        } else {
            d.println(F("Couldn't close the file, returning"));
            return false;
        }
    } else {
        d.println(F("File wasn't open, skipping closing"));
    }

    d.print(F("Getting next filename from sequence"));

    char buffer[13];
    m_sequence.next(buffer);
    buffer[12] = 0;

    d.print(F("New file open:")).println(buffer);

    m_file = SD.open(buffer, FILE_WRITE);
    m_open = (m_file ? true : false);

    d.println(m_open ? F("File open!") : F("File unopened"));

    return m_open;
}

bool SdWriter::isOpen() {
    return m_open;
}

bool SdWriter::close() {
    if(m_open) {
        m_file.close();
        m_open = false;
        d.println(F("Closing file."));
        return true;
    } else {
        d.println(F("File already closed! nothing to close"));
    }
    return false;
}

size_t SdWriter::write(char *s, size_t n) {
    return m_file.write((byte*)s, n);
}

size_t SdWriter::writeInt32(uint32_t id) {
    return m_file.print(id);
}

uint16_t SdWriter::countFilesInSd() {
    int count = 0;
    File root = SD.open("/");
    while (true) {
        File entry = root.openNextFile();
        if (!entry) {
            break;
        }
        if (!entry.isDirectory()) {
            count++;
        }
        entry.close();
    }
    return count;
}

void SdWriter::writeHeader(uint32_t *ids, byte len) {
    m_file.print("YYYYMMDDTHHMMSS,");
    for(byte i = 0; i<len; i++) {
        m_file.print(F(","));
        m_file.print(ids[i]);
    }
}

void SdWriter::writeDatetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t sec) {
    if (m_open) {
        char auxBuffer[5];

        // Year
        Utils::leftPad(year, auxBuffer, 4);
        auxBuffer[4] = 0;
        m_file.print(auxBuffer);

        // Month
        Utils::leftPad(month, auxBuffer, 2);
        auxBuffer[2] = 0;
        m_file.print(auxBuffer);

        // Day
        Utils::leftPad(day, auxBuffer, 2);
        m_file.print(auxBuffer);

        m_file.print(F("T"));

        // Hour
        Utils::leftPad(hour, auxBuffer, 2);
        m_file.print(auxBuffer);

        // Minute
        Utils::leftPad(minute, auxBuffer, 2);
        m_file.print(auxBuffer);

        // Second
        Utils::leftPad(sec, auxBuffer, 2);
        m_file.print(auxBuffer);
    } else {
        d.println(F("Throwing timestamp away because file is not open!"));
    }
}

void SdWriter::writeTuple(uint16_t temp, uint16_t current, uint16_t voltage) {
    if (m_open) {
        d.println(F("Writing to disk"));
        m_file.print(F(",")); m_file.print((unsigned int)temp);
        m_file.print(F(",")); m_file.print((unsigned int)current);
        m_file.print(F(",")); m_file.print((unsigned int)voltage);
    } else {
        d.println(F("Throwing data away because file is not open!"));
    }
}

void SdWriter::writeNewline() {
    if (m_open) {
        m_file.println();
    } else {
        d.println(F("Throwing CRLF away because file is not open!"));
    }
}

