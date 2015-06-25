#include <SdData.h>

SdWriter::SdWriter() :
    m_chip_select_pin(0),
    m_open(false),
    m_ok(false)
{
}

bool SdWriter::setup(int chipSelectPin, SerialEndpoint *debugEndpoint) {
    m_chip_select_pin = chipSelectPin;
    d.setup(debugEndpoint);
#ifdef OUTPUT_TO_SERIAL1
    Serial.print("  begining SD with pin:");
    Serial.println((int)m_chip_select_pin);
#endif

    pinMode(SS, OUTPUT);

    pinMode(chipSelectPin, OUTPUT);
    if (!SD.begin(m_chip_select_pin)) {
#ifdef OUTPUT_TO_SERIAL1
        Serial.println("Failed initialization of SD card. Check wiring.");
#endif
        return (m_ok = false);
    }

    uint16_t filesCount = countFilesInSd();
#ifdef OUTPUT_TO_SERIAL1
    Serial.print("  there are ");
    Serial.print(filesCount);
    Serial.println(" files in the SD card.");
#endif
    //m_sequence.setStart(filesCount+1);

    return (m_ok = true);
}

bool SdWriter::open(char *filename_buff13) {
    if (m_open) {
#ifdef OUTPUT_TO_SERIAL1
        Serial.println("File already open, trying to close");
#endif
        bool could = close();
#ifdef OUTPUT_TO_SERIAL1
        if(could) {
            Serial.println("It closed the file");
        } else {
            Serial.println("Couldn't close the file, returning");
            return false;
        }
#else
        if (!could) {
            return false;
        }
#endif
    }
#ifdef OUTPUT_TO_SERIAL1
    else {
        Serial.println("File wasn't open, skipping closing");
    }

    Serial.print("Getting next filename from sequence");
#endif
    filename_buff13[12] = 0;

#ifdef OUTPUT_TO_SERIAL1
    Serial.print("New file open:");
    Serial.println(filename_buff13);
#endif

    if (m_ok) {
        m_file = SD.open(filename_buff13, FILE_WRITE);
        m_open = (m_file ? true : false);
    } else {
        m_open = true;
    }

#ifdef OUTPUT_TO_SERIAL1
    Serial.println(m_open ? F("File open!") : F("File unopened"));
#endif

    return m_open;
}

bool SdWriter::isOpen() {
    return m_open;
}

bool SdWriter::close() {
    if (m_open) {
        if (m_ok) {
            m_file.close();
        }
        m_open = false;
#ifdef OUTPUT_TO_SERIAL1
        Serial.println("Closing file.");
#endif
        return true;
    }
#ifdef OUTPUT_TO_SERIAL1
    else {
        Serial.println("File already closed! nothing to close");
    }
#endif
    return false;
}

size_t SdWriter::write(char *s, size_t n) {
    return m_ok ? m_file.write((byte*)s, n) : 0;
}

size_t SdWriter::writeInt32(uint32_t id) {
    return m_ok ? m_file.print(id) : 0;
}

size_t SdWriter::writeFloat(float f) {
    return m_ok ? m_file.print(f) : 0;
}

size_t SdWriter::writeChar(char c) {
    return m_ok ? m_file.print(c) : 0;
}

uint16_t SdWriter::countFilesInSd() {
    if (m_ok) {
        return 0;
    }
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
    if (m_ok) {
        m_file.print(F("YYYYMMDDTHHMMSS,"));
        for(byte i = 0; i<len; i++) {
            m_file.print(F(","));
            m_file.print(ids[i]);
        }
    }
}

void SdWriter::writeDatetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t sec) {
    if (m_ok) {
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
        }
#ifdef OUTPUT_TO_SERIAL1
        else {
            Serial.println("Throwing timestamp away because file is not open!");
        }
#endif
    }
#ifdef OUTPUT_TO_SERIAL1
    else {
        Serial.println("Throwing timestamp away because SD has errors.");
    }
#endif
}

void SdWriter::writeTuple(uint16_t temp, uint16_t current, uint16_t voltage) {
    if (m_ok) {
        if (m_open) {
#ifdef OUTPUT_TO_SERIAL1
            Serial.println("Writing to disk");
#endif
            m_file.print(F(",")); m_file.print((unsigned int)temp);
            m_file.print(F(",")); m_file.print((unsigned int)current);
            m_file.print(F(",")); m_file.print((unsigned int)voltage);
        }
#ifdef OUTPUT_TO_SERIAL1
        else {
            Serial.println("Throwing data away because file is not open!");
        }
#endif
    }
#ifdef OUTPUT_TO_SERIAL1
    else {
        Serial.println("Throwing data away because SD has errors.");
    }
#endif
}

void SdWriter::writeNewline() {
    if (m_ok) {
        if (m_open) {
            m_file.println();
        }
#ifdef OUTPUT_TO_SERIAL1
        else {
            Serial.println("Throwing CRLF away because file is not open!");
        }
#endif
    }
#ifdef OUTPUT_TO_SERIAL1
    else {
        Serial.println("Throwing CRLF away because SD has errors.");
    }
#endif
}

