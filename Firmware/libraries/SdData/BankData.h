#ifndef __BANK_DATA_H_
#define __BANK_DATA_H_

#include <elibatt_config.h>

#include <DataStream.h>
#include <Debugger.h>
#include <AdafruitSD.h>
#include <SdData.h>
#include <Utils.h>

#define MAX_BOARDS 16

#define ID_TEMP 0
#define ID_CURR 1
#define ID_VOLT 2

/**
 * Gathers and synchronizes the data received from the bank communication. It's in charge of
 * determining when the data is flushed to its underlying sd writer. Has a hard limit of 64 ids.
 */
class BankData {
private:
	// Has a hard limit of 64 slave boards.

	// The size of the next chunk of data is calculated as follows:
	// 10*MAX_BOARDS+8+MAX_BOARDS/8
	// {
	uint32_t m_registered_ids[MAX_BOARDS];
	byte m_count_ids;
	uint16_t m_buffer[MAX_BOARDS][3]; //
	uint16_t m_bit_set; // For more than 16 or more than 32  boards, use uint32_t or uint64_t.

	// Timestamp of the last row written to the file.
	uint16_t m_year; uint8_t m_month; uint8_t m_day;    // Date: YYYY-MM-DD
	uint8_t m_hour; uint8_t m_minute; uint8_t m_second; // Time: HH:mm:ss
	// }

	SdWriter m_sd_writer;

	uint32_t m_current_duration;
	uint32_t m_file_duration;

	Debugger d;

private:
	int ready();
	byte findId(uint32_t id);

public:
	BankData();

	/**
	 * Writes a new line in the CSV file, even if it hasn't received data from all the registered
	 * devices.
	 */
    void forceFlush(char *buff13);

	bool setup(int chipSelectPin, uint32_t fileDuration, SerialEndpoint *dbgEndpoint);

	bool registerId(uint32_t id);
	void unregisterId(uint32_t id);

	byte registeredIdCount();
	uint32_t registeredIdAt(byte pos);

	void setTime(uint16_t year, uint8_t month, uint8_t day,  uint8_t hour, uint8_t minute, uint8_t sec);

	int addData(uint32_t id, uint16_t temp, uint16_t current, uint16_t voltage);
	int addData(uint32_t id, byte *buffer6bytes);
};

#endif // __BANK_DATA_H_
