#include <assert.h>
#include "json_printer.h"

void print_packet_as_json(int pkt_idx) {
    struct packet *p = &packet_list[pkt_idx];
    char * str_buf = (char*)malloc(1024);
    
    printf("{\n\t\"name\": \"%s\"", p->name);
    
    for (int idx = 0; idx < p->option_list_sz; idx++) {
	    struct poption *o = &p->option_list[idx];
        memset(str_buf,0,sizeof(str_buf)*sizeof(char));
        //gp_debug("bit: %d byte: %d diff: %d",o->bit_alignment,o->data_byte_offset,o->bit_alignment/BYTE_SIZE);
		switch (o->otype) {
			case O_ATTRIBUTE:
                buffer_to_json_type_str(p->data, o->bit_alignment, str_buf, o);
                printf(",\n\t\"%s\": %s",o->name, str_buf);
            break;
            case O_DATA:
                printf(",\n\t\"%s\": ",o->name);
                if (o->type.ft == FT_CHAR)
                    printf("\"");
                else
                    printf("[");
            
                for(int i=0;i<o->data_size_i;i++) {
                    buffer_to_json_type_str(p->data, o->bit_alignment+(i*BYTE_SIZE), str_buf, o);
                    printf("%s",str_buf);
                    if (i != o->data_size_i - 1 && o->type.ft != FT_CHAR)
                        printf(",");
                    else if (o->type.ft != FT_CHAR)
                        printf("]");
                    else if (i == o->data_size_i - 1 && o->type.ft == FT_CHAR)
                        printf("\"");
                }
            
            break;
            case O_CRC:
                printf(",\n\t\"%s\": ",o->name);
                buffer_to_json_type_str(p->data, o->bit_alignment, str_buf, o);
                printf("%s",str_buf);
                printf(",\n\t\"crc_valid\": ");
                if (p->crc_valid) {
                    printf("true");
                } else {
                    printf("false");
                }
            break;
            default:
            break;
		}
    }
    printf("\n}\n");
    free(str_buf);
}

void buffer_to_json_type_str(uint8_t * buf, int buf_bit_idx,char * type_str, struct poption *o) {
    int dw = o->data_width;
    uint8_t check_bits = dw % BYTE_SIZE;
    uint64_t bit_mask = 0xffffffffffffffff;
    uint32_t buf_idx = 0;
    if (buf_bit_idx >= BYTE_SIZE) {
        buf_idx = buf_bit_idx / BYTE_SIZE;
    }
    
    uint32_t data_sz_bytes = dw / BYTE_SIZE;
        
    if (check_bits) {
        data_sz_bytes++;
    }
    uint64_t tmp_u=0;
    uint64_t tmp_s=0;
    switch(o->type.ft) {
        case FT_UNSIGNED:
            if (check_bits) {
                bit_mask = (1<<dw)-1;
            }
            memcpy(&tmp_u, &buf[buf_idx],data_sz_bytes);
            tmp_u >>= o->bit_alignment % BYTE_SIZE;
            //gp_debug("Val: %x bitmsk: %x raw val: %x shift: %x", tmp_u, bit_mask, buf[buf_idx], o->bit_alignment % BYTE_SIZE);
            sprintf(type_str,"%"PRIu64, tmp_u&bit_mask);
        break;
        case FT_SIGNED:
            if (check_bits) {
                bit_mask ^= (1<<dw)-1;
            }
            memcpy(&tmp_s, &buf[buf_idx],data_sz_bytes);
            tmp_s >>= o->bit_alignment % BYTE_SIZE;
            //gp_debug("Val: %x bitmsk: %x raw val: %x shift: %x sz: %x", tmp_s, bit_mask, buf[buf_idx], o->bit_alignment % BYTE_SIZE,dw);
            sprintf(type_str,"%"PRId64, tmp_s|bit_mask);
        break;
        case FT_FLOAT:
            if (dw == 32) {
                float tmp_f_32;
                memcpy(&tmp_f_32, &buf[buf_idx],4);
                sprintf(type_str,"%f", tmp_f_32);
            } else if (dw == 64) {
                double tmp_f_64;
                memcpy(&tmp_f_64, &buf[buf_idx],8);
                sprintf(type_str,"%lf", tmp_f_64);
            }
        break;
        case FT_CHAR:
            sprintf(type_str,"%c", buf[buf_idx]);
        break;
        default: assert(false);
    }
}
