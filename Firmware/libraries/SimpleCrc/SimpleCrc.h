#ifndef __SIMPLE_CRC_H__
#define __SIMPLE_CRC_H__

#include "ElibattFramework.h"

#if ENABLED_SIMPLE_CRC

/**
 * Provides easy-to-use utilitary functions for checksum calculation.
 */
class SimpleCrc {
private:
	/**
	 * Content that will be reduced to a checksum.
	 */
	String m_source;

public:
	/**
	 * Builds an object with the given content for the checksum calculation, by any algorithm.
	 */
	SimpleCrc(const String &source);

	/** Implementation of the crc16 CCITT algorithm. */
	void getCrc16(byte buffer[2]);

	/** Implementation of Fletcher's algorithm. */
	void getFletcher16(byte buffer[2]);

	/**
	 * Taken from: http://stackoverflow.com/a/23726131, slightly modified to operate on Arduino's
	 * builtin datatypes.
	 */
	static uint16_t crc16(const String &data);

	/**
	 * Taken from: http://en.wikipedia.org/wiki/Fletcher's_checksum, slightly modified to operate on
	 * Arduino's builtin datatypes.
	 */
	static uint16_t fletcher16(const String &data);
};

#endif // ENABLED_SIMPLE_CRC
#endif // __SIMPLE_CRC_H__
