#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "packet.h"
#include "gensrc.h"

static char *type_to_c_type_str(struct type t) {
    static char bfr[20];
    int sz = 0;
    while (sz < t.ft_sz) sz += 8;

    switch (t.ft) {
    default:
    case FT_SIGNED:
        sprintf(bfr, "int%d", sz);
        break;
    case FT_UNSIGNED:
        sprintf(bfr, "uint%d", sz);
        break;
    case FT_FLOAT:
        if (sz == 8) sprintf(bfr, "double");
        else sprintf(bfr, "float");
        break;
    }
    return bfr;
}

#define generate_line(fmt, ...) do { printf(fmt, ##__VA_ARGS__); } while (0)
void generate_public_hdr(const char *path, const char *prefix) {
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    FILE *f = fopen(fpath, "w");

}

void generate_src(const char *path, const char *prefix) {
    generate_public_hdr(path, prefix);
}
