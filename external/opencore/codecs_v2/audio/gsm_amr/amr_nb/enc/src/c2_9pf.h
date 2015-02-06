

#ifndef c2_9pf_h
#define c2_9pf_h "$Id $"

#include "typedef.h"

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
    Word16 code_2i40_9bits(
        Word16 subNr,       /* i : subframe number                               */
        Word16 x[],         /* i : target vector                                 */
        Word16 h[],         /* i : impulse response of weighted synthesis filter */
        /*     h[-L_subfr..-1] must be set to zero.          */
        Word16 T0,          /* i : Pitch lag                                     */
        Word16 pitch_sharp, /* i : Last quantized pitch gain                     */
        Word16 code[],      /* o : Innovative codebook                           */
        Word16 y[],         /* o : filtered fixed codebook excitation            */
        Word16 * sign,      /* o : Signs of 2 pulses                             */
        Flag   * pOverflow  /* o : Flag set when overflow occurs                 */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _c2_9PF_H_ */



