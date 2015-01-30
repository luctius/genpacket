#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "debug_print.h"
#include "generate.h"

#include "genhdr.h"
#include "gensrc.h"
#include "gencqc.h"

void generate_packet_files(const char *path, const char *prefix) {
    /* capatilise gen_name */
    char ifndef_buf[strlen(prefix) +1];
    strcpy(ifndef_buf, prefix);
    for (int i = 0; ifndef_buf[i]; i++) ifndef_buf[i] = toupper(prefix[i]);


    FILE *hdr_file = stdout;
    char phdr[PATH_MAX];
    snprintf(phdr, PATH_MAX, "%s/%s.h", path, prefix);
    hdr_file = fopen(phdr, "w");

    FILE *src_file = stdout;
    char psrc[PATH_MAX];
    snprintf(psrc, PATH_MAX, "%s/%s.c", path, prefix);
    src_file = fopen(phdr, "w");

    FILE *cqc_file = stdout;
    char pcqc[PATH_MAX];
    snprintf(pcqc, PATH_MAX, "%s/%s_cqc.h", path, prefix);
    cqc_file = fopen(phdr, "w");



    gp_debug("generating header to %s", phdr);
    generate_public_hdr(hdr_file, prefix, ifndef_buf);

    gp_debug("generating source to %s", psrc);
    generate_public_src(src_file, prefix, ifndef_buf);

    gp_debug("generating cqc to %s", pcqc);
    generate_cqc_hdr(cqc_file, prefix, ifndef_buf);
}

