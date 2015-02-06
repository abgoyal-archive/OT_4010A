

#ifndef C1035PF_H
#define C1035PF_H "$Id $"

#include    "typedef.h"

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/
    extern Word16 gray[];
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

    void code_10i40_35bits(
        Word16 x[],     /* (i)   : target vector                                */
        Word16 cn[],    /* (i)   : residual after long term prediction          */
        Word16 h[],     /* (i)   : impulse response of weighted synthesis filter
                             h[-L_subfr..-1] must be set to zero            */
        Word16 cod[],   /* (o)   : algebraic (fixed) codebook excitation        */
        Word16 y[],     /* (o)   : filtered fixed codebook excitation           */
        Word16 indx[],  /* (o)   : index of 10 pulses (sign + position)         */
        Flag *pOverflow /* (i/o) : overflow Flag                                */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _C1035PF_H_ */
