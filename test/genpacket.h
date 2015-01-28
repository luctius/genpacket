#ifndef GENPACKET_H
#define GENPACKET_H

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GENPACKET_NRO_PIPES (1)

enum genpacket_direction {
    GENPACKET_R=1,
    GENPACKET_W=2,
};

struct p0_fixed {
    uint8_t frame0;
    uint8_t attribute0;
    uint16_t attribute1;
    uint32_t attribute2;
    uint32_t data0[1];
    uint16_t data1[1];
    float data2[1];
    int8_t data3[3];
    int8_t data4[3];
    uint8_t frame1;
    int8_t data5[1];
    uint8_t attribute3;
    uint8_t data6[2];
    uint8_t crc0;
    uint8_t frame2;
    
};

struct p1_fixed {
    uint8_t attribute0;
    uint8_t attribute1;
    uint8_t attribute2;
    float data0[1];
    uint8_t attribute3;
    uint32_t crc0;
    
};

struct p2_fixed {
    uint8_t frame0;
    uint8_t attribute0;
    uint8_t attribute1;
    uint8_t attribute2;
    float data0[10];
    uint16_t crc0;
    
};

struct p3_calculated {
    uint8_t frame0;
    uint8_t attribute0;
    uint8_t sz2;
    uint8_t sz;
    uint8_t attribute1;
    uint8_t *data0;
    uint8_t attribute2;
    uint8_t *data1;
    uint8_t crc0;
    
};


extern void genpacket_p0_fixed_received(struct p0_fixed *packet, void *private_ctx);
extern void genpacket_p1_fixed_received(struct p1_fixed *packet, void *private_ctx);
extern void genpacket_p2_fixed_received(struct p2_fixed *packet, void *private_ctx);
extern void genpacket_p3_calculated_received(struct p3_calculated *packet, void *private_ctx);

int genpacket_p0_fixed_send(struct p0_fixed *packet);
int genpacket_p1_fixed_send(struct p1_fixed *packet);
int genpacket_p2_fixed_send(struct p2_fixed *packet);
int genpacket_p3_calculated_send(struct p3_calculated *packet);

struct genpacket_params {
    int pipe_fds[GENPACKET_NRO_PIPES];
    enum genpacket_direction pipe_dir[GENPACKET_NRO_PIPES];
    void *private_ctx;
};

int genpacket_init(struct genpacket_params *params);
int genpacket_process(int timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* GENPACKET_H */

