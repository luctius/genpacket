#include <assert.h>
#include "json_printer.h"

void print_packet_as_json(int pkt_idx) {
    struct packet *p = &packet_list[pkt_idx];
    char * str_buf = (char*)malloc(1024);
    
    printf("{\n\t\"name\": \"%s\"", p->name);
    
    for (int idx = 0; idx < p->option_list_sz; idx++) {
	    struct poption *o = &p->option_list[idx];
        memset(str_buf,0,sizeof(str_buf)*sizeof(char));
        //gp_debug("i: %d o otype: %d otype: %d",i, o->otype,otype);
		switch (o->otype) {
			case O_ATTRIBUTE:
                buffer_to_json_type_str(p->data, o->data_byte_offset, str_buf, &o->type, o->data_width);
                printf(",\n\t\"%s\": %s",o->name, str_buf);
            break;
            case O_DATA:
                printf(",\n\t\"%s\": ",o->name);
                if (o->type.ft == FT_CHAR)
                    printf("\"");
                else
                    printf("[");
            
                for(int i=0;i<o->data_size_i;i++) {
                    buffer_to_json_type_str(p->data, o->data_byte_offset+i, str_buf, &o->type, o->data_width);
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
                buffer_to_json_type_str(p->data, o->data_byte_offset, str_buf, &o->type, o->data_width);
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

void buffer_to_json_type_str(uint8_t * buf, int buf_idx,char * type_str, struct type * t, int dw) {
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
        default: assert(false);
    }
}
