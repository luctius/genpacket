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

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;


    yyparse();

    if (has_parse_error) {
        parse_error(NULL, NULL, "too many errors; quitting");
        exit(EXIT_FAILURE);
    }

    printf("\n");
    for (int i = 0; i < packet_list_sz; i++) {
        printf("%s\n", packet_to_str(i) );
    }

    for (int i = 0; i < packet_list_sz; i++) {
        free_packet(i);
    }

    return (0);
}

