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
            
            printf("{\n\tname: \"%s\"", p->name);
            // Attribute stuff
            int attr_idx = -1;
            do {
                attr_idx = next_otype(p, O_ATTRIBUTE,attr_idx);
                if (attr_idx >= 0) {
                    struct poption *attr = &p->option_list[attr_idx];
                    
                    char type_str[100];
                    
                    buffer_to_type_str(packet_buffer, byte_offset, type_str, &attr->type, attr->data_width);
                    
                    printf(",\n\t%s: %s",attr->name, type_str);
                    
                    //gp_debug("Attr datawidth: %d\nAttr type: %d\n", attr->data_width, attr->type.ft);
                    //, v_to_str(attr->frame_val));
                    byte_offset += attr->data_width/8;
                }
                //gp_debug("ttr idx: %d\n", attr_idx);
                
            } while(attr_idx >= 0);
            
            
            // Data stuff
            int data_idx = -1;
            do {
                data_idx = next_otype(p, O_DATA,data_idx);
                //gp_debug("Data idx: %d\n", attr_idx);
                if (data_idx >= 0) {
                    struct poption *data = &p->option_list[data_idx];
                    
                    char data_str[1024];
                    
                    printf(",\n\t%s: ",data->name);
                    if (data->type.ft == FT_CHAR)
                        printf(" \"");
                    else
                        printf(" [");
                    
                    for(int i=0;i<data->data_size_i;i++) {                    
                        buffer_to_type_str(packet_buffer, byte_offset, data_str, &data->type, data->data_width);
                        printf("%s",data_str);
                        if (i != data->data_size_i - 1 && data->type.ft != FT_CHAR)
                            printf(",");
                        else if (data->type.ft != FT_CHAR)
                            printf("]");
                        else if (i == data->data_size_i - 1 && data->type.ft == FT_CHAR)
                            printf("\"");
                        byte_offset += data->data_width/8;
                    }
                }
                //gp_debug("ttr idx: %d\n", attr_idx);
                
            } while(data_idx >= 0);
            printf("\n}\n");
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