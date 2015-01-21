#include "decoder.h"
#include "packet.h"


void decode_using_packet(int pkt_idx, FILE * input_stream) {
	if (input_stream == NULL) {
	  fprintf(stderr, "Can't open input file!\n");
	  exit(1);
	}
	
    struct packet *p = &packet_list[pkt_idx];

    switch(p->ptype) {
        default:
        case PT_FIXED:
			decode_fixed_packet(pkt_idx, input_stream);
            break;
        case PT_DYNAMIC:
            //decode_dynamic_packet(p, input_stream, pdata);
            break;
        case PT_CALCULATED:
            //decode_calculated_packet(p, input_stream, pdata);
            break;
    }
}

void decode_fixed_packet(int pkt_idx, FILE * input_stream) {
    struct packet *p = &packet_list[pkt_idx];
	printf("Fixed packet size: %d", p->size);
	
	// Check if there are any magic packet numbers
    if (packet_has_option_type(pkt_idx,O_FRAME)) {

    } else { // No magic numbers read simple packet
		packet_buffer = malloc(p->size);
	
		read(input_stream, packet_buffer, p->size);
		
		
	
		free(packet_buffer);
    }
}