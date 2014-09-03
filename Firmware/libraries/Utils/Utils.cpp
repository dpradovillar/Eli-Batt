#include <Utils.h>

void Utils::copyArray(byte *src, byte *dst, size_t len) {
    while(len-- > 0) {
        *dst++ = *src++;
    }
}
void Utils::copyArray(const char *src, byte *dst, size_t len) {
	size_t i = 0;
    while(i < len) {
        dst[i] = src[i];
    }
}

uint16_t Utils::toShort(byte hi, byte lo) {
    return (uint16_t)(((((uint16_t)hi) << 8) | (((uint16_t)lo) << 0)) & 0xFFFF);
}
void Utils::toByte(uint16_t x, byte *buffer) {
    buffer[0] = (byte)(x >> 8);
    buffer[1] = (byte)(x >> 0);
}

uint32_t Utils::toInt32(byte *buff) {
    return (uint32_t)(
    ((uint32_t)buff[0]) << 24 |
    ((uint32_t)buff[1]) << 16 |
    ((uint32_t)buff[2]) << 8 |
    ((uint32_t)buff[3]) << 0);
}

void Utils::toByte(uint32_t x, byte *buff) {
    buff[0] = (byte)(x >> 24);
    buff[1] = (byte)(x >> 16);
    buff[2] = (byte)(x >> 8);
    buff[3] = (byte)(x >> 0);
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

void Utils::toHex(char *buffer, byte value) {
    byte hi = value / 16;
    byte lo = value % 16;
    buffer[0] = (hi < 10 ? '0'+hi : 'A'+(hi-10));
    buffer[1] = (lo < 10 ? '0'+lo : 'A'+(lo-10));
}
