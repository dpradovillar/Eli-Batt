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

uint16_t Utils::toShort(byte hi, byte lo) {
    return (uint16_t)(((((uint16_t)hi) << 8) | (((uint16_t)lo) << 0)) & 0xFFFF);
}

void Utils::toByte(uint16_t x, byte *buffer2bytes) {
	buffer2bytes[0] = (byte)(x >> 8);
	buffer2bytes[1] = (byte)(x >> 0);
}

uint32_t Utils::toInt32(byte *buffer4bytes) {
    return (uint32_t)(
    ((uint32_t)buffer4bytes[0]) << 24 |
    ((uint32_t)buffer4bytes[1]) << 16 |
    ((uint32_t)buffer4bytes[2]) << 8 |
    ((uint32_t)buffer4bytes[3]) << 0);
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

