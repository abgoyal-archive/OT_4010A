

#ifndef _ENC_LAG3_H_
#define _ENC_LAG3_H_
#define enc_lag3_h "$Id $"

#include "typedef.h"

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; [Define module specific macros here]
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; [Include all pre-processor statements here.]
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; [Declare variables used in this module but defined elsewhere]
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
    typedef struct
    {
        Word16 past_gain;
    } agcState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16
    Enc_lag3(                /* o  : Return index of encoding     */
        Word16 T0,           /* i  : Pitch delay                          */
        Word16 T0_frac,      /* i  : Fractional pitch delay               */
        Word16 T0_prev,      /* i  : Integer pitch delay of last subframe */
        Word16 T0_min,       /* i  : minimum of search range              */
        Word16 T0_max,       /* i  : maximum of search range              */
        Word16 delta_flag,   /* i  : Flag for 1st (or 3rd) subframe       */
        Word16 flag4,        /* i  : Flag for encoding with 4 bits        */
        Flag   *pOverflow
    );




#ifdef __cplusplus
}
#endif

#endif  /* _ENC_LAG3_H_ */


