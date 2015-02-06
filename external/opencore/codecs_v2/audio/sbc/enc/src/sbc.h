
#ifndef __SBC__
#define __SBC__

#ifdef __cplusplus
extern "C"
{
#endif


#include "oscl_types.h"
#include "sbc_type_defs.h"

#define fabs(x) ((x) < 0 ? (-x) : (x))

    /*$F
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *    A2DP specification Adopted version 1.0: Appendix B, Section 12.5.1. Page 54
     *    and Page 55
     *
     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     */
#define SYNCWORD    0x9C    /* syncword -- The 8 bit string 10011100b or 0x9C */

    /* Two bits to indicate the sampling frequency of 16, 32, 44.1, 48 kHz */
    typedef enum    sf_t { SF_16, SF_32, SF_44, SF_48 } sf_t;

    /* Two bits to indicate the block length of 4, 8, 12, 16 */
    typedef enum bl_t { BL_4, BL_8, BL_12, BL_16 } bl_t;

    /* One bit to indicate the bit allocation method */
    typedef enum am_t { AM_LOUDNESS, AM_SNR } am_t;

    /* One bit to indicate the number of subbands */
    typedef enum sb_t { SB_4, SB_8 } sb_t;

    /*
     -------------------------------------------------------------------------------
     *    This structure contains an unpacked SBC frame. Yes, there is probably
     *    quite some unused space herein
     -------------------------------------------------------------------------------
     */
    typedef struct sbc_t
    {
        UInt        sf_index;
        UWord8      blocks;
        UInt        channel_mode;
        UWord8      channels;
        UInt        allocation_method;
        UWord8      subbands;
        Int         bitpool;
        UWord8      join;                   /* bit number x set means joint stereo has been used in subband x */
        Int         bits[2][8];             /* bits distribution */
        Int         bitpointer;
        UWord32     scale_factor[2][8];
        Int         sb_sample[16][2][8];    /* modified subband samples */
    } sbc_t;

    typedef struct  crc_t
    {
        UWord8  crc_buffer[11];
        Int     crc_consumed;
    } crc_t;


    typedef struct  analysis_filter_t
    {
        Int  X[2][200] ;
    } analysis_filter_t;


    typedef struct  enc_state_t
    {
        Boolean             init;
        crc_t               crc;
        sbc_t               sbc;
        analysis_filter_t   filter;
    } enc_state_t;

#ifdef __cplusplus
}
#endif


#endif /* __SBC__ */
