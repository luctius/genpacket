#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "gensrc.h"

#include "skels/send_func_impl.h"
#include "skels/source_skeleton.h"
#include "skels/write_data.h"
#include "skels/calculated_packet_test_function_impl.h"
#include "skels/dynamic_packet_test_function_impl.h"
#include "skels/fixed_packet_no_frame_test_function_impl.h"
#include "skels/fixed_packet_test_function_impl.h"

#define rnd_to_pwr2(sz) do { if ( (sz & (sz-1) ) != 0) { sz |= sz>>1; sz |= sz>>2; sz |= sz>>4; sz |= sz>>8; sz |= sz>>16; sz++; } } while (0)

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
            generatep_write_data(stream, indent, true, o->name, 0, record->prefix, (o->data_width / CHAR_BIT) );
        }
        else {
            if (o->otype == O_FRAME) {
                fprintf(stream, "\n%*s", indent, "");
                fprintf(stream, "packet->%s = %s;\n", o->name, v_to_str(o->frame_val) );
                fprintf(stream, "%*s", indent, "");
            }

            generatep_write_data(stream, indent, false, o->name, 0, record->prefix, (o->data_width / CHAR_BIT) );
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

void generate_send_functions_impl(FILE *stream, struct source_skeleton_gen_struct *record, unsigned int indent) {
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];

        generatep_send_func_impl(stream, indent, p->name, record->prefix, NULL);
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

void generate_public_src(FILE *stream, const char *prefix, const char *ifndefname) {
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

    generatep_source_skeleton(stream, 0, NULL, "TBD", "TBD", ifndefname, prefix, min_sz, NULL, total_sz, NULL, "1");
    fprintf(stream, "\n");
}
