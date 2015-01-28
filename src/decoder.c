#include "decoder.h"
#include "packet.h"
#include "debug_print.h"
#include <inttypes.h>
#include "crc.h"
#include "json_printer.h"

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
            //decode_dynamic_packet(pkt_idx, input_stream);
            break;
        case PT_CALCULATED:
            decode_calculated_packet(pkt_idx, input_stream);
            break;
    }
    
    print_packet_as_json(pkt_idx);
}

void decode_calculated_packet(int pkt_idx, FILE * input_stream) {
    struct packet *p = &packet_list[pkt_idx];
	gp_debug("Dynamic packet");
	
    //data_buffer = malloc(p->size);
    
    uint64_t byte_offset = 0;
    
    if (packet_has_option_type(pkt_idx,O_FRAME)) {
        gp_debug("Framed packet type");
        
        // Frame stuff
        int frame_opt_idx = next_otype(p, O_FRAME,-1);
        struct poption *frame_id = &p->option_list[frame_opt_idx];
        int frame_byte = v_get_i(frame_id->frame_val);
        
        gp_debug("Frame data width: %d",frame_id->data_width);
        
        // Update estimated size
        p->size = calculate_min_size(p);
        gp_debug("Dynamic packet new size: %d", p->size);
        
        gp_debug("Searching for frame 0x%x", frame_byte);
        
        gp_debug("Read %d bytes", p->size);
        data_buffer = malloc(p->size);
        
        int frame_byte_sz = frame_id->data_width/8;
        int timeout = 10;
        int frame_at_idx = find_frame_in_data(p, frame_byte, frame_byte_sz,timeout,input_stream);
        
        if (frame_at_idx >= 0) {
            gp_debug("Found frame at index: %d", frame_at_idx);
            data_buffer = realloc(data_buffer,p->size + frame_at_idx);
            
            gp_debug("Read %d bytes", frame_at_idx);
            // Read rest of packet
            fread(&data_buffer[p->size], frame_at_idx, 1,input_stream);
            
            p->data = (uint8_t *)malloc(p->size);
            memcpy(p->data,&data_buffer[frame_at_idx],p->size);
            
            free(data_buffer);
            // Print packet
        	for(int i = 0; i < p->size; ++i) {
                gp_debug("%d: %x",i,p->data[i]);
        	}
            // Update byte offset
            byte_offset = 0;
            struct poption *s_o = NULL;

            for (int idx = 0; idx < p->option_list_sz; idx++) {
        	    struct poption *o = &p->option_list[idx];
        		switch (o->otype) {
                    case O_SIZE:
                        o->data_byte_offset = byte_offset;
                        
                        p->size += p->data[o->data_byte_offset];
                        
                        p->data = realloc(p->data,p->size);
            
                        gp_debug("Read %d bytes", frame_at_idx);
                        // Read rest of packet
                        fread(&p->data[p->size - p->data[o->data_byte_offset]], p->data[o->data_byte_offset], 1,input_stream);
            
                        // Print packet
                    	for(int i = 0; i < p->size; ++i) {
                            gp_debug("%d: %x",i,p->data[i]);
                    	}
                        
                        gp_debug("Dynamic packet new size: %d", p->size);
                        
                        byte_offset += o->data_width/8;
                    break;
                    case O_DATA:
                        s_o = get_option_by_name(p,o->data_size_str);
                
                        o->data_size_i = p->data[s_o->data_byte_offset];
                        o->data_byte_offset = byte_offset;
                        gp_debug("Data %s new size: %d from: %s offset: %d",o->name, o->data_size_i,s_o->name,o->data_byte_offset);
                        
                        byte_offset += (o->data_width/8) * o->data_size_i;
                    break;
                    case O_CRC:
                        o->data_byte_offset = byte_offset;
                        calculate_crc(p,o);
                        byte_offset += o->data_width/8;
                    break;
                    default:
                        o->data_byte_offset = byte_offset;
                        byte_offset += o->data_width/8;
                    break;
        		}
            }
        }
        //free(data_buffer);
    }
}



