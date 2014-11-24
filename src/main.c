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

#include "genpacket.tab.h"

int main(int argc, char **argv) {
    while (yylex() != 0) {}
    return (0);
}

