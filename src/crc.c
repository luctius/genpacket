#include "crc.h"
#include "debug_print.h"

/* using the configuration:
*    Width        = 8
*    Poly         = 0x07
*    XorIn        = 0x00
*    ReflectIn    = False
*    XorOut       = 0x00
*    ReflectOut   = False
*/


uint8_t crc8(uint8_t *data, size_t len) {
    crc_cfg_t crc_config = {
        .width          = 8,
        .poly           = 0x07,
        .xor_in         = 0x00,
        .reflect_in     = false,
        .xor_out        = 0x00,
        .reflect_out    = false,
    };
    
    return (uint8_t) custom_crc(&crc_config, data, len);
}

/* using the configuration:
*    Width        = 32
*    Poly         = 0x04c11db7
*    XorIn        = 0xffffffff
*    ReflectIn    = True
*    XorOut       = 0xffffffff
*    ReflectOut   = True
*/

uint32_t crc32(const void *data, size_t len) {
    crc_cfg_t crc_config = {
        .width          = 32,
        .poly           = 0x04c11db7,
        .xor_in         = 0xffffffff,
        .reflect_in     = true,
        .xor_out        = 0xffffffff,
        .reflect_out    = true,
    };
    
    return (uint32_t) custom_crc(&crc_config, data, len);
}



uint64_t custom_crc(crc_cfg_t *cfg,uint8_t *data, size_t len) {
    cfg->msb_mask = (crc_t)1u << (cfg->width - 1);
    cfg->crc_mask = (cfg->msb_mask - 1) | cfg->msb_mask;
    cfg->crc_shift = cfg->width < 8 ? 8 - cfg->width : 0;

    cfg->poly &= cfg->crc_mask;
    cfg->xor_in &= cfg->crc_mask;
    cfg->xor_out &= cfg->crc_mask;
    
    //crc8_populate_msb(crc8_table,CRC8_POLY);
    //return crc8_calc(crc8_table, data, len, CRC8_INIT_VALUE);
    
    crc_table_gen(cfg);


    /**
     * Calculate the initial crc value.
     *
     * \param cfg  A pointer to a initialised crc_cfg_t structure.
     * \return     The initial crc value.
     *****************************************************************************/
    crc_t crc =  crc_init(cfg);


    /**
     * Update the crc value with new data.
     *
     * \param crc      The current crc value.
     * \param cfg      A pointer to a initialised crc_cfg_t structure.
     * \param data     Pointer to a buffer of \a data_len bytes.
     * \param data_len Number of bytes in the \a data buffer.
     * \return         The updated crc value.
     *****************************************************************************/
    crc = crc_update(cfg, crc, data, len);


    /**
     * Calculate the final crc value.
     *
     * \param cfg  A pointer to a initialised crc_cfg_t structure.
     * \param crc  The current crc value.
     * \return     The final crc value.
     *****************************************************************************/
    return (uint64_t) crc_finalize(cfg, crc);
}
