#ifndef PARSER_H
#define PARSER_H

#include "packet.h"

void cb_new_packet(enum packet_type ptype, char *name);
void cb_pa_size(int size);
void cb_pa_pipe(int pipe);

void cb_frame_option(int64_t frame);
void cb_attr_option(void);
void cb_size_option(void);
void cb_crc_option(char *method);
void cb_data_option(void);
void cb_op_name(char *name);

void cb_op_type(struct type t);
void cb_op_datawidth(struct type t);
void cb_op_datasize_string(char *attr);
void cb_op_datasize_int(int64_t value);
void cb_op_default_int(int64_t value);
void cb_op_default_double(double value);
void cb_op_values_int(int64_t value);
void cb_op_values_double(double value);
void cb_op_start(char *attr);
void cb_op_end(char *attr);
void cb_op_exclude(char *attr);

#endif /* PARSER_H */
