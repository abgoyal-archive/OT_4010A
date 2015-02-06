

#ifndef COR_H_X2_H
#define COR_H_X2_H

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
    void cor_h_x2(
        Word16 h[],    /* (i): impulse response of weighted synthesis filter */
        Word16 x[],    /* (i): target                                        */
        Word16 dn[],   /* (o): correlation between target and h[]            */
        Word16 sf,     /* (i): scaling factor: 2 for 12.2, 1 for others      */
        Word16 nb_track,/* (i): the number of ACB tracks                     */
        Word16 step,   /* (i): step size from one pulse position to the next
                           in one track                                  */
        Flag *pOverflow
    );

#ifdef __cplusplus
}
#endif

#endif  /* _COR_H_X2_H_ */

