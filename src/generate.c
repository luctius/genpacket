#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

#include "packet.h"
#include "debug_print.h"
#include "generate.h"

#include "genhdr.h"
#include "gensrc.h"
#include "gentools.h"

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
    src_file = fopen(psrc, "w");

    FILE *tools_hdr_file = stdout;
    char ptools_hdr[PATH_MAX];
    snprintf(ptools_hdr, PATH_MAX, "%s/%s_tools.h", path, prefix);
    tools_hdr_file = fopen(ptools_hdr, "w");

    FILE *tools_src_file = stdout;
    char ptools_src[PATH_MAX];
    snprintf(ptools_src, PATH_MAX, "%s/%s_tools.c", path, prefix);
    tools_src_file = fopen(ptools_src, "w");

    gp_debug("generating header to %s", phdr);
    generate_public_hdr(hdr_file, prefix, ifndef_buf);
    fclose(hdr_file);

    gp_debug("generating source to %s", psrc);
    generate_public_src(src_file, prefix, ifndef_buf);
    fclose(src_file);

    gp_debug("generating tools header to %s", ptools_hdr);
    generate_tools_header(tools_hdr_file, prefix, ifndef_buf);
    fclose(tools_hdr_file);

    gp_debug("generating tools source to %s", ptools_src);
    generate_tools_source(tools_src_file, prefix, ifndef_buf);
    fclose(tools_src_file);
}

