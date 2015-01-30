#include "decoder.h"
#include "packet.h"
#include "debug_print.h"
#include <inttypes.h>
#include "crc.h"
#include "json_printer.h"

void decode_packet(int pkt_idx, FILE * input_stream) {
	if (input_stream == NULL) {
	  fprintf(stderr, "Can't open input file!\n");
	  exit(1);
	}
	
    struct packet *p = &packet_list[pkt_idx];

    if (packet_has_option_type(pkt_idx,O_FRAME)) {
        gp_debug("Framed packet type");
        
        // Frame stuff
        int frame_opt_idx = next_otype(p, O_FRAME,-1);
        struct poption *frame_id = &p->option_list[frame_opt_idx];
        int frame_byte = v_get_i(frame_id->frame_val);
        
        gp_debug("Frame data width: %d",frame_id->data_width);
        
        // Update estimated size
        if (p->ptype==PT_CALCULATED) {
            p->size = calculate_min_size(p);
        }
        
        gp_debug("Read %d bytes", p->size);
        data_buffer = malloc(p->size);
        
        int timeout = 10;
        int frame_at_idx = find_frame_in_data(p, frame_byte, frame_id->data_width,timeout,input_stream);
        
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
        } else {
            gp_debug("Alas frame not found");
        }
        
        //free(data_buffer);
    } else {
        gp_debug("Packet has no frame id");
        p->data = (uint8_t *)malloc(p->size);
        fread(p->data, p->size, 1,input_stream);
    }
        
	for(int i = 0; i < p->size; ++i) {
        gp_debug("%d: %x",i,p->data[i]);
	}
    
    update_data_offsets(p, input_stream);
 
    
    print_packet_as_json(pkt_idx);
}



int find_frame_in_data(struct packet * p, int frame_byte, int frame_bit_sz,int timeout,FILE * input_stream) {
    uint8_t check_bits = frame_bit_sz % BYTE_SIZE;
    uint32_t frame_byte_sz = 1;
    uint8_t bit_mask = 0xff;
    if (frame_bit_sz >= BYTE_SIZE) {
        frame_byte_sz = frame_bit_sz / BYTE_SIZE;
    } 
    if (check_bits) {
        bit_mask = (1<<check_bits)-1;
    }
    
    while(timeout--) {
        fread(data_buffer, p->size,1 ,input_stream);
        uint32_t num_checked = 0;
        for(int i = 0; i < p->size; ++i) {                
            for(uint32_t j = 0; j < frame_byte_sz; ++j) {
                gp_debug("Checking %x against %x",(data_buffer[i] & (1<<check_bits)-1) , ((((uint8_t*)&frame_byte)[j]) & (1<<check_bits)-1))
                if ((data_buffer[i] & bit_mask)== ((((uint8_t*)&frame_byte)[j]) & bit_mask)) {
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

void update_data_offsets(struct packet *p, FILE * input_stream) {
    // Update byte offset
    uint32_t bit_offset = 0;
    struct poption *s_o = NULL;

    for (int idx = 0; idx < p->option_list_sz; idx++) {
	    struct poption *o = &p->option_list[idx];
		switch (o->otype) { 
            case O_SIZE:
                o->data_byte_offset = bit_offset / BYTE_SIZE;
                
                if (p->ptype == PT_CALCULATED) {
                    p->size += p->data[o->data_byte_offset];
                    p->data = realloc(p->data,p->size);

                    // Read rest of packet
                    fread(&p->data[p->size - p->data[o->data_byte_offset]], p->data[o->data_byte_offset], 1,input_stream);

                    // Print packet
                	for(int i = 0; i < p->size; ++i) {
                        gp_debug("%d: %x",i,p->data[i]);
                	}
            
                    gp_debug("Dynamic packet new size: %d", p->size);
                }
            
                bit_offset += o->data_width;
            break;
            case O_DATA:
                if (p->ptype == PT_CALCULATED) {
                    s_o = get_option_by_name(p,o->data_size_str);
    
                    o->data_size_i = p->data[s_o->data_byte_offset];
                    o->data_byte_offset = bit_offset / BYTE_SIZE;
                    gp_debug("Data %s new size: %d from: %s offset: %d",o->name, o->data_size_i,s_o->name,o->data_byte_offset);
            
                    bit_offset += o->data_width * o->data_size_i;
                } else {
                    o->data_byte_offset = bit_offset / BYTE_SIZE;//byte_offset;
                    bit_offset += o->data_width * o->data_size_i;
                }
            break;
            case O_CRC:
                 o->data_byte_offset = bit_offset / BYTE_SIZE;
                calculate_crc(p,o);
                bit_offset += o->data_width;
            break;
            default:
                o->data_byte_offset = bit_offset / BYTE_SIZE;
                bit_offset += o->data_width;
            break;
		}
    }
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
    } else if (strcmp(o->crc_method,"crc_16")==0) {
        gp_debug("CRC 16 method o: %d",o->data_byte_offset);
        uint16_t packet_crc;
        uint16_t crc = crc16(p->data, o->data_byte_offset);
        memcpy(&packet_crc, p->data + o->data_byte_offset,sizeof(uint16_t));
        if (crc == packet_crc) {
            p->crc_valid = true;
        } 
        gp_debug("Expected: %x Received: %x", crc, packet_crc&((1<<o->data_width)-1));
    } else if (strcmp(o->crc_method,"crc_custom")==0) {
        gp_debug("CRC custom method o: %d %d",o->data_byte_offset, o->data_width);
        uint64_t packet_crc;
        crc_cfg_t crc_config = {
            .width          = o->data_width,
            .poly           = p->crc_poly,
            .xor_in         = p->crc_xor_in,
            .reflect_in     = p->crc_reflect_in,
            .xor_out        = p->crc_xor_out,
            .reflect_out    = p->crc_reflect_out,
        };
        
        gp_debug("CRC settings:\nwidth:%d\npoly:0x%x\nxor_in:0x%x\nxor_out:0x%x\nreflect_in:%d\nreflect_out:%d\n",o->data_width, p->crc_poly,p->crc_xor_in,p->crc_xor_out,p->crc_reflect_in,p->crc_reflect_out);
        /*for(int i=0;i<o->data_byte_offset;i++) {
            printf("%02x",p->data[i]);
        }*/
        uint64_t crc = custom_crc(&crc_config,p->data, o->data_byte_offset);
        memcpy(&packet_crc, p->data + o->data_byte_offset,o->data_width/8);
        if ((crc&((1<<o->data_width)-1)) == (packet_crc&((1<<o->data_width)-1))) {
            p->crc_valid = true;
        } 
        gp_debug("Expected: %x Received: %x", crc, packet_crc&((1<<o->data_width)-1));
    } else if (strcmp(o->crc_method,"crc_citt")==0) {
        gp_debug("CITT CRC method");
    }
}