#ifndef PACKAGE_PRINTER_H
#define PACKAGE_PRINTER_H
#include <stdio.h>
#include <stdint.h>
#include "packet.h"

void print_packet_as_json(int pkt_idx);
void buffer_to_json_type_str(uint8_t * buf, int buf_idx,char * type_str, struct type * t, int dw);

#endif
