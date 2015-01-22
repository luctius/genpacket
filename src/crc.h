#ifndef CRC_TOOLS
#define CRC_TOOLS

#include <stdint.h>
#include <stdio.h>

uint8_t crc8_slow(uint32_t crc, uint8_t *data, size_t len);
uint8_t crc8(uint32_t crc, uint8_t *data, size_t len);
uint32_t crc32(uint32_t crc, const void *buf, size_t size);


#endif
