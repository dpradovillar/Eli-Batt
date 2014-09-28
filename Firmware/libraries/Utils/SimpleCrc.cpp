#include <SimpleCrc.h>

uint16_t SimpleCrc::crc16(byte *inputBuffer, size_t len) {
    byte x;
    uint16_t crc = 0xFFFF;
    uint16_t i = 0;
    while (len--) {
        x = crc >> 8 ^ inputBuffer[i++];
        x ^= x >> 4;
        crc = (crc << 8) ^ ((uint16_t) (x << 12)) ^ ((uint16_t) (x << 5)) ^ ((uint16_t) x);
    }
    return crc;
}

uint16_t SimpleCrc::fletcher16(byte *inputBuffer, size_t len) {
    uint16_t sum1 = 0xff, sum2 = 0xff;
    uint16_t i = 0;
    while (len) {
        size_t tlen = len > 20 ? 20 : len;
        len -= tlen;
        do {
            sum2 += sum1 += inputBuffer[i++];
        } while (--tlen);
        sum1 = (sum1 & 0xff) + (sum1 >> 8);
        sum2 = (sum2 & 0xff) + (sum2 >> 8);
    }
    /* Second reduction step to reduce sums to 8 bits */
    sum1 = (sum1 & 0xff) + (sum1 >> 8);
    sum2 = (sum2 & 0xff) + (sum2 >> 8);
    return sum2 << 8 | sum1;
}
