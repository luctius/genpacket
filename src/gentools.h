#ifndef GENCQC_H
#define GENCQC_H


void generate_tools_header(FILE *stream, const char *prefix, const char *ifndefname);
void generate_tools_source(FILE *stream, const char *prefix, const char *ifndefname);

#endif /* GENCQC_H */