void decode_fixed_packet(int pkt_idx, FILE * input_stream) {
    struct packet *p = &packet_list[pkt_idx];
	gp_debug("Fixed packet size: %d", p->size);
    
    uint64_t byte_offset = 0;
    int frame_at_idx = -1;
    
    if (packet_has_option_type(pkt_idx,O_FRAME)) {
        gp_debug("Framed packet type");
        
        // Frame stuff
        int frame_opt_idx = next_otype(p, O_FRAME,-1);
        struct poption *frame_id = &p->option_list[frame_opt_idx];
        int frame_byte = v_get_i(frame_id->frame_val);
        
        gp_debug("Frame data width: %d",frame_id->data_width);
        gp_debug("Searching for frame 0x%x", frame_byte);
        
        gp_debug("Read %d bytes", p->size);
        
        //fread(data_buffer, p->size,1 ,input_stream);
        data_buffer = malloc(p->size);
        int frame_byte_sz = frame_id->data_width/8;
        int timeout = 10;
        frame_at_idx = find_frame_in_data(p, frame_byte, frame_byte_sz,timeout, input_stream);
        if (frame_at_idx >= 0) {
            gp_debug("Found frame at index: %d", frame_at_idx);
            data_buffer = realloc(data_buffer,p->size + frame_at_idx);
        
            gp_debug("Read %d bytes", frame_at_idx);
            // Read rest of packet
            fread(&data_buffer[p->size], frame_at_idx, 1,input_stream);
        
            p->data = (uint8_t *)malloc(p->size);
            memcpy(p->data,&data_buffer[frame_at_idx],p->size);
        
            free(data_buffer);
            // Print packet
        	for(int i = 0; i < p->size; ++i) {
                gp_debug("%d: %x",i,p->data[i]);
        	}
        }
    } else {
        gp_debug("Packet has no frame id");
        p->data = (uint8_t *)malloc(p->size);
        fread(p->data, p->size, 1,input_stream);
    }
        
	for(int i = 0; i < p->size; ++i) {
        gp_debug("%d: %x",i,p->data[i]);
	}
    // Update byte offset
    byte_offset = 0;

    for (int idx = 0; idx < p->option_list_sz; idx++) {
	    struct poption *o = &p->option_list[idx];
		switch (o->otype) {
            case O_DATA:
                o->data_byte_offset = byte_offset;
                byte_offset += (o->data_width/8) * o->data_size_i;
            break;
            case O_CRC:
                o->data_byte_offset = byte_offset;
                calculate_crc(p,o);
                byte_offset += o->data_width/8;
            break;
            default:
                o->data_byte_offset = byte_offset;
                byte_offset += o->data_width/8;
            break;
		}
    }
}

int find_frame_in_data(struct packet * p, int frame_byte, int frame_byte_sz,int timeout,FILE * input_stream) {
    while(timeout--) {
        fread(data_buffer, p->size,1 ,input_stream);
        int num_checked = 0;
        for(int i = 0; i < p->size; ++i) {
            for(int j = 0; j < frame_byte_sz; ++j) {
                if (data_buffer[i] == ((uint8_t*)&frame_byte)[j]) {
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

void calculate_crc(struct packet * p, struct poption * o) {
    p->crc_valid = false;
    if (strcmp(o->crc_method,"crc_8")==0) {
        //gp_debug("Calc crc from: %d to %d", frame_at_idx, byte_offset);
        uint8_t crc = crc8(p->data, o->data_byte_offset);
        if (crc == p->data[o->data_byte_offset]) {
            p->crc_valid = true;
        }
        gp_debug("Expected: %x Received: %x", crc, p->data[o->data_byte_offset]);
    } else if (strcmp(o->crc_method,"crc_32")==0) {
        gp_debug("CRC 32 method o: %d",o->data_byte_offset);
        uint32_t packet_crc;
        uint32_t crc = crc32(p->data, o->data_byte_offset);
        memcpy(&packet_crc, p->data + o->data_byte_offset,sizeof(uint32_t));
        if (crc == packet_crc) {
            p->crc_valid = true;
        } 
        gp_debug("Expected: %x Received: %x", crc, packet_crc);
    } else if (strcmp(o->crc_method,"crc_citt")==0) {
        gp_debug("CITT CRC method");
    }
}