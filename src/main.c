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
extern FILE *yyin;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    struct gengetopt_args_info args_info;
    
    if (cmdline_parser (argc, argv, &args_info) != 0)
             exit(1);
    
    FILE *p_fp=fopen(args_info.packetdesc_arg,"r");
    if(!p_fp) {
        printf("couldn't open packet description file for reading\n");
        exit(0);
    }
    yyin = p_fp; 
    yyparse();
    yylex_destroy();
    for (int i = 0; i < packet_list_sz; i++) {
        char *s = packet_to_str(i) ;
        printf("%s\n", s);
        free(s);
    }
    free(p_fp);
    if (has_parse_error) {
        parse_error(NULL, NULL, "too many errors; quitting");
        exit(EXIT_FAILURE);
    }

    if (strcmp(basename(argv[0]), "genpacket") == 0) {
        generate_packet_files("./", "genpacket");
    }
    
    if (strcmp(basename(argv[0]), "packit") == 0) {
        gp_debug("Using packit command");
        FILE *i_fp=fopen(args_info.input_arg,"r");
        if(!i_fp) {
            printf("couldn't open packet description file for reading\n");
            exit(0);
        }
        decode_using_packet(0,i_fp);
        free(i_fp);
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

