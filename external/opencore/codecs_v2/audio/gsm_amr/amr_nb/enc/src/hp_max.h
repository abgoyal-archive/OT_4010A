

#ifndef HP_MAX_H
#define HP_MAX_H  "$Id $"

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

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 hp_max(
        Word32 corr[],      /* i   : correlation vector.                      */
        Word16 scal_sig[],  /* i   : scaled signal.                           */
        Word16 L_frame,     /* i   : length of frame to compute pitch         */
        Word16 lag_max,     /* i   : maximum lag                              */
        Word16 lag_min,     /* i   : minimum lag                              */
        Word16 *cor_hp_max, /* o   : max high-pass filtered norm. correlation */
        Flag   *pOverflow   /* i/o : overflow Flag                            */
    );

#ifdef __cplusplus
}
#endif

#endif  /* _HP_MAX_H_ */


