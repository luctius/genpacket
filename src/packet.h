#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>

#include "value.h"

extern int line_num;
extern bool has_parse_error;

enum po_type {
    O_FRAME,
    O_ATTRIBUTE,
    O_SIZE,
    O_DATA,
    O_CRC,
};

struct type {
    enum field_type ft;
    int ft_sz;
};

inline bool type_is_valid(struct type t) {
    if (t.ft_sz == 0) return false;
    if (t.ft != FT_CHAR && t.ft != FT_SIGNED && t.ft != FT_UNSIGNED && t.ft != FT_FLOAT) return false;
    if (t.ft_sz > 64) return false;
    return true;
}

inline bool within_max(uint64_t val, uint8_t bits, enum field_type ft) {
    uint64_t max = 0;
    if (ft == FT_UNSIGNED) {
        max = (1<<bits) -1;
    }
    if (ft == FT_SIGNED) {
        max = (1<<(bits-1)) -1;
    }
    if (val > max) return false;
    return true;
}

inline bool v_is_valid(struct value v_to_check, struct type to_compare) {
    if (v_to_check.ft == to_compare.ft) return true;
    if (to_compare.ft == FT_SIGNED || to_compare.ft == FT_UNSIGNED) {
        if (v_is_float(v_to_check)) return false;
        if (to_compare.ft == FT_UNSIGNED) {
            if (v_is_signed(v_to_check) ) {
                if (v_get_i(v_to_check) < 0) return false;
            }
        }

        if (v_is_unsigned(v_to_check) ) {
            if (within_max(v_get_u(v_to_check), to_compare.ft_sz, to_compare.ft) == false) return false;
        }
        if (v_is_signed(v_to_check) ) {
            int64_t tval = v_get_i(v_to_check);
            tval = (tval < 0) ? tval * -1 : tval;
            if (within_max(tval, to_compare.ft_sz, to_compare.ft) == false) return false;
        }
    }
    return true;
}

/*
    frame <"name"> [val] type=<type>
    attr(ibute) <"name"> type=<type> default=<value> values=<value1>,<value2>
    size <"name"> type=<type> data_width=<type> exclude=<"attr1">,<"attrN">
    data <"name"> data_size=<number|"attribute"> exclude=<"attr1">,<"attrN"> type=<type> data_width=<type> escape=<esc> escape_values=<e1>,<eN> escape_op=<func> escape_param=<param>
    crc <"name"> [method] start=<"start_attribute"> end=<"end_attribute"> exclude=<"attr1">,<"attrN"> type=<type>
*/

struct poption {
    char *name;
    enum po_type otype;

    struct type type;
    int data_width;

    int value_list_sz;
    struct value *value_list;

    bool default_set;
    struct value default_val; /* use type.ft to determine type */

    int exclude_list_sz;
    char **exclude_list;

    char *start_attr;
    char *end_attr;

    char *data_size_str;
    int data_size_i;

    char *crc_method;

    struct value frame_val;
};

enum packet_type {
    PT_FIXED,
    PT_DYNAMIC,
    PT_CALCULATED,
};

struct packet {
    char *name;
    enum packet_type ptype;
    int size;
    int pipe;

    int option_list_sz;
    struct poption *option_list;
};

extern int pipes;
extern int packet_list_sz;
extern struct packet *packet_list;

void add_packet(enum packet_type ptype, char *name);
struct packet *get_curr_packet(void);
void free_packet(int idx);

void add_option(enum po_type otype);
struct poption *get_curr_option(void);
void option_add_name(struct packet *p, struct poption *o, char *name);
void free_option(int pidx, int oidx);

bool packet_name_is_unique(struct packet *p);
bool option_name_is_unique(struct packet *p, struct poption *o);

void check_curr_packet(void);
void check_curr_option(void);

char *packet_to_str(int pkt_idx);
char *option_to_str(int pkt_idx, int idx);
char *type_to_str(struct type t);

void parse_error(struct packet *packet, struct poption *option, const char *fmt, ...);
void parse_debug(struct packet *packet, struct poption *option, const char *fmt, ...);

bool packet_has_option_type(int pkt_idx, enum po_type option_type);
int next_otype(struct packet * p, enum po_type otype, int idx);

#endif /* PACKET_H */
