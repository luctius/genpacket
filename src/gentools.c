#include "skels/cqc.h"

void generate_tools_header(FILE *stream, const char *prefix, const char *ifndefname) {
    generatep_tools_hdr(stream, 0, ifndefname, 0, prefix);
    fprintf(stream, "\n");
}

void generate_tools_source(FILE *stream, const char *prefix, const char *ifndefname) {
    generatep_tools_src(stream, 0, prefix);
    fprintf(stream, "\n");
}

