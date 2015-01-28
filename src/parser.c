#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

void cb_new_packet(enum packet_type ptype, char *name) {
    add_packet(ptype, name);
}

void cb_pa_size(struct value v){
    struct packet *p = get_curr_packet();
    if (p->size != 0) parse_error(p, NULL, "\"size\" already declared");
    p->size = v_get_i(v);
}

void cb_pa_pipe(struct value v) {
    struct packet *p = get_curr_packet();
    if (p->pipe != -1) parse_error(p, NULL, "\"pipe\" already declared");
    p->pipe = v_get_i(v);
}

void cb_attr_option() {
    add_option(O_ATTRIBUTE);
}

void cb_frame_option(struct value v) {
    add_option(O_FRAME);

    struct poption *o = get_curr_option();
    o->frame_val = v;
}

void cb_size_option() {
    add_option(O_SIZE);
}

void cb_crc_option(char *method) {
    add_option(O_CRC);

    struct poption *o = get_curr_option();
    o->crc_method = method;
}

void cb_data_option() {
    add_option(O_DATA);
}

void cb_op_name(char *name) {
    struct packet *p = get_curr_packet();
    struct poption *o = get_curr_option();
    option_add_name(p, o, name);
}

void cb_op_type(struct type t) {
    struct poption *o = get_curr_option();
    o->type = t;
}

void cb_op_datawidth_type(struct type t) {
    struct poption *o = get_curr_option();
    o->data_width = t.ft_sz;
}

void cb_op_datawidth_v(struct value v) {
    struct poption *o = get_curr_option();
    o->data_width = v_get_i(v);
}

void cb_op_datasize_string(char *attr) {
    struct poption *o = get_curr_option();
    o->data_size_str = attr;
    o->data_size_i = 0;
}

void cb_op_datasize_v(struct value v) {
    struct poption *o = get_curr_option();
    o->data_size_i = v_get_i(v);
}

void cb_op_xor_in_v(struct value v) {
    struct packet *p = get_curr_packet();
    p->crc_xor_in = v_get_u(v);
}

void cb_op_xor_out_v(struct value v) {
    struct packet *p = get_curr_packet();
    p->crc_xor_out = v_get_u(v);
}

void cb_op_reflect_in_b(bool b) {
    struct packet *p = get_curr_packet();
    p->crc_reflect_in = b;
}

void cb_op_reflect_out_b(bool b) {
    struct packet *p = get_curr_packet();
    p->crc_reflect_out = b;
}

void cb_op_poly_v(struct value v) {
    struct packet *p = get_curr_packet();
    p->crc_poly = v_get_i(v);
}

void cb_op_default_v(struct value v){
    struct packet *p = get_curr_packet();
    struct poption *o = get_curr_option();
    if (o->default_set == true) parse_error(p,o,"option default already declared");
    o->default_val = v;
    o->default_set = true;
}

void cb_op_values_v(struct value v){
    struct poption *o = get_curr_option();
    o->value_list = realloc(o->value_list, ++o->value_list_sz * sizeof(struct value) );
    o->value_list[o->value_list_sz-1] = v;
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
    o->exclude_list = realloc(o->exclude_list, ++o->exclude_list_sz * sizeof(char *) );
    o->exclude_list[o->exclude_list_sz-1] = string;
}

