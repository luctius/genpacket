/*
 * File automatically generated by
 * gengen  by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#ifndef SEND_FUNCTION_DECL_GEN_STRUCT_H
#define SEND_FUNCTION_DECL_GEN_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct send_function_decl_gen_struct
{
  const char *packet_name;
  const char *prefix;
};

void
generate_send_function_decl(FILE *stream, struct send_function_decl_gen_struct *record, unsigned int indent);

void
generatep_send_function_decl(FILE *stream, unsigned int indent, const char *packet_name, const char *prefix);

char *
genstring_send_function_decl(struct send_function_decl_gen_struct *record, unsigned int indent);

char *
genstringp_send_function_decl(unsigned int indent, const char *packet_name, const char *prefix);

int
strcnt_send_function_decl(struct send_function_decl_gen_struct *record, unsigned int indent);

void
init_send_function_decl_gen_struct(struct send_function_decl_gen_struct *record);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // SEND_FUNCTION_DECL_GEN_STRUCT_H
