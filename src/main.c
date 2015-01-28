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
#include "cmdline_packit.h"
#include "decoder.h"
#include "debug_print.h"

extern int yylex_destroy(void);
extern enum debug_levels global_debug_lvl;
extern FILE *yyin;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct gengetopt_args_info args_info;
    struct packit_args_info packit_args;
    FILE *p_fp;
    bool debug_flag = false;
    char * packet_desc = NULL;
    
    if (strcmp(basename(argv[0]), "genpacket") == 0) {
        if (cmdline_parser (argc, argv, &args_info) != 0)
                 exit(1);
        
        debug_flag = args_info.debug_flag;
        packet_desc = args_info.packetdesc_arg;
    } else if (strcmp(basename(argv[0]), "packit") == 0) {
        if (cmdline_packit_parser (argc, argv, &packit_args) != 0)
                 exit(1);
        packet_desc = packit_args.packetdesc_arg;
        debug_flag = packit_args.packetdesc_arg;
    }
    
    p_fp=fopen(packet_desc,"r");
    
    if(!p_fp) {
        gp_err("Couldn't open packet description file for reading\n");
        exit(0);
    }
    yyin = p_fp; 
    yyparse();
    yylex_destroy();
    if (debug_flag) {
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
        cmdline_parser_free (&args_info);
    }
    
    if (strcmp(basename(argv[0]), "packit") == 0) {
        gp_debug("Using packit command");
        FILE *i_fp;
        if (packit_args.input_arg[0] == '-')
            i_fp=stdin;
        else
            i_fp=fopen(packit_args.input_arg,"r");
        if(!i_fp) {
            gp_err("couldn't open input file %s for reading\n",packit_args.input_arg);
            exit(0);
        }
        decode_using_packet(packit_args.packetindex_arg,i_fp);
        fclose(i_fp);
        cmdline_packit_parser_free (&packit_args);
    }

    /* cleanup */
    for (int i = 0; i < packet_list_sz; i++) {
        free_packet(i);
    }
    
    free(packet_list);
    packet_list_sz = 0;
    
    

    return (0);
}

