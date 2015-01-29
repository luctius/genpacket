#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "genpacket.h"

int main() {
    struct genpacket_params params = {
        .rfds = {  STDIN_FILENO, },
        .wfds = {  STDOUT_FILENO, },
        .private_ctx = NULL,
    };

    genpacket_init(&params);

    struct p0_fixed p0f = {
        .frame0 = 0x10,
        .attribute0 = 10,
        .attribute1 = 2827,
        .attribute2 = 202116108,
        .data0 = { 842150450, },
        .data1 = { 5654, },
        .data2 = { 1.0f, },
        .data3 = { 'h', 'e', 'y', },
        .data4 = { 'j', 'o', 'w', } ,
        .frame1 = 0x30,
        .data5 = { 'a', },
        .attribute3 = 1,
        .data6 = {2,3,},
        .crc0 = 193,
        .frame2 = 0x20,
    };
    genpacket_p0_fixed_send(&p0f);

    return 0;
}
