#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

void cb_new_packet(enum packet_type ptype, char *name) {
    add_packet(ptype, name);
}

void cb_pa_size(int size) {
    struct packet *p = get_curr_packet();
    if (p->size != -1) fprintf(stderr, "warning: packet %s' size already declared\n", p->name);
    p->size = size;
}

void cb_pa_pipe(int pipe) {
    struct packet *p = get_curr_packet();
    if (p->pipe != -1) fprintf(stderr, "warning: packet %s' pipe already declared\n", p->name);
    p->pipe = pipe;
}

void cb_attr_option() {
    add_option(O_ATTRIBUTE);
}

void cb_frame_option(uint64_t frame) {
    add_option(O_FRAME);
}

void cb_size_option() {
    add_option(O_SIZE);
}

void cb_crc_option(char *method) {
    add_option(O_CRC);
}

void cb_data_option() {
    add_option(O_DATA);
}


void cb_op_name(char *name) {
    struct poption *o = get_curr_option();
    free(o->name);
    o->name = name;
}

void cb_op_type(struct type t) {
    struct poption *o = get_curr_option();
    o->type = t;
}

void cb_op_datawidth_type(struct type t) {
    struct poption *o = get_curr_option();
    o->data_width = t.ft_sz;
}

void cb_op_datawidth_int(uint64_t value) {
    struct poption *o = get_curr_option();
    o->data_width = value;
}

void cb_op_datasize_string(char *attr) {
    printf("(datasize: %s NI) ", attr);
}

void cb_op_datasize_int(uint64_t value) {
    printf("(datasize: %d NI) ", value);
}

void cb_op_default_int(uint64_t value){
    struct poption *o = get_curr_option();
    if (o->default_set == true) fprintf(stderr, "warning: option %s' default already declared\n", o->name);
    switch (o->type.ft) {
        case FT_SIGNED:
            o->default_val.i = (int64_t) value;
            break;
        case FT_UNSIGNED:
            o->default_val.u = value;
            break;
        case FT_FLOAT:
            o->default_val.i = (int64_t) value;
            fprintf(stderr, "error: using integer in an float option: %d\n", value);
            break;
        default: return; break;
    }

    o->default_set = true;
}

void cb_op_default_double(double value){
    struct poption *o = get_curr_option();
    if (o->default_set == true) fprintf(stderr, "warning: option %s' default already declared\n", o->name);
    switch (o->type.ft) {
        case FT_SIGNED:
            o->default_val.d = value;
            fprintf(stderr, "error: using float in an integer option: %f\n", value);
            break;
        case FT_UNSIGNED:
            o->default_val.d = value;
            fprintf(stderr, "error: using float in an integer option: %f\n", value);
            break;
        case FT_FLOAT:
            o->default_val.d = value;
            break;
        default: break;
    }

    o->default_set = true;
}

void cb_op_values_int(uint64_t value){
    struct poption *o = get_curr_option();
    o->value_list = realloc(o->value_list, ++o->value_list_sz * sizeof(union value) );
    union value *vp = &o->value_list[o->value_list_sz-1];
    switch (o->type.ft) {
        case FT_SIGNED:
            vp->i = (int64_t) value;
            break;
        case FT_UNSIGNED:
            vp->u = value;
            break;
        case FT_FLOAT:
            vp->i = (int64_t) value;
            fprintf(stderr, "error: using float in an integer option: %f\n", value);
            break;
        default: break;
    }
}

void cb_op_values_double(double value){
    struct poption *o = get_curr_option();
    o->value_list = realloc(o->value_list, ++o->value_list_sz * sizeof(union value) );
    union value *vp = &o->value_list[o->value_list_sz-1];
    switch (o->type.ft) {
        case FT_SIGNED:
            vp->d = value;
            fprintf(stderr, "error: using float in an integer option: %f\n", value);
            break;
        case FT_UNSIGNED:
            vp->d = value;
            fprintf(stderr, "error: using float in an integer option: %f\n", value);
            break;
        case FT_FLOAT:
            vp->d = value;
            break;
        default: break;
    }
}

void cb_op_start(char *string){
    struct poption *o = get_curr_option();
    o->start_attr = string;
}

void cb_op_end(char *string){
    struct poption *o = get_curr_option();
    o->end_attr = string;
}

void cb_op_exclude(char *string){
    struct poption *o = get_curr_option();
    o->exclude_list = realloc(o->exclude_list, ++o->exclude_list_sz * sizeof(union value) );
    o->exclude_list[o->exclude_list_sz-1] = string;
}

