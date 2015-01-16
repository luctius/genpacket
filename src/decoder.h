#ifndef DECODER_H
#define DECODER_H

uint8_t * packet_buffer;

void decode_using_packet(int idx, FILE * input_stream , struct packet_data * pdata);

#endif