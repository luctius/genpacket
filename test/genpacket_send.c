#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "genpacket.h"

int main(int argc, char *argv[]) {
    struct genpacket_params params = {
        .rfds           = {  STDIN_FILENO, },
        .wfds           = {  STDOUT_FILENO, },
        .private_ctx    = NULL,
    };

    genpacket_init(&params);

    struct p0_fixed p0f = {
        .attribute0 = 10,
        .attribute1 = 2827,
        .attribute2 = 202116108,
        .data0      = { 842150450, },
        .data1      = { 5654, },
        .data2      = { 1.0f, },
        .data3      = { 'h', 'e', 'y', },
        .data4      = { 'j', 'o', 'w', } ,
        .data5      = { 'a', },
        .attribute3 = 1,
        .data6      = {2,3,},
    };

    struct p1_fixed p1f = {
        .attribute0 = 10,
        .attribute1 = 11,
        .attribute2 = 12,
        .data0      = { 1.0f, },
        .attribute3 = 10,
    };

    struct p2_fixed p2f = {
        .attribute0 = 10,
        .attribute1 = 11,
        .attribute2 = 12,
        .data0      = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, },
    };

    uint8_t array_p3c[] = {1,2,3,4,};
    struct p3_calculated p3c = {
        .attribute0 = 10,
        .sz2        = 3,
        .sz         = 4,
        .attribute1 = 10,
        .data0      = &array_p3c[0],
        .attribute2 = 10,
        .data1      = &array_p3c[0],
    };

    uint8_t array_p4c[] = {1,2,3,4,};
    struct p4_calculated p4c = {
        .attribute0 = 10,
        .sz2        = 2,
        .sz         = 4,
        .attribute1 = 10,
        .data0      = &array_p4c[0],
        .attribute2 = 10,
        .data1      = &array_p4c[0],
    };

    struct p5_fixed p5f = {
        .attribute0 = 60,
        .attribute1 = 1,
        .attribute2 = 0,
        .data0      = { 5, 7, },
    };

    if ( (argc > 0) && strcmp("-p0", argv[1]) ) {
        genpacket_p0_fixed_send(&p0f);
    }
    if ( (argc > 0) && strcmp("-p1", argv[1]) ) {
        genpacket_p1_fixed_send(&p1f);
    }
    else if ( (argc > 0) && strcmp("-p2", argv[1]) ) {
        genpacket_p2_fixed_send(&p2f);
    }
    else if ( (argc > 0) && strcmp("-p3", argv[1]) ) {
        genpacket_p3_calculated_send(&p3c);
    }
    else if ( (argc > 0) && strcmp("-p4", argv[1]) ) {
        genpacket_p4_calculated_send(&p4c);
    }
    else if ( (argc > 0) && strcmp("-p5", argv[1]) ) {
        genpacket_p5_fixed_send(&p5f);
    }
    else genpacket_p0_fixed_send(&p0f);

    return 0;
}
