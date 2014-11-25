#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <stdbool.h>

enum po_type {
    O_FRAME,
    O_ATTRIBUTE,
    O_SIZE,
    O_DATA,
    O_CRC,
};

enum field_type {
    FT_SIGNED,
    FT_UNSIGNED,
    FT_FLOAT,
};

union value {
    int64_t i;
    uint64_t u;
    double d;
};
/*
struct value {
    enum field_type ft;
    union {
        int64_t i;
        uint64_t u;
        double d;
    }
};
*/

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
    int data_width;

    int value_list_sz;
    union value *value_list;

    bool default_set;
    union value default_val; /* use type.ft to determine type */

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
    int size;
    int pipe;

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

bool check_packet(int idx);
bool check_option(int pkt_idx, int idx);

char *packet_to_str(int pkt_idx);
char *option_to_str(int pkt_idx, int idx);

char *value_to_str(struct type t, union value v);
char *type_to_str(struct type t);

#endif /* PACKET_H */
