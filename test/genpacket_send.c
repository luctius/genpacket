#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "genpacket.h"

void genpacket_p0_fixed_received(struct p0_fixed *packet, void *private_ctx) {
    printf("hello world\n");
}

int main() {
    struct genpacket_params params = {
        .rfds = {  STDIN_FILENO, },
        .wfds = {  STDOUT_FILENO, },
        .private_ctx = NULL,
    };

    genpacket_init(&params);
    //genpacket_process(21);

    printf("bla");

    return 0;
}
