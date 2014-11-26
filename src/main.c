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

#include "packet.h"
#include "genpacket.tab.h"
#include "gensrc.h"

extern int yylex_destroy(void);

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    yyparse();
    yylex_destroy();
    if (has_parse_error) {
        parse_error(NULL, NULL, "too many errors; quitting");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < packet_list_sz; i++) {
        char *s = packet_to_str(i) ;
        printf("%s\n", s);
        free(s);
    }

    if (strcmp(argv[0], "genpacket") == 0) generate_src("./", "test");


    /* cleanup */
    for (int i = 0; i < packet_list_sz; i++) {
        free_packet(i);
    }
    free(packet_list);
    packet_list_sz = 0;

    return (0);
}

