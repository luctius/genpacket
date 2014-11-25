#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "packet.h"

#define TS_BFR_LEN 256

int packet_list_sz = 0;
struct packet *packet_list = NULL;

void add_packet(enum packet_type ptype, char *name) {
    packet_list = realloc(packet_list, ++packet_list_sz * sizeof(struct packet) );
    assert(packet_list != NULL);

    struct packet *p = get_curr_packet();
    p->name = name;
    p->ptype = ptype;

    p->size = v_set_i(-1);
    p->pipe = v_set_i(-1);
    p->option_list_sz = 0;
    p->option_list = NULL;

    if (!packet_name_is_unique(p) ) {
        fprintf(stderr, "error: packet (%s) allready exists\n", p->name);
    }
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
    o->otype = otype;

    o->default_set = false;

    o->value_list_sz = 0;
    o->value_list = NULL;

    o->exclude_list_sz = 0;
    o->exclude_list = NULL;

    char *defname;
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

struct poption *get_curr_option(void) {
    if (packet_list_sz <= 0) return NULL;
    struct packet *p = get_curr_packet();
    if (p->option_list_sz == 0) return NULL;

    return &p->option_list[p->option_list_sz-1];
}

bool check_packet(int idx){
    if (packet_list_sz <= idx) return false;
    struct packet *p = &packet_list[idx];

    return false;
}

bool check_option(int pkt_idx, int idx){
    if (packet_list_sz <= idx) return false;
    struct packet *p = &packet_list[pkt_idx];

    return false;
}

char *packet_to_str(int pkt_idx) {
    int size = 400;
    char *ts_bfr = malloc(size * sizeof(char) );

    struct packet *p = &packet_list[pkt_idx];

    int ctr = 0;
    char *sp = ts_bfr;
    ctr += sprintf(&sp[ctr], "packet %s ", p->name);
    ctr += sprintf(&sp[ctr], "[size: %s] ", v_to_str(p->size) );
    ctr += sprintf(&sp[ctr], "[pipe: %s] ", v_to_str(p->pipe) );
    ctr += sprintf(&sp[ctr], "{\n");

        for (int i = 0; i < p->option_list_sz; i++) {
            ctr += sprintf(&sp[ctr], "\t%s\n", option_to_str(pkt_idx, i) );
            if (ctr > (size * 0.75) ) {
                size += 400;
                ts_bfr = realloc(ts_bfr, size * sizeof(char) );
            }
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
    ctr += sprintf(&sp[ctr], "[data_width: %s] ", v_to_str(o->data_width) );

    if (o->default_set) ctr += sprintf(&sp[ctr], "[default: %s] ", v_to_str(o->default_val) );

    if (o->exclude_list_sz > 0) {
        ctr += sprintf(&sp[ctr], "exclude: ");
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
