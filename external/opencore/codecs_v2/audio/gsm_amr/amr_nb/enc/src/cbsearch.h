

#ifndef cbsearch_h
#define cbsearch_h "$Id $"


#include "typedef.h"
#include "mode.h"

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

    void cbsearch(Word16 x[],     /* i : target vector, Q0                      */
    Word16 h[],     /* i : impulse response of weighted synthesis */
    /*     filter h[-L_subfr..-1] must be set to  */
    /*    zero. Q12                               */
    Word16 T0,      /* i : Pitch lag                              */
    Word16 pitch_sharp, /* i : Last quantized pitch gain, Q14     */
    Word16 gain_pit,/* i : Pitch gain, Q14                        */
    Word16 res2[],  /* i : Long term prediction residual, Q0      */
    Word16 code[],  /* o : Innovative codebook, Q13               */
    Word16 y[],     /* o : filtered fixed codebook excitation, Q12 */
    Word16 **anap,  /* o : Signs of the pulses                    */
    enum Mode mode, /* i : coder mode                             */
    Word16 subNr,   /* i : subframe number                        */
    Flag  *pOverflow  /* o : Flag set when overflow occurs        */
                 );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _CBSEARCH_H_ */



