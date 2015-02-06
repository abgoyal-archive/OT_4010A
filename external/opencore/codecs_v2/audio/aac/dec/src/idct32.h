

#ifndef IDCT32_H
#define IDCT32_H



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

    void idct_32(Int32 vec[], Int32 scratch_mem[]);



#ifdef __cplusplus
}
#endif

#endif  /* IDCT32_H */
