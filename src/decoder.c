#include "decoder.h"
#include "packet.h"
#include "debug_print.h"

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
	gp_debug("Fixed packet size: %d", p->size);
    packet_buffer = malloc(p->size);
    
    if (packet_has_option_type(pkt_idx,O_FRAME)) {
        gp_debug("Framed packet type");
        
        // Frame stuff
        int frame_opt_idx = next_otype(p, O_FRAME,0);
        struct poption *frame_id = &p->option_list[frame_opt_idx];
        int frame_byte = v_get_i(frame_id->frame_val);
        
        gp_debug("Frame data width: %d",frame_id->data_width);
        gp_debug("Serching for frame 0x%x", frame_byte);
        
        fread(packet_buffer, p->size,1 ,input_stream);
        
        int frame_byte_sz = frame_id->data_width/8;
        int timeout = 10;
        int frame_at_idx = find_frame_id(p, frame_byte, frame_byte_sz,timeout);
        
        if (frame_at_idx >= 0) {
            gp_debug("Found frame at index: %d", frame_at_idx);
            packet_buffer = realloc(packet_buffer,p->size + frame_at_idx);
            // Read rest of packet
            fread(&packet_buffer[p->size - frame_at_idx], p->size - frame_at_idx, 1,input_stream);
            
            // Print packet
        	for(size_t i = frame_at_idx; i < p->size; ++i) {
                gp_debug("%d: %x",i,packet_buffer[i]);
        	}
        }
        
    }
    
    
	

	//fread(packet_buffer, p->size, 1,input_stream);
	
	
	// Check if there are any magic packet numbers
    if(0) { // No magic numbers read simple packet
		packet_buffer = malloc(p->size);
	
		fread(packet_buffer, p->size,1 ,input_stream);
		
		for(size_t i = 0; i < p->size; ++i) {
            gp_debug("%d: %x",i,packet_buffer[i]);
		    /* code */
		}
	
		
    }
    free(packet_buffer);
}


int find_frame_id(struct packet * p, int frame_byte, int frame_byte_sz,int timeout) {
    while(timeout--) {
        int num_checked = 0;
        for(size_t i = 0; i < p->size; ++i) {
            for(size_t j = 0; j < frame_byte_sz; ++j) {
                if (packet_buffer[i] == ((uint8_t*)&frame_byte)[j]) {
                    num_checked++;
                    if (num_checked == frame_byte_sz) {
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}