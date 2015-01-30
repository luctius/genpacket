#include "skels/cqc.h"

void generate_cqc_hdr(FILE *stream, const char *prefix, const char *ifndefname) {
    generatep_cqc(stream, 0, ifndefname, 0);
    fprintf(stream, "\n");
}

