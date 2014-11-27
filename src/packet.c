#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <stdarg.h>

#include "packet.h"

#define TS_BFR_LEN 256

int packet_list_sz = 0;
struct packet *packet_list = NULL;

extern inline bool type_is_valid(struct type t);
extern inline bool v_is_valid(struct value v_to_check, struct type to_compare);
extern inline bool within_max(uint64_t val, uint8_t bits, enum field_type ft);

void parse_debug(struct packet *packet, struct poption *option, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    has_parse_error = true;
    fprintf(stderr, "[line: %d] debug: ", line_num);
    if (packet != NULL) {
        fprintf(stderr, "[%s", packet->name);
        if (option != NULL) fprintf(stderr, ".%s", option->name);
        fprintf(stderr, "] ");
    }

    vfprintf(stderr, fmt, args);
    fprintf(stderr, ".\n");
}

void parse_error(struct packet *packet, struct poption *option, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    has_parse_error = true;
    fprintf(stderr, "[line: %d] error: ", line_num);
    if (packet != NULL) {
        fprintf(stderr, "[%s", packet->name);
        if (option != NULL) fprintf(stderr, ".%s", option->name);
        fprintf(stderr, "] ");
    }

    vfprintf(stderr, fmt, args);
    fprintf(stderr, ".\n");
}

void add_packet(enum packet_type ptype, char *name) {
    packet_list = realloc(packet_list, ++packet_list_sz * sizeof(struct packet) );
    assert(packet_list != NULL);

    struct packet *p = get_curr_packet();
    memset(p, 0x0, sizeof(struct packet) );
    p->name = name;
    p->ptype = ptype;

    p->size = 0;
    p->pipe = -1;
    p->option_list_sz = 0;
    p->option_list = NULL;

    if (!packet_name_is_unique(p) ) {
        parse_error(p, NULL, "name \"%s\" already exist");
    }
}

void free_packet(int idx) {
    struct packet *p = &packet_list[idx];

    if (p->name != NULL) {
        free(p->name);
        p->name = NULL;
    }

    for (int i = 0; i < p->option_list_sz; i++) {
        free_option(idx, i);
    }
    free(p->option_list);
    p->option_list = NULL;
    p->option_list_sz = 0;
}

struct packet *get_curr_packet() {
    if (packet_list_sz <= 0) return NULL;
    return &packet_list[packet_list_sz-1];
}

void add_option(enum po_type otype) {
    if (packet_list_sz <= 0) return;
    struct packet *p = get_curr_packet();

    p->option_list = realloc(p->option_list, ++p->option_list_sz * sizeof(struct poption) );
    assert(p->option_list != NULL);

    struct poption *o = get_curr_option();
    memset(o, 0x0, sizeof(struct poption) );
    o->otype = otype;

    o->default_set = false;

    o->value_list_sz = 0;
    o->value_list = NULL;

    o->exclude_list_sz = 0;
    o->exclude_list = NULL;

    o->data_size_str = NULL;
    o->data_size_i = -1;

    o->crc_method = NULL;

    const char *defname;
    switch(o->otype) {
        case O_FRAME:  defname = "frame%d";
                       break;
        case O_ATTRIBUTE: defname = "attribute%d";
                          break;
        case O_SIZE:   defname = "size%d";
                       break;
        case O_DATA:   defname = "data%d";
                       break;
        case O_CRC:    defname = "crc%d";
                       break;
        default:       defname = "default%d";
                       break;
    }

    o->name = malloc(strlen(defname) +100);

    sprintf(o->name, defname, 0);
    for (int i = 1; i < 100 && !option_name_is_unique(p, o); i++) {
        sprintf(o->name, defname, i);
    }
    o->name = realloc(o->name, (strlen(o->name) +1) * sizeof(char) );
}

