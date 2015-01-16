#include "decoder.h"
#include "packet.h"


void decode_using_packet(int idx, FILE * input_stream , struct packet_data * pdata) {
	if (input_stream == NULL) {
	  fprintf(stderr, "Can't open input file!\n");
	  exit(1);
	}
	
    struct packet *p = &packet_list[pkt_idx];

    switch(p->ptype) {
        default:
        case PT_FIXED:
			decode_fixed_packet(p, input_stream, pdata);
            break;
        case PT_DYNAMIC:
            //decode_dynamic_packet(p, input_stream, pdata);
            break;
        case PT_CALCULATED:
            //decode_calculated_packet(p, input_stream, pdata);
            break;
    }
}

void decode_fixed_packet(struct * packet, FILE * input_stream,struct packet_data * pdata) {
	printf("Fixed packet size: %d", p->size);
	
	// Check if there are any magic packet numbers
    if (packet_has_option_type(PO_FRAME)) {

    } else { // No magic numbers read simple packet
		packet_buffer = malloc(p->size);
	
		read(input_stream, packet_buffer, p->size);
		
		
	
		free(packet_buffer);
    }
}