/**
 * \file pycrc_stdout
 * Functions and types for CRC checks.
 *
 * Generated on Tue Jan 27 16:29:56 2015,
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
#ifndef __PYCRC_STDOUT__
#define __PYCRC_STDOUT__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algoritm-specific code, is desired.
 *****************************************************************************/
#define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least cfg->width bits.
 *****************************************************************************/
typedef unsigned long long int crc_t;


/**
 * The configuration type of the CRC algorithm.
 *****************************************************************************/
typedef struct {
    unsigned int width;     /*!< The width of the polynomial */
    crc_t poly;             /*!< The CRC polynomial */
    bool reflect_in;         /*!< Whether the input shall be reflected or not */
    crc_t xor_in;           /*!< The initial value of the algorithm */
    bool reflect_out;        /*!< Wether the output shall be reflected or not */
    crc_t xor_out;          /*!< The value which shall be XOR-ed to the final CRC value */

    /* internal parameters */
    crc_t msb_mask;             /*!< a bitmask with the Most Significant Bit set to 1
                                     initialise as (crc_t)1u << (width - 1) */
    crc_t crc_mask;             /*!< a bitmask with all width bits set to 1
                                     initialise as (cfg->msb_mask - 1) | cfg->msb_mask */
    unsigned int crc_shift;     /*!< a shift count that is used when width < 8
                                     initialise as cfg->width < 8 ? 8 - cfg->width : 0 */
} crc_cfg_t;


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param data         The data word to be reflected.
 * \param data_len     The width of \a data expressed in number of bits.
 * \return             The reflected data.
 *****************************************************************************/
crc_t crc_reflect(crc_t data, size_t data_len);


/**
 * Populate the private static crc table.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \return     void
 *****************************************************************************/
void crc_table_gen(const crc_cfg_t *cfg);


/**
 * Calculate the initial crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \return     The initial crc value.
 *****************************************************************************/
crc_t crc_init(const crc_cfg_t *cfg);


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param cfg      A pointer to a initialised crc_cfg_t structure.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(const crc_cfg_t *cfg, crc_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param cfg  A pointer to a initialised crc_cfg_t structure.
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
crc_t crc_finalize(const crc_cfg_t *cfg, crc_t crc);


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __PYCRC_STDOUT__ */