void free_option(int pidx, int oidx) {
    struct packet *p = &packet_list[pidx];
    struct poption *o = &p->option_list[oidx];

    if (o->name != NULL) {
        free(o->name);
        o->name = NULL;
    }
    if (o->data_size_str != NULL) {
        free(o->data_size_str);
        o->data_size_str = NULL;
    }
    if (o->start_attr != NULL) {
        free(o->start_attr);
        o->start_attr = NULL;
    }
    if (o->end_attr != NULL) {
        free(o->end_attr);
        o->end_attr = NULL;
    }
    if (o->crc_method != NULL) {
        free(o->crc_method);
        o->crc_method = NULL;
    }

    free(o->value_list);
    o->value_list = NULL;
    o->value_list_sz = 0;

    for (int i = 0; i < o->exclude_list_sz; i++) {
        free(o->exclude_list[i]);
    }
    free(o->exclude_list);
    o->exclude_list_sz = 0;
    o->exclude_list = NULL;
}


struct poption *get_curr_option(void) {
    if (packet_list_sz <= 0) return NULL;
    struct packet *p = get_curr_packet();
    if (p->option_list_sz == 0) return NULL;

    return &p->option_list[p->option_list_sz-1];
}

void option_add_name(struct packet *p, struct poption *o, char *name) {
    free(o->name);

    o->name = name;
    if (!option_name_is_unique(p, o) ) {
        parse_error(p, o, "option name is a duplicate");
    }
}

void check_curr_packet(void) {
    struct packet *p = &packet_list[packet_list_sz-1];

    int total_sz_bits = 0;
    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *o = &p->option_list[i];
        if (o->otype == O_DATA) {
            if (o->data_size_i != -1) total_sz_bits += o->type.ft_sz * o->data_size_i;
        }
        else total_sz_bits += o->type.ft_sz;
    }

    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *o = &p->option_list[i];

        if (o->data_size_str != NULL) {
            struct poption top = { .name = o->data_size_str, };
            if (option_name_is_unique(p, &top) ) {
                parse_error(p, o, "data_size \"%s\" is an unkown attribute", o->start_attr);
            }
        }

        if (o->start_attr != NULL) {
            struct poption top = { .name = o->start_attr, };
            if (option_name_is_unique(p, &top) ) {
                parse_error(p, o, "start attribute \"%s\" is unknown", o->start_attr);
            }
        }

        if (o->end_attr != NULL) {
            struct poption top = { .name = o->end_attr, };
            if (option_name_is_unique(p, &top) ) {
                parse_error(p, o, "end attribute \"%s\" is unknown", o->end_attr);
            }
        }

        for (int j = 0; j < o->exclude_list_sz; j++) {
            struct poption top = { .name = o->exclude_list[j], };
            if (option_name_is_unique(p, &top) ) {
                parse_error(p, o, "exclude[%d] \"%s\" is unknown", j, o->exclude_list[j]);
            }
        }
    }

    switch(p->ptype) {
        default:
        case PT_FIXED:
            {
                int data_mbrs_wo_sz = 0;
                for (int i = 0; i < p->option_list_sz; i++) {
                    struct poption *o = &p->option_list[i];
                    if (o->otype == O_DATA) {
                        if (o->data_size_i == -1) {
                            data_mbrs_wo_sz++;

                            if (data_mbrs_wo_sz > 1 || p->size == 0) {
                                parse_error(p, o, "fixed size packet, but no size given for packet or data member");
                            }
                            else o->data_size_i = ( (p->size * 8) - total_sz_bits)/o->type.ft_sz;
                        }
                    }
                }
            }
            break;
        case PT_DYNAMIC:
            for (int i = 0; i < p->option_list_sz; i++) {
                struct poption *o = &p->option_list[i];
                if (o->otype == O_DATA) {
                    if (o->data_size_str == NULL) {
                        parse_error(p, o, "no size member given for dynamic data");
                    }
                }
            }
            break;
        case PT_CALCULATED:
            {
                /*
                   Match size and data members if possible.
                 */
                int size_members = 0;
                int data_members = 0;
                for (int i = 0; i < p->option_list_sz; i++) {
                    struct poption *o = &p->option_list[i];
                    if (o->otype == O_SIZE) size_members++;
                    if (o->otype == O_DATA) {
                        if (o->data_size_str == NULL && o->data_size_i == -1) {
                            data_members++;
                        }
                    }
                }
                
                if (size_members == data_members) {
                    char *size_mbr_names[size_members];
                    size_members = 0;
                    data_members = 0;
                    for (int i = 0; i < p->option_list_sz && (data_members < size_members); i++) {
                        struct poption *o = &p->option_list[i];
                        if (o->otype == O_SIZE) {
                            size_mbr_names[size_members++] = o->name;
                        }
                        if (o->otype == O_DATA) {
                            if (o->data_size_str == NULL && o->data_size_i == -1) {
                                if (size_mbr_names[data_members] == NULL) {
                                    parse_error(p, o, "unable to match data and size members");
                                }
                                else o->data_size_str = size_mbr_names[data_members++];
                            }
                        }
                    }
                }

                for (int i = 0; i < p->option_list_sz; i++) {
                    struct poption *o = &p->option_list[i];
                    if (o->otype == O_DATA) {
                        if (o->data_size_str == NULL && o->data_size_i == -1) {
                            parse_error(p, o, "data member has unkown size");
                        }
                    }
                }
            }
            break;
    }

    /* final size check */
    total_sz_bits = 0;
    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *o = &p->option_list[i];
        if ( (o->otype == O_DATA) && ( (o->data_size_i != -1) || (p->ptype == PT_FIXED) ) ) {
            total_sz_bits += o->type.ft_sz * o->data_size_i;
        }
        else total_sz_bits += o->type.ft_sz;
    }
    if (total_sz_bits % 8 != 0) {
        parse_error(p, NULL, "packet not byte aligned");
    }
    if ( (p->size != 0) && (p->size < (total_sz_bits/8) ) ) {
        parse_error(p, NULL, "packet size is smaller than all members combined");
    }
    else if (p->ptype == PT_FIXED && (total_sz_bits) != (p->size * 8) ) {
        parse_error(p, NULL, "packet size given cannot be correct");
        parse_debug(p, NULL, "packet size given (in bits %d), packet size in bits (%d)", p->size*8, total_sz_bits);
    }
}

