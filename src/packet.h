#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>

#include "value.h"

extern int line_num;
extern bool has_parse_error;
#define parse_error(fmt, ...) \
    do { \
        has_parse_error = true; \
        fprintf(stderr, "[line: %d] error: ", line_num); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
        fprintf(stderr, ".\n"); \
    } while (0);

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
    struct value data_width;

    int value_list_sz;
    struct value *value_list;

    bool default_set;
    struct value default_val; /* use type.ft to determine type */

    int exclude_list_sz;
    char **exclude_list;

    char *start_attr;
    char *end_attr;
};

enum packet_type {
    PT_FIXED,
    PT_DYNAMIC,
    PT_CALCULATED,
};

struct packet {
    char *name;
    enum packet_type ptype;
    struct value size;
    struct value pipe;

    int option_list_sz;
    struct poption *option_list;
};

extern int packet_list_sz;
extern struct packet *packet_list;

void add_packet(enum packet_type ptype, char *name);
struct packet *get_curr_packet(void);
void free_packet(int idx);

void add_option(enum po_type otype);
struct poption *get_curr_option(void);
void option_add_name(struct packet *p, struct poption *o, char *name);

bool packet_name_is_unique(struct packet *p);
bool option_name_is_unique(struct packet *p, struct poption *o);

bool check_packet(int idx);
bool check_option(int pkt_idx, int idx);

char *packet_to_str(int pkt_idx);
char *option_to_str(int pkt_idx, int idx);
char *type_to_str(struct type t);

#endif /* PACKET_H */
