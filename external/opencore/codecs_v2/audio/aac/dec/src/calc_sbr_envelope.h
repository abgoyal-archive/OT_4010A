

#ifndef CALCULATE_SBR_ENVELOPE_H
#define CALCULATE_SBR_ENVELOPE_H


#include    "s_sbr_frame_data.h"
#include    "sbr_generate_high_freq.h"
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/
    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    void calc_sbr_envelope(SBR_FRAME_DATA *frameData,
    Int32 *aBufR,
    Int32 *aBufI,
    Int    freqBandTable1[2][MAX_FREQ_COEFFS + 1],
    const Int32 *nSfb,
    Int32   freqBandTable2[MAX_NOISE_COEFFS + 1],
    Int32   nNBands,
    Int32   reset,
    Int32 *degreeAlias,
    Int32 *harm_index,
    Int32 *phase_index,
    Int32 hFp[64],
    Int32 *sUp,
    Int32 limSbc[][13],
    Int32 *gateMode,
#ifdef HQ_SBR
    Int32 *fBuf_man[64],
    Int32 *fBuf_exp[64],
    Int32 *fBufN_man[64],
    Int32 *fBufN_exp[64],
#endif
    Int32 scratch_mem[][64],
    struct PATCH Patch,
    Int32  sqrt_cache[][4],
    Int32  LC_flag);
#ifdef __cplusplus
}
#endif

#endif   /*  CALCULATE_SBR_ENVELOPE_H */