void check_curr_option(void) {
    struct packet *p = &packet_list[packet_list_sz-1];
    struct poption *o = &p->option_list[p->option_list_sz-1];

    if (o->type.ft_sz == 0) {
        o->type.ft_sz = 8;
        o->type.ft = FT_UNSIGNED;
    }

    if (!type_is_valid(o->type) ) {
        parse_error(p, o, "type \"%s\" is invalid", type_to_str(o->type) );
    }

    if (o->data_width > o->type.ft_sz) {
        parse_error(p, o, "data_width \"%d\" is invalid", o->data_width);
    }
    else if (o->data_width == 0) o->data_width = o->type.ft_sz;

    if (o->default_set) {
        if (!v_is_valid(o->default_val, o->type) ) {
            parse_error(p, o, "default value \"%s\" is not valid for an attribute of type %s", 
                v_to_str(o->default_val), type_to_str(o->type) );
        }
        else o->default_val = v_convert(o->default_val, o->type.ft);
    }

    for (int i = 0; i < o->value_list_sz; i++) {
        if (!v_is_valid(o->value_list[i], o->type) ) {
            parse_error(p, o, "values[%d] \"%s\" is not valid for an attribute of type %s", 
                i, v_to_str(o->value_list[i]), type_to_str(o->type) );
        }
        else o->value_list[i] = v_convert(o->value_list[i], o->type.ft);
    }

    switch(o->otype) {
        case O_FRAME: // frame <"name"> [val] type=<type>
            if (!v_is_valid(o->frame_val, o->type) ) {
                parse_error(p, o, "frame value \"%s\" is not valid for an attribute of type %s", 
                        v_to_str(o->frame_val), type_to_str(o->type) );
            }
            break;
        case O_ATTRIBUTE: // attr(ibute) <"name"> type=<type> default=<value> values=<value1>,<value2>
            break;
        case O_SIZE: // size <"name"> type=<type> data_width=<type> exclude=<"attr1">,<"attrN">
            break;
        case O_DATA: // data <"name"> data_size=<number|"attribute"> exclude=<"attr1">,<"attrN"> type=<type> data_width=<type> escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
            //test data_size
            break;
        case O_CRC: // crc <"name"> [method] start=<"start_attribute"> end=<"end_attribute"> exclude=<"attr1">,<"attrN"> type=<type>
            //test method
            break;
        default:    
            break;
    }
}

