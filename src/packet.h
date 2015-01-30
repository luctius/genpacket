#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>

#include "value.h"
#include "debug_print.h"

#ifndef FIX_UNUSED
#define FIX_UNUSED(X) (void) (X) /* avoid warnings for unused params */
#endif

extern int line_num;
extern bool has_parse_error;

enum po_type {
    O_ATTRIBUTE,
    O_CRC,
    O_DATA,
    O_FRAME,
    O_HIDDEN,
    O_SIZE,
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
    
    uint32_t bit_alignment;
    uint32_t data_byte_offset;
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
    uint8_t * data;
    bool crc_valid;
    bool crc_reflect_in;
    bool crc_reflect_out;
    uint32_t crc_xor_in;
    uint32_t crc_xor_out;
    uint32_t crc_poly;
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
uint32_t calculate_min_size(struct packet * p);

struct poption *get_option_by_name(struct packet * p,char* name);

#define parse_error(_pack,_op,fmt, ...) \
do { \
    has_parse_error = true; \
    struct packet *_p = NULL; \
    struct poption *_o = NULL; \
    if (_pack != NULL) _p = (struct packet *) _pack; \
    if (_op != NULL) _o = (struct potion *) _op; \
    char _str[1024]; \
    int _ctr = 0; \
    _ctr += snprintf(&_str[_ctr], 1024-_ctr, "[line: %d]: ", line_num); \
    if (_p != NULL) { \
        _ctr += snprintf(&_str[_ctr], 1024-_ctr, "[%s", _p->name); \
        if (_o != NULL) _ctr += snprintf(&_str[_ctr], 1024 - _ctr, ".%s", _o->name); \
        _ctr += snprintf(&_str[_ctr], 1024 - _ctr, "] "); \
    } \
    gp_err("%s" fmt, _str, ##__VA_ARGS__);  \
} while (0)

#define parse_debug(_pack,_op,fmt, ...) \
do { \
    struct packet *_p = NULL; \
    struct poption *_o = NULL; \
    if (_pack != NULL) _p = (struct packet *) _pack; \
    if (_op != NULL) _o = (struct potion *) _op; \
    char _str[1024]; \
    int _ctr = 0; \
    _ctr += snprintf(&_str[_ctr], 1024-_ctr, "[line: %d]", line_num); \
    if (_p != NULL) { \
        _ctr += snprintf(&_str[_ctr], 1024-_ctr, "[%s", _p->name); \
        if (_o != NULL) _ctr += snprintf(&_str[_ctr], 1024 - _ctr, ".%s", _o->name); \
        _ctr += snprintf(&_str[_ctr], 1024 - _ctr, "] "); \
    } \
    gp_debug("%s" fmt, _str, ##__VA_ARGS__);  \
} while (0)

#endif /* PACKET_H */
