#ifndef __SD_DATA_H_
#define __SD_DATA_H_

#include <DataStream.h>
#include <Debugger.h>
#include <AdafruitSD.h>
#include <Utils.h>

#include <elibatt_config.h>

/**
 * Wrapper for the built-in SD card library. It performs some application specific logic as dumping
 * messages as binary data, data compression, checksum verification and specific file handling.
 */
class SdWriter {
private:
    byte m_chip_select_pin;
    bool m_open;
    File m_file;
    Debugger d;

    bool m_ok;

public:
    SdWriter();

    /**
     * Specifies the pin used for selecting the sd card, as it uses the SPI interface and many other
     * devices may share the same SPI data bus. Returns true if the object is set up correctly so is
     * possible to write on the sd card.
     */
    bool setup(int chipSelectPin, SerialEndpoint *debugEndpoint=NULL);

    /**
     * Opens a file to write in the sd card with next name in a sequence. If a file was previously
     * open, its data are flushed and the file is closed before opening the new one. This guarantees
     * at most one file is open at any time. Returns true if the file was succesfully open (and
     * closed if any was already open) or false if it couldn't close the previously open file or it
     * couldn't open the new file.
     */
    bool open(char *filename_buff13);

    bool isOpen();

    /**
     * Flushes the data of any open file and closes it (if any is open). Returns true if the file
     * was successfully closed, or false it couldn't be closed or it was alread closed.
     */
    bool close();

    /**
     * Writes raw data directly into the open file. It returns the count of bytes written or 0, if
     * none were written or the file was unopened.
     */
    size_t write(char *s, size_t n);

    size_t writeInt32(uint32_t id);
    size_t writeFloat(float f);
    size_t writeChar(char c);

    /**
     * Returns the count of files in the SD.
     */
    uint16_t countFilesInSd();

    void writeHeader(uint32_t *ids, byte len);
    void writeDatetime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t sec);
    void writeTuple(uint16_t temp, uint16_t current, uint16_t voltage);
    void writeNewline();
};

/*class BinarySdWriter {
private:
    BinarySdWriter();

};*/

#endif // __SD_DATA_H_