char *packet_to_str(int pkt_idx) {
    struct packet *p = &packet_list[pkt_idx];

    int size = TS_BFR_LEN + (TS_BFR_LEN * p->option_list_sz);
    char *ts_bfr = malloc(size * sizeof(char) );

    int ctr = 0;
    char *sp = ts_bfr;

    switch(p->ptype) {
        default:
        case PT_FIXED:
            ctr += sprintf(&sp[ctr], "fixed ");
            break;
        case PT_DYNAMIC:
            ctr += sprintf(&sp[ctr], "dynamic ");
            break;
        case PT_CALCULATED:
            ctr += sprintf(&sp[ctr], "calculated ");
            break;
    }

    ctr += sprintf(&sp[ctr], "packet %s ", p->name);
    ctr += sprintf(&sp[ctr], "[size: %d] ", p->size);
    ctr += sprintf(&sp[ctr], "[pipe: %d] ", p->pipe);
    ctr += sprintf(&sp[ctr], "{\n");

        for (int i = 0; i < p->option_list_sz; i++) {
            ctr += sprintf(&sp[ctr], "\t%s\n", option_to_str(pkt_idx, i) );
            option_to_str(pkt_idx, i);
        }

    ctr += sprintf(&sp[ctr], "}\n");

    ts_bfr = realloc(ts_bfr, strlen(ts_bfr) +2 );

    return ts_bfr;
}

char *option_to_str(int pkt_idx, int idx) {
    static char ts_bfr[TS_BFR_LEN];
    struct packet *p = &packet_list[pkt_idx];
    struct poption *o = &p->option_list[idx];

    int ctr = 0;
    char *sp = ts_bfr;
    ctr += sprintf(&sp[ctr], "%s ", o->name);
    ctr += sprintf(&sp[ctr], "[type: %s] ", type_to_str(o->type) );
    ctr += sprintf(&sp[ctr], "[data_width: %d] ", o->data_width);

    if (o->default_set) ctr += sprintf(&sp[ctr], "[default: %s] ", v_to_str(o->default_val) );

    if (o->exclude_list_sz > 0) {
        ctr += sprintf(&sp[ctr], "[exclude: ");
        for (int i = 0; i < o->exclude_list_sz; i++) {
            if (i != 0) ctr += sprintf(&sp[ctr], ", ");
            ctr += sprintf(&sp[ctr], "%s", o->exclude_list[i]);
        }
        ctr += sprintf(&sp[ctr], "] ");
    }

    if (o->value_list_sz > 0) {
        ctr += sprintf(&sp[ctr], "[value: ");
        for (int i = 0; i < o->value_list_sz; i++) {
            if (i != 0) ctr += sprintf(&sp[ctr], ", ");
            ctr += sprintf(&sp[ctr], "%s", v_to_str(o->value_list[i]) );
        }
        ctr += sprintf(&sp[ctr], "] ");
    }

    if (o->data_size_str != NULL)   ctr += sprintf(&sp[ctr], "[data_size: %s] ", o->data_size_str);
    else if (o->data_size_i != -1) ctr += sprintf(&sp[ctr], "[data_size: %d] ", o->data_size_i);

    return sp;
}

char *type_to_str(struct type t) {
    static char bfr[12];

    if (t.ft_sz == 0) {
        sprintf(bfr, "default");
        return bfr;
    }

    switch (t.ft) {
    case FT_SIGNED:
        sprintf(bfr, "int%d", t.ft_sz);
        break;
    case FT_UNSIGNED:
        if (t.ft_sz == 1) sprintf(bfr, "bit");
        else sprintf(bfr, "uint%d", t.ft_sz);
        break;
    case FT_FLOAT:
        if (t.ft_sz == 8) sprintf(bfr, "double");
        else sprintf(bfr, "float");
        break;
    default: sprintf(bfr, "default"); break;
    }
    return bfr;
}

bool packet_name_is_unique(struct packet *p) {
    if (p->name == NULL) return false;

    int cnt = 0;
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *pt = &packet_list[i];
        if (p == pt) continue;
        if (strcmp(p->name, pt->name) == 0) return false;
    }

    return true;
}

bool option_name_is_unique(struct packet *p, struct poption *o) {
    if (o->name == NULL) return false;

    int cnt = 0;
    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *ot = &p->option_list[i];
        if (o == ot) continue;
        if (strcmp(o->name, ot->name) == 0) return false;
    }

    return true;
}
