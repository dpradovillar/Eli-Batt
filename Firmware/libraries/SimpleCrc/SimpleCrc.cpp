#include <SimpleCrc/SimpleCrc.h>

SimpleCrc::SimpleCrc(const String &source) :
	m_source(source)
{
}

void SimpleCrc::getCrc16(byte buffer[2]) {
	uint16_t crc = crc16(m_source);
	buffer[1] = (crc >> 0) & 0xFF;
	buffer[0] = (crc >> 8) & 0xFF;
}

void SimpleCrc::getFletcher16(byte buffer[2]) {
	uint16_t crc = fletcher16(m_source);
	buffer[1] = (crc >> 0) & 0xFF;
	buffer[0] = (crc >> 8) & 0xFF;
}

uint16_t SimpleCrc::crc16(const String &data){
	byte x;
	uint16_t crc = 0xFFFF;
	uint16_t i = 0;
	uint16_t len = data.length();
	while (len--){
		x = crc >> 8 ^ data.charAt(i++);
		x ^= x>>4;
		crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
	}
	return crc;
}

uint16_t SimpleCrc::fletcher16(const String &data) {
	uint16_t sum1 = 0xff, sum2 = 0xff;
	uint16_t i = 0;
	uint16_t len = data.length();
	while (len) {
			len tlen = len > 20 ? 20 : len;
			len -= tlen;
			do {
				sum2 += sum1 += data.charAt(i++);
			} while (--tlen);
			sum1 = (sum1 & 0xff) + (sum1 >> 8);
			sum2 = (sum2 & 0xff) + (sum2 >> 8);
	}
	/* Second reduction step to reduce sums to 8 bits */
	sum1 = (sum1 & 0xff) + (sum1 >> 8);
	sum2 = (sum2 & 0xff) + (sum2 >> 8);
	return sum2 << 8 | sum1;
}
