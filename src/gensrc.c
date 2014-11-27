#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "gensrc.h"

#include "skels/header.h"
#include "skels/structs.h"
#include "skels/receive_callback.h"
#include "skels/send_function.h"
#include "skels/var_declaration.h"

static char *type_to_c_type_str(struct type t) {
    static char bfr[20];
    int sz = 0;
    while (sz < t.ft_sz) sz += 8;

    switch (t.ft) {
    default:
    case FT_SIGNED:
        sprintf(bfr, "int%d_t", sz);
        break;
    case FT_UNSIGNED:
        sprintf(bfr, "uint%d_t", sz);
        break;
    case FT_FLOAT:
        if (sz == 8) sprintf(bfr, "double");
        else sprintf(bfr, "float");
        break;
    }
    return bfr;
}

void generate_struct_member(FILE *stream, struct structs_gen_struct *record, unsigned int indent) {
    struct packet *p = NULL;
    for (int j = 0; j < packet_list_sz; j++) {
        p = &packet_list[j];
        if (strcmp(record->struct_name, p->name) == 0) break;
    }

    if (p == NULL) {
        fprintf(stderr, "error: unable to find packet: %s\n", record->struct_name);
        return;
    }

    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *o = &p->option_list[i];
        if (o->otype == O_DATA) {
            if (o->data_size_i > 0) generatep_var_declaration(stream, indent, 1, o->name, 0, o->data_size_i, type_to_c_type_str(o->type) );
            else generatep_var_declaration(stream, indent, 0, o->name, 1, 0, type_to_c_type_str(o->type) );
        }
        else generatep_var_declaration(stream, indent, 0, o->name, 0, 0, type_to_c_type_str(o->type) );
    }
}

void generate_packets(FILE *stream, struct header_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];
        generatep_structs(stream, indent, NULL, p->name);
        fprintf(stream, "\n");
    }
}

void generate_receive_functions(FILE *stream, struct header_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];
        generatep_receive_callback(stream, indent, p->name, record->prefix, "void");
    }
}

void generate_send_functions(FILE *stream, struct header_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];
        generatep_send_function(stream, indent, p->name, record->prefix);
    }
}

#define generate_line(fmt, ...) do { printf(fmt, ##__VA_ARGS__); } while (0)
void generate_public_hdr(const char *path, const char *prefix) {
    /*
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    FILE *f = fopen(fpath, "w");
    */


    struct header_gen_struct hgs;
    init_header_gen_struct(&hgs);
    hgs.prefix = prefix;
    hgs.packets = NULL;
    hgs.receive_functions = NULL;
    hgs.send_functions = NULL;
    hgs.nro_pipes = packet_list_sz;

    /* capatilise gen_name */
    char ifndef_buf[strlen(hgs.prefix) +1];
    strcpy(ifndef_buf, hgs.prefix);
    for (int i = 0; ifndef_buf[i]; i++) ifndef_buf[i] = toupper(hgs.prefix[i]);
    hgs.ifndefname = ifndef_buf;

    generate_header(stdout, &hgs, 0);
    printf("\n");
}

void generate_src(const char *path, const char *prefix) {
    generate_public_hdr(path, prefix);
}
