#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "gensrc.h"

#include "skels/assignment.h"
#include "skels/calculated_packet_test_function_impl.h"
#include "skels/cqc.h"
#include "skels/dynamic_packet_test_function_impl.h"
#include "skels/fixed_packet_no_frame_test_function_impl.h"
#include "skels/fixed_packet_test_function_impl.h"
#include "skels/function_call.h"
#include "skels/header.h"
#include "skels/receive_callback_decl.h"
#include "skels/send_func_impl.h"
#include "skels/send_function_decl.h"
#include "skels/source_skeleton.h"
#include "skels/structs.h"
#include "skels/var_declaration.h"
#include "skels/write_data.h"

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
        generatep_receive_callback_decl(stream, indent, p->name, record->prefix, "void");
    }
}

void generate_send_functions(FILE *stream, struct header_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];
        generatep_send_function_decl(stream, indent, p->name, record->prefix);
    }
}

#define generate_line(fmt, ...) do { printf(fmt, ##__VA_ARGS__); } while (0)
void generate_public_hdr(const char *path, const char *prefix, const char *ifndefname) {
    FIX_UNUSED(path);

    FILE *file = stdout;
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    file = fopen(fpath, "w");

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

extern void generate_test_frame_dynamic(FILE *stream, struct dynamic_packet_test_function_impl_gen_struct *record, unsigned int indent) {
    FIX_UNUSED(record);
    FIX_UNUSED(indent);

    fprintf(stream, "return -1;");
}

extern void generate_test_frame_calc(FILE *stream, struct calculated_packet_test_function_impl_gen_struct *record, unsigned int indent) {
    FIX_UNUSED(record);
    FIX_UNUSED(indent);

    fprintf(stream, "return -1;");
}

extern void generate_test_frame_fixed(FILE *stream, struct fixed_packet_test_function_impl_gen_struct *record, unsigned int indent) {
    struct packet *p = NULL;
    for (int j = 0; j < packet_list_sz; j++) {
        p = &packet_list[j];
        if (strcmp(record->name, p->name) == 0) break;
    }

    if (p == NULL) {
        fprintf(stderr, "error: unable to find packet: %s\n", record->name);
        return;
    }
    fprintf(stream, "\n");

    for (int i = 0; i < p->option_list_sz; i++) {
        struct poption *o = &p->option_list[i];
        if (o->otype == O_FRAME) {
            if (v_is_float(o->frame_val) ) {
                fprintf(stream, "%*s" "if (packet->%s != %f) return -1;\n", indent, "", o->name, v_get_f(o->frame_val) );
            }
            else if (v_is_unsigned(o->frame_val) ) {
                fprintf(stream, "%*s" "if (packet->%s != 0x%" PRIx64 ") return -1;\n", indent, "", o->name, v_get_u(o->frame_val) );
            }
            else if (v_is_int(o->frame_val) ) {
                fprintf(stream, "%*s" "if (packet->%s != %" PRIi64 ") return -1;\n", indent, "", o->name, v_get_i(o->frame_val) );
            }
            else if (v_is_char(o->frame_val) ) {
                fprintf(stream, "%*s" "if (packet->%s != %c) return -1;\n", indent, "", o->name, v_get_c(o->frame_val) );
            }
        }
    }
}

void generate_test_packets(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];

        int min_sz = calculate_min_size(p);
        if (p->ptype == PT_FIXED) {
            bool frame_found = false;
            for (int j = 0; j < p->option_list_sz; j++) {
                struct poption *o = &p->option_list[j];
                if (o->otype == O_FRAME) {
                    frame_found = true;
                }
            }
            if (frame_found) {
                generatep_fixed_packet_test_function_impl(stream, indent, p->name, min_sz, record->prefix, NULL);
            }
            else generatep_fixed_packet_no_frame_test_function_impl(stream, indent, p->name, min_sz, record->prefix);
        }
        else if (p->ptype == PT_DYNAMIC) {
            generatep_dynamic_packet_test_function_impl(stream, indent, p->name, min_sz, record->prefix, NULL);
        }
        else if (p->ptype == PT_CALCULATED) {
            generatep_calculated_packet_test_function_impl(stream, indent, p->name, min_sz, record->prefix, NULL);
        }
    }
}

void generate_call_recv_tests(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    fprintf(stream, "\n");

    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];

        fprintf(stream, "%*s" "if (size == -1) size = (test_%s(&%s_ctx, head) );\n", indent, "", p->name, record->prefix);
    }
}

void generate_send_func(FILE *stream, struct send_func_impl_gen_struct *record, unsigned int indent) {
    struct packet *p = NULL;
    for (int j = 0; j < packet_list_sz; j++) {
        p = &packet_list[j];
        if (strcmp(record->packet_name, p->name) == 0) break;
    }

    if (p == NULL) {
        fprintf(stderr, "error: unable to find packet: %s\n", record->packet_name);
        return;
    }


    for (int j = 0; j < p->option_list_sz; j++) {
        struct poption *o = &p->option_list[j];
        if (o->otype == O_DATA) {
            generatep_write_data(stream, indent, o->name, 0, record->prefix, (o->data_width / CHAR_BIT) * o->data_size_i);
        }
        else generatep_write_data(stream, indent, o->name, 0, record->prefix, (o->data_width / CHAR_BIT) );
    }
}

void generate_send_functions_impl(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];

        generatep_send_func_impl(stream, indent, p->name, record->prefix, NULL);
    }
}

void generate_public_src(const char *path, const char *prefix, const char *ifndefname) {
    FILE *file = stdout;
    
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.c", path, pheader);
    file = fopen(fpath, "w");

    uint32_t min_sz = UINT32_MAX;
    for (int i = 0; i < packet_list_sz; i++) {
        uint32_t tsz = calculate_min_size(&packet_list[i]);
        if (tsz < min_sz) min_sz = tsz;
    }
    uint32_t sz = 0;
    for (int i = 0; i < packet_list_sz; i++) {
        uint32_t tsz = calculate_min_size(&packet_list[i]);
        if (tsz > sz) sz = tsz;
    }
    uint32_t total_sz = sz+min_sz;
    rnd_to_pwr2(min_sz);
    rnd_to_pwr2(total_sz);

    generatep_cqc(file, 0, ifndefname, 0);
    generatep_source_skeleton(file, 0, NULL, "TBD", "TBD", ifndefname, prefix, min_sz, NULL, total_sz, NULL, "1");
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

