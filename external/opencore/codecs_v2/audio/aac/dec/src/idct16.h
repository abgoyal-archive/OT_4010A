

#ifndef IDCT16_H
#define IDCT16_H



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


    void idct_16(Int32 vec[], Int32 scratch_mem[]);


#ifdef __cplusplus
}
#endif

#endif  /* IDCT16_H */
