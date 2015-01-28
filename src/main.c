#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <libgen.h>

#include "packet.h"
#include "genpacket.tab.h"
#include "gensrc.h"
#include "cmdline.h"
#include "decoder.h"
#include "debug_print.h"

extern int yylex_destroy(void);
extern enum debug_levels global_debug_lvl;
extern FILE *yyin;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct gengetopt_args_info args_info;
    
    if (cmdline_parser (argc, argv, &args_info) != 0)
             exit(1);
    
    FILE *p_fp=fopen(args_info.packetdesc_arg,"r");
    if(!p_fp) {
        gp_err("Couldn't open packet description file %s for reading\n",args_info.packetdesc_arg);
        exit(0);
    }
    yyin = p_fp; 
    yyparse();
    yylex_destroy();
    if (args_info.debug_flag) {
        global_debug_lvl = GP_DEBUG_LEVEL_DEBUG;
        for (int i = 0; i < packet_list_sz; i++) {
            char *s = packet_to_str(i) ;
            printf("%s\n", s);
            free(s);
        }
    } else {
        global_debug_lvl = GP_DEBUG_LEVEL_INFORMATIONAL;
    }
    fclose(p_fp);
    if (has_parse_error) {
        parse_error(NULL, NULL, "too many errors; quitting");
        exit(EXIT_FAILURE);
    }

    if (strcmp(basename(argv[0]), "genpacket") == 0) {
        generate_packet_files(args_info.output_path_arg, "genpacket");
    }
    
    if (strcmp(basename(argv[0]), "packit") == 0) {
        gp_debug("Using packit command");
        FILE *i_fp;
        if (args_info.input_arg[0] == '-')
            i_fp=stdin;
        else
            i_fp=fopen(args_info.input_arg,"r");
        if(!i_fp) {
            gp_err("couldn't open input file %s for reading\n",args_info.input_arg);
            exit(0);
        }
        decode_using_packet(args_info.packetindex_arg,i_fp);
        fclose(i_fp);
    }

    /* cleanup */
    for (int i = 0; i < packet_list_sz; i++) {
        free_packet(i);
    }
    
    free(packet_list);
    packet_list_sz = 0;
    cmdline_parser_free (&args_info);

    return (0);
}

