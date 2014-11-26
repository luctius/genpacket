#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "packet.h"
#include "gensrc.h"

#define generate_line(fmt, ...) do { printf(fmt, ##__VA_ARGS__); } while (0)
void generate_public_hdr(const char *path, const char *prefix) {
    char fpath[PATH_MAX];
    char pheader[NAME_MAX];

    snprintf(pheader, NAME_MAX, "/%s_gpacket", prefix);
    snprintf(fpath, PATH_MAX, "%s/%s.h", path, pheader);
    FILE *f = fopen(fpath, "w");

    generate_line("#ifndef %s_h_\n", pheader);
    generate_line("#define %s_h_\n", pheader);

    generate_line("\n");
    for (int i = 0; i < packet_list_sz; i++) {
        struct packet *p = &packet_list[i];
        generate_line("struct %s_%s {\n", prefix, p->name); {
            for (int j = 0; j < p->option_list_sz; j++) {
                struct poption *o = &p->option_list[j];

                generate_line("\t%s %s;\n", type_to_str(o->type), o->name);

            } generate_line("};\n");
        }
    }

    generate_line("#endif /*%s_h_*/\n", pheader);
}

void generate_src(const char *path, const char *prefix) {
    generate_public_hdr(path, prefix);
}
