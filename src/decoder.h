#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <stdio.h>
#include <packet.h>

uint8_t * packet_buffer;

void decode_using_packet(int idx, FILE * input_stream);
void decode_fixed_packet(int pkt_idx, FILE * input_stream);
int find_frame_id(struct packet * p, int frame_byte, int frame_byte_sz,int timeout);
#endif