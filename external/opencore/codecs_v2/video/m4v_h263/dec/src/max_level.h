


#ifndef max_level_H
#define max_level_H

#include "mp4def.h"




#ifdef __cplusplus
extern "C"
{
#endif

    extern const int intra_max_level[2][NCOEFF_BLOCK];

    extern const int inter_max_level[2][NCOEFF_BLOCK];

    extern const int intra_max_run0[28];


    extern const int intra_max_run1[9];

    extern const int inter_max_run0[13];


    extern const int inter_max_run1[4];


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


    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif













