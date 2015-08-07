#include <Utils.h>

#define TO_HEX(c) ((c)<10 ? '0'+(c) : 'A'+((c)-10))

void Utils::copyArray(byte *src, byte *dst, size_t len) {
    while(len-- > 0) {
        *dst++ = *src++;
    }
}

void Utils::copyArray(const char *src, byte *dst, size_t len) {
    size_t i = 0;
    while(i < len) {
        dst[i] = src[i];
        i++;
    }
}

void Utils::toByte(uint16_t x, byte *buffer2bytes) {
    buffer2bytes[0] = (byte)(x >> 8);
    buffer2bytes[1] = (byte)(x >> 0);
}

uint32_t Utils::toInt32(const char *buffer4bytes) {
    return toInt32((byte*)buffer4bytes);
}

uint32_t Utils::toInt32(byte *buffer4bytes) {
    return (uint32_t)(
    ((uint32_t)buffer4bytes[0]) << 24 |
    ((uint32_t)buffer4bytes[1]) << 16 |
    ((uint32_t)buffer4bytes[2]) << 8 |
    ((uint32_t)buffer4bytes[3]) << 0);
}

uint16_t Utils::toUInt16(byte *buffer2bytes) {
	return (uint16_t)(
	((uint16_t)buffer2bytes[0]) << 8 |
	((uint16_t)buffer2bytes[1]) << 0);
}

void Utils::toByte(uint32_t x, byte *buffer4bytes) {
    buffer4bytes[0] = (byte)(x >> 24);
    buffer4bytes[1] = (byte)(x >> 16);
    buffer4bytes[2] = (byte)(x >> 8);
    buffer4bytes[3] = (byte)(x >> 0);
}

bool Utils::arrayEquals(byte *src1, byte *src2, size_t len) {
    for(size_t i = 0; i < len; i++) {
        if (src1[i] != src2[i]) {
            return false;
        }
    }
    return true;
}

bool Utils::arrayEquals(const char *src1, byte *src2, size_t len) {
    return arrayEquals(src2, src1, len);
}

bool Utils::arrayEquals(byte *src2, const char *src1, size_t len) {
    return arrayEquals(src2, (byte*)src1, len);
}

void Utils::toHex(char *buffer2bytes, byte value) {
    buffer2bytes[0] = TO_HEX(value / 16);
    buffer2bytes[1] = TO_HEX(value % 16);
}

void Utils::toHex(char *buffer4bytes, uint16_t value) {
    toHex(buffer4bytes,   (byte)(value >>  8));
    toHex(buffer4bytes+2, (byte)(value >>  0));
}

void Utils::toHex(char *buffer8bytes, uint32_t value) {
    toHex(buffer8bytes,   (byte)(value >> 24));
    toHex(buffer8bytes+2, (byte)(value >> 16));
    toHex(buffer8bytes+4, (byte)(value >>  8));
    toHex(buffer8bytes+6, (byte)(value >>  0));
}

void Utils::putInMemory(byte *mem, uint16_t address, uint16_t value) {
    toByte(value, mem+address);
}

void Utils::onFailure(int dbg_pin, const __FlashStringHelper *err, bool doLog) {
    // TODO(rtapiapincheira): log err to the SD card
#define DOT_DURATION    100
#define DASH_DURATION   300
#define SLEEP_DURATION 5000

    digitalWrite(dbg_pin, LOW);
    char i;
    while(true) {
        for (i = 0; i < 3; i++) {
            digitalWrite(dbg_pin, HIGH);
            delay(DOT_DURATION);
            digitalWrite(dbg_pin, LOW);
            delay(DOT_DURATION);
        }
        delay(150);
        for (i = 0; i < 3; i++) {
            digitalWrite(dbg_pin, HIGH);
            delay(DASH_DURATION);
            digitalWrite(dbg_pin, LOW);
            delay(DASH_DURATION);
        }
        delay(150);
        for (i = 0; i < 3; i++) {
            digitalWrite(dbg_pin, HIGH);
            delay(DOT_DURATION);
            digitalWrite(dbg_pin, LOW);
            delay(DOT_DURATION);
        }
        delay(SLEEP_DURATION);
    }
}

void Utils::leftPad(uint32_t value, char *buffLenBytes, int len, char toPad) {
	int last = len-1;
	while(value > 0 && len > 0) {
		char lastDig = value%10 + '0';
		value /= 10;
		buffLenBytes[len-1] = lastDig;
		--len;
	}
	while(len > 0) {
		buffLenBytes[len-1] = toPad;
		--len;
	}
}

void Utils::splitFloat(float value, uint32_t &_int, uint32_t &_dec) {
    _int = (uint32_t)value;
    _dec = FLOAT_TO_INT_FACTOR*(value - _int);
}


