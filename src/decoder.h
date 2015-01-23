#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <stdio.h>
#include <packet.h>

uint8_t * data_buffer;

void decode_using_packet(int idx, FILE * input_stream);
void decode_fixed_packet(int pkt_idx, FILE * input_stream);
void decode_calculated_packet(int pkt_idx, FILE * input_stream);

int find_frame_id(struct packet * p, int frame_byte, int frame_byte_sz,int timeout,FILE * input_stream);
void buffer_to_type_str(uint8_t * buf, int buf_idx,char * type_str, struct type * t, int dw);
void print_packet_as_json(int pkt_idx);

void calculate_crc(struct packet * p, struct poption * o);
#endif