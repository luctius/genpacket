#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "packet.h"

char tos_bfr[12];
char *type_to_str(struct type t) {
    switch (t.ft) {
    case FT_SIGNED:
        sprintf(tos_bfr, "int%d", t.ft_sz);
        break;
    case FT_UNSIGNED:
        sprintf(tos_bfr, "uint%d", t.ft_sz);
        break;
    case FT_FLOAT:
        sprintf(tos_bfr, "float%d", t.ft_sz);
        break;
    default:
        sprintf(tos_bfr, "default");
    }
    return tos_bfr;
}
