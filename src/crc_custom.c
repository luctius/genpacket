/**
 * \file pycrc_stdout
 * Functions and types for CRC checks.
 *
 * Generated on Tue Jan 27 16:29:46 2015,
 * by pycrc v0.8.2, https://www.tty1.net/pycrc/
 * using the configuration:
 *    Width        = Undefined
 *    Poly         = Undefined
 *    XorIn        = Undefined
 *    ReflectIn    = Undefined
 *    XorOut       = Undefined
 *    ReflectOut   = Undefined
 *    Algorithm    = table-driven
 *****************************************************************************/
#include "crc_custom.h"     /* include the header file generated with pycrc */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Static table used for the table_driven implementation.
 * Must be initialised with the crc_init function.
 *****************************************************************************/
//static crc_t crc_table[256];

/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc_t crc_reflect(crc_t data, size_t data_len)
{
    unsigned int i;
    crc_t ret;

    ret = data & 0x01;
    for (i = 1; i < data_len; i++) {
        data >>= 1;
        ret = (ret << 1) | (data & 0x01);
    }
    return ret;
}


/**
 * Calculate the initial crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \return     The initial crc value.
 *****************************************************************************/
crc_t crc_init(const crc_cfg_t *cfg)
{
    if (cfg->reflect_in) {
        return crc_reflect(cfg->xor_in & cfg->crc_mask, cfg->width) << cfg->crc_shift;
    } else {
        return cfg->xor_in & cfg->crc_mask << cfg->crc_shift;
    }
}


/**
 * Populate the private static crc table.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \return     void
 *****************************************************************************/
void crc_table_gen(const crc_cfg_t *cfg)
{
    crc_t crc;
    crc_t *crc_table_ptr = &cfg->crc_table[0];
    unsigned int i, j;

    for (i = 0; i < 256; i++) {
        if (cfg->reflect_in) {
            crc = crc_reflect(i, 8);
        } else {
            crc = i;
        }
        crc <<= (cfg->width - 8 + cfg->crc_shift);
        for (j = 0; j < 8; j++) {
            if (crc & (cfg->msb_mask << cfg->crc_shift)) {
                crc = (crc << 1) ^ (cfg->poly << cfg->crc_shift);
            } else {
                crc = crc << 1;
            }
        }
        if (cfg->reflect_in) {
            crc = crc_reflect(crc >> cfg->crc_shift, cfg->width) << cfg->crc_shift;
        }
        crc_table_ptr[i] = crc & (cfg->crc_mask << cfg->crc_shift);
    }
}


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param cfg      A pointer to a initialised crc_cfg_t structure.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(const crc_cfg_t *cfg, crc_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int tbl_idx;

    if (cfg->reflect_in) {
        while (data_len--) {
            tbl_idx = ((crc >> cfg->crc_shift) ^ *d) & 0xff;
            crc = (cfg->crc_table[tbl_idx] ^ (crc >> 8)) & (cfg->crc_mask << cfg->crc_shift);

            d++;
        }
    } else {
        while (data_len--) {
            tbl_idx = ((crc >> (cfg->width - 8 + cfg->crc_shift)) ^ *d) & 0xff;
            crc = (cfg->crc_table[tbl_idx] ^ (crc << 8)) & (cfg->crc_mask << cfg->crc_shift);

            d++;
        }
    }
    return crc & (cfg->crc_mask << cfg->crc_shift);
}


/**
 * Calculate the final crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
crc_t crc_finalize(const crc_cfg_t *cfg, crc_t crc)
{
    crc >>= cfg->crc_shift;
    if (cfg->reflect_in == !cfg->reflect_out) {
        crc = crc_reflect(crc, cfg->width);
    }
    return (crc ^ cfg->xor_out) & cfg->crc_mask;
}



