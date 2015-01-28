#ifndef CRC_TOOLS
#define CRC_TOOLS

#include <stdint.h>
#include <stdio.h>
#include "crc_custom.h"

#define CRC8_POLY 0x07

uint64_t custom_crc(crc_cfg_t *cfg,uint8_t *data, size_t len);
uint8_t crc8(uint8_t *data, size_t len);
uint32_t crc32(const void *data, size_t len);


#endif
