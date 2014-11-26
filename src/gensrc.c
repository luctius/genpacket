#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "packet.h"
#include "gensrc.h"

#define generate_line(fmt, ...) do { fprintf(f, fmt, ##__VA_ARGS__); } while (0)
void generate_public_hdr(char *path, char *prefix) {
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "/%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    FILE *f = fopen(fpath, "w");

    generate_line("#ifndef %s_h_\n");
    generate_line("#define %s_h_\n");


    generate_line("#endif /*%s_h_*/\n");
}

void generate_src(char *path, char *prefix) {
    generate_public_hdr(path, prefix);
}
