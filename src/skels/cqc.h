/*
 * File automatically generated by
 * gengen  by Lorenzo Bettini 
 * http://www.gnu.org/software/gengen
 */

#ifndef CQC_GEN_STRUCT_H
#define CQC_GEN_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cqc_gen_struct
{
  const char *ifndefname;
  short modulo;
};

void
generate_cqc(FILE *stream, struct cqc_gen_struct *record, unsigned int indent);

void
generatep_cqc(FILE *stream, unsigned int indent, const char *ifndefname, short modulo);

char *
genstring_cqc(struct cqc_gen_struct *record, unsigned int indent);

char *
genstringp_cqc(unsigned int indent, const char *ifndefname, short modulo);

int
strcnt_cqc(struct cqc_gen_struct *record, unsigned int indent);

void
init_cqc_gen_struct(struct cqc_gen_struct *record);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // CQC_GEN_STRUCT_H
