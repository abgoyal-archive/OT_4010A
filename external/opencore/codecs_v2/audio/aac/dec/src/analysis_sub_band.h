

#ifndef ANALYSIS_SUB_BAND_H
#define ANALYSIS_SUB_BAND_H



#include "pv_audio_type_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES AND SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/


    void analysis_sub_band_LC(Int32 vec[64],
    Int32 cosine_total[],
    Int32 maxBand,
    Int32 scratch_mem[][64]);

#ifdef HQ_SBR


    void analysis_sub_band(Int32 vec[64],
                           Int32 cosine_total[],
                           Int32 sine_total[],
                           Int32 maxBand,
                           Int32 scratch_mem[][64]);

#endif


#ifdef __cplusplus
}
#endif

#endif  /* ANALYSIS_SUB_BAND_H */
