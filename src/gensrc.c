#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "gensrc.h"

#include "skels/source_skeleton.h"
#include "skels/header.h"
#include "skels/structs.h"
#include "skels/receive_callback.h"
#include "skels/send_function.h"
#include "skels/var_declaration.h"

#define rnd_to_pwr2(sz) do { if ( (sz & (sz-1) ) != 0) { sz |= sz>>1; sz |= sz>>2; sz |= sz>>4; sz |= sz>>8; sz |= sz>>16; sz++; } } while (0)

const char *ctx = "ctx";
const char *recvbuf = "recv_buff";
const char *recvbuf_cqc = "recv_buff_cqc";
const char *sendbuf = "send_buff";
const char *sendbuf_cqc = "send_buff_cqc";

static char *type_to_c_type_str(struct type t) {
    static char bfr[20];
    int sz = 0;
    while (sz < t.ft_sz) sz += CHAR_BIT;

    switch (t.ft) {
    default:
    case FT_SIGNED:
        sprintf(bfr, "int%d_t", sz);
        break;
    case FT_UNSIGNED:
        sprintf(bfr, "uint%d_t", sz);
        break;
    case FT_FLOAT:
        if (sz == (sizeof(double) * CHAR_BIT) ) sprintf(bfr, "double");
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
    FIX_UNUSED(indent);
    FIX_UNUSED(record);

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
void generate_public_hdr(const char *path, const char *prefix, const char *ifndefname) {
    FIX_UNUSED(path);

    FILE *file = stdout;
    /*
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    FILE *f = fopen(fpath, "w");
    */

    struct header_gen_struct hgs;
    init_header_gen_struct(&hgs);
    hgs.prefix      = prefix;
    hgs.ifndefname  = ifndefname;
    hgs.packets     = NULL;
    hgs.receive_functions = NULL;
    hgs.send_functions = NULL;
    hgs.nro_pipes   = pipes;

    generate_header(file, &hgs, 0);
    fprintf(file, "\n");
}

void generate_receive_buffers(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    FIX_UNUSED(record);

    uint32_t sz = 0;
    for (int i = 0; i < packet_list_sz; i++) {
        uint32_t tsz = calculate_min_size(&packet_list[i]);
        sz = (tsz > sz) ? tsz : sz;
    }
    rnd_to_pwr2(sz);
    
    generatep_var_declaration(stream, indent, 1, recvbuf, 0, sz, "uint8_t");
    generatep_var_declaration(stream, indent, 0, recvbuf_cqc, 0, 0, "struct cqc");
}


void generate_send_buffers(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    FIX_UNUSED(record);

    uint32_t sz = 0;
    for (int i = 0; i < packet_list_sz; i++) {
        uint32_t tsz = calculate_min_size(&packet_list[i]);
        sz = (tsz > sz) ? tsz : sz;
    }
    rnd_to_pwr2(sz);
    
    generatep_var_declaration(stream, indent, 1, sendbuf, 0, sz, "uint8_t");
    generatep_var_declaration(stream, indent, 0, sendbuf_cqc, 0, 0, "struct cqc");
}

void generate_init_receive_buffers(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    FIX_UNUSED(record);

    char params_buf[ (4*2) + ( (strlen(recvbuf) +4) * 2) +10 ];
    sprintf(params_buf, "%s.%s,ARRAY_SZ(ctx.%s)", ctx, recvbuf_cqc, recvbuf);
    generatep_function_call(stream, indent, "cqc_init", params_buf);
}

void generate_init_send_buffers(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
}

void generate_public_src(const char *path, const char *prefix, const char *ifndefname) {
    FILE *file = stdout;
    /*
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.c", path, pheader);
    FILE *f = fopen(fpath, "w");
    */

    struct source_skeleton_gen_struct hgs;
    init_source_skeleton_gen_struct(&hgs);
    hgs.cmd_options = "<to be done>";
    hgs.genpacket   = "<to be done>";
    hgs.prefix      = prefix;
    hgs.ifndefname  = ifndefname;
    hgs.receive_buffers = NULL;
    hgs.send_buffers = NULL;
    hgs.version     = "1";

    generate_source_skeleton(file, &hgs, 0);
    fprintf(file, "\n");
}

void generate_packet_files(const char *path, const char *prefix) {
    /* capatilise gen_name */
    char ifndef_buf[strlen(prefix) +1];
    strcpy(ifndef_buf, prefix);
    for (int i = 0; ifndef_buf[i]; i++) ifndef_buf[i] = toupper(prefix[i]);

    generate_public_hdr(path, prefix, ifndef_buf);
    generate_public_src(path, prefix, ifndef_buf);
}
