#include "decoder.h"
#include "packet.h"
#include "debug_print.h"
#include <inttypes.h>

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
    
    uint64_t byte_offset = 0;
    
    if (packet_has_option_type(pkt_idx,O_FRAME)) {
        gp_debug("Framed packet type");
        
        // Frame stuff
        int frame_opt_idx = next_otype(p, O_FRAME,-1);
        struct poption *frame_id = &p->option_list[frame_opt_idx];
        int frame_byte = v_get_i(frame_id->frame_val);
        
        gp_debug("Frame data width: %d",frame_id->data_width);
        gp_debug("Searching for frame 0x%x", frame_byte);
        
        gp_debug("Read %d bytes", p->size);
        
        fread(packet_buffer, p->size,1 ,input_stream);
        
        int frame_byte_sz = frame_id->data_width/8;
        int timeout = 10;
        int frame_at_idx = find_frame_id(p, frame_byte, frame_byte_sz,timeout);
        
        // Update byte offset
        byte_offset += frame_at_idx + frame_byte_sz;
        
        if (frame_at_idx >= 0) {
            gp_debug("Found frame at index: %d", frame_at_idx);
            packet_buffer = realloc(packet_buffer,p->size + frame_at_idx);
            
            gp_debug("Read %d bytes", frame_at_idx);
            // Read rest of packet
            fread(&packet_buffer[p->size], frame_at_idx, 1,input_stream);
            
            // Print packet
        	for(int i = frame_at_idx; i < p->size + frame_at_idx; ++i) {
                gp_debug("%d: %x",i,packet_buffer[i]);
        	}
            
            printf("{\n\t\"name\": \"%s\"", p->name);
            
            char * str_buf = (char*)malloc(1024);
            // Handle attributes and data
            for (int idx = frame_opt_idx+1; idx < p->option_list_sz; idx++) {
        	    struct poption *o = &p->option_list[idx];
                memset(str_buf,0,sizeof(str_buf)*sizeof(char));
                //gp_debug("i: %d o otype: %d otype: %d",i, o->otype,otype);
        		switch (o->otype) {
        			case O_ATTRIBUTE:
                        buffer_to_type_str(packet_buffer, byte_offset, str_buf, &o->type, o->data_width);
                    
                        printf(",\n\t\"%s\": %s",o->name, str_buf);
                        // Update byte position
                        byte_offset += o->data_width/8;
                    break;
                    case O_DATA:
                        printf(",\n\t\"%s\": ",o->name);
                        if (o->type.ft == FT_CHAR)
                            printf("\"");
                        else
                            printf("[");
                    
                        for(int i=0;i<o->data_size_i;i++) {                    
                            buffer_to_type_str(packet_buffer, byte_offset, str_buf, &o->type, o->data_width);
                            printf("%s",str_buf);
                            if (i != o->data_size_i - 1 && o->type.ft != FT_CHAR)
                                printf(",");
                            else if (o->type.ft != FT_CHAR)
                                printf("]");
                            else if (i == o->data_size_i - 1 && o->type.ft == FT_CHAR)
                                printf("\"");
                            // Update byte position
                            byte_offset += o->data_width/8;
                        }
                    break;
                    case O_CRC:
                        printf(",\n\t\"%s\": ",o->name);
                        buffer_to_type_str(packet_buffer, byte_offset, str_buf, &o->type, o->data_width);
                        printf("%s",str_buf);
                    break;
                    default:
                    break;
        		}
            }
            printf("\n}\n");
            free(str_buf);
        }
        
    }
    free(packet_buffer);
}

void buffer_to_type_str(uint8_t * buf, int buf_idx,char * type_str, struct type * t, int dw) {
    switch(t->ft) {
        case FT_UNSIGNED:
            if (dw == 8) {
                sprintf(type_str,"%"PRIu8, buf[buf_idx]);
            } else if (dw == 16) {
                uint16_t tmp;
                memcpy(&tmp, &buf[buf_idx],2);
                sprintf(type_str,"%"PRIu16, tmp);
            } else if (dw == 32) {
                uint32_t tmp;
                memcpy(&tmp, &buf[buf_idx],4);
                sprintf(type_str,"%"PRIu32, tmp);
            } else if (dw == 64) {
                uint64_t tmp;
                memcpy(&tmp, &buf[buf_idx],8);
                sprintf(type_str,"%"PRIu64, tmp);
            }
        break;
        case FT_SIGNED:
            if (dw == 8) {
                sprintf(type_str,"%"PRId8, buf[buf_idx]);
            } else if (dw == 16) {
                int16_t tmp;
                memcpy(&tmp, &buf[buf_idx],2);
                sprintf(type_str,"%"PRId16, tmp);
            } else if (dw == 32) {
                int32_t tmp;
                memcpy(&tmp, &buf[buf_idx],4);
                sprintf(type_str,"%"PRId32, tmp);
            } else if (dw == 64) {
                int64_t tmp;
                memcpy(&tmp, &buf[buf_idx],8);
                sprintf(type_str,"%"PRId64, tmp);
            }
        break;
        case FT_FLOAT:
            if (dw == 32) {
                float tmp;
                memcpy(&tmp, &buf[buf_idx],4);
                sprintf(type_str,"%f", tmp);
            } else if (dw == 64) {
                double tmp;
                memcpy(&tmp, &buf[buf_idx],8);
                sprintf(type_str,"%lf", tmp);
            }
        break;
        case FT_CHAR:
            sprintf(type_str,"%c", buf[buf_idx]);
        break;
    }
}

int find_frame_id(struct packet * p, int frame_byte, int frame_byte_sz,int timeout) {
    while(timeout--) {
        int num_checked = 0;
        for(int i = 0; i < p->size; ++i) {
            for(int j = 0; j < frame_byte_sz; ++j) {
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