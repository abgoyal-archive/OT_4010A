
#ifndef CALC_SBR_SYNFILTERBANK_H
#define CALC_SBR_SYNFILTERBANK_H


#include "pv_audio_type_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/
#define N  14

#define ROUND_SYNFIL  (32768 + 4096)
#define ROUND_SYNFIL1  (32768 + 4096)
#define ROUND_SYNFIL2  (32768 - 4096)

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


    void calc_sbr_synfilterbank_LC(Int32 * Sr,
    Int16 * timeSig,
    Int16   V[1280],
    bool bDownSampleSBR);

#ifdef HQ_SBR


    void calc_sbr_synfilterbank(Int32 * Sr,
                                Int32 * Si,
                                Int16 * timeSig,
                                Int16   V[1280],
                                bool bDownSampleSBR);

#endif

#ifdef __cplusplus
}
#endif

#endif

