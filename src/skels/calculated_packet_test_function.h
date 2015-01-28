/*
 * File automatically generated by
 * gengen 1.4 by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#ifndef CALCULATED_PACKET_TEST_FUNCTION_GEN_STRUCT_H
#define CALCULATED_PACKET_TEST_FUNCTION_GEN_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct calculated_packet_test_function_gen_struct
{
  const char *name;
  int packet_size;
  const char *prefix;
  const char *test_frame_calc;
};

extern void generate_test_frame_calc(FILE *stream, struct calculated_packet_test_function_gen_struct *record, unsigned int indent);


void
generate_calculated_packet_test_function(FILE *stream, struct calculated_packet_test_function_gen_struct *record, unsigned int indent);

void
generatep_calculated_packet_test_function(FILE *stream, unsigned int indent, const char *name, int packet_size, const char *prefix, const char *test_frame_calc);

char *
genstring_calculated_packet_test_function(struct calculated_packet_test_function_gen_struct *record, unsigned int indent);

char *
genstringp_calculated_packet_test_function(unsigned int indent, const char *name, int packet_size, const char *prefix, const char *test_frame_calc);

int
strcnt_calculated_packet_test_function(struct calculated_packet_test_function_gen_struct *record, unsigned int indent);

void
init_calculated_packet_test_function_gen_struct(struct calculated_packet_test_function_gen_struct *record);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // CALCULATED_PACKET_TEST_FUNCTION_GEN_STRUCT_H
