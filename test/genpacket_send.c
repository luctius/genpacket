#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "genpacket.h"

void genpacket_p0_fixed_received(struct p0_fixed *packet, void *private_ctx) {
    printf("hello world\n");
}

int main() {
    struct genpacket_params params = {
        .pipe_fds = {  STDIN_FILENO, },
        .pipe_dir = { GENPACKET_R, },
        .private_ctx = NULL,
    };

    genpacket_init(&params);
    //genpacket_process(21);

    printf("bla");

    return 0;
}
