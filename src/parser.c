#include <stdlib.h>
#include "parser.h"

void cb_new_packet(enum packet_type ptype, char *name) {
    switch(ptype) {
        case PT_FIXED: printf("fixed "); break;
        case PT_DYNAMIC: printf("dynamic "); break;
        case PT_CALCULATED: printf("calculated "); break;
    }
    printf("packet %s", name);
}

void cb_pa_size(int size) {
    printf(" (size: %d)", size);
}

void cb_pa_pipe(int pipe) {
    printf(" (pipe: %d)", pipe);
}

void cb_attr_option() {
    printf("\nattribute ");
}

void cb_frame_option(int64_t frame) {
    printf("\nframe ");
}

void cb_size_option() {
    printf("\nsize ");
}

void cb_crc_option(char *method) {
    printf("\ncrc ");
}

void cb_data_option() {
    printf("\ndata ");
}


void cb_op_name(char *name) {
    printf("(name: %s) ", name);
}

void cb_op_type(struct type t) {
    printf("(type: %s) ", type_to_str(t) );
}

void cb_op_datawidth(struct type t) {
    printf("(datawidth: %s) ", type_to_str(t) );
}

void cb_op_datasize_string(char *attr) {
    printf("(datasize: %s) ", attr);
}

void cb_op_datasize_int(int64_t value) {
    printf("(datasize: %d) ", value);
}

void cb_op_default_int(int64_t value){
    printf("(default: %d) ", value);
}

void cb_op_default_double(double value){
    printf("(default: %f) ", value);
}

void cb_op_values_int(int64_t value){
    printf("(default: %d) ", value);
}

void cb_op_values_double(double value){
    printf("(default: %f) ", value);
}

void cb_op_start(char *string){
    printf("(start: %s) ", string);
}

void cb_op_end(char *string){
    printf("(start: %s) ", string);
}

void cb_op_exclude(char *string){
    printf("(exclude: %s) ", string);
}

