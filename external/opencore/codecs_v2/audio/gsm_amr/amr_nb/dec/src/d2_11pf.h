
#ifndef d2_11pf_h
#define d2_11pf_h "$Id $"

#include "typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ********************************************************************************
    *                         LOCAL VARIABLES AND TABLES
    ********************************************************************************
    */

    /*
    ********************************************************************************
    *                         DEFINITION OF DATA TYPES
    ********************************************************************************
    */

    /*
    ********************************************************************************
    *                         DECLARATION OF PROTOTYPES
    ********************************************************************************
    */
    /*************************************************************************
     *
     *  FUNCTION:  decode_2i40_11bits (decod_ACELP())
     *
     *  PURPOSE:   Algebraic codebook decoder for 2 pulses coded with 11 bits
     *
     *************************************************************************/

    void decode_2i40_11bits(
        Word16 sign,   /* i : signs of 2 pulses.                       */
        Word16 index,  /* i : Positions of the 2 pulses.               */
        Word16 cod[]   /* o : algebraic (fixed) codebook excitation    */
    );

#ifdef __cplusplus
}
#endif

#endif
