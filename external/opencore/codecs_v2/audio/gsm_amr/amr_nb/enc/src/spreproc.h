

#ifndef spreproc_h
#define spreproc_h "$Id $"

#include "cnst.h"
#include "mode.h"
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
    void subframePreProc(
        enum Mode mode,            /* i  : coder mode                            */
        const Word16 gamma1[],     /* i  : spectral exp. factor 1                */
        const Word16 gamma1_12k2[],/* i  : spectral exp. factor 1 for EFR        */
        const Word16 gamma2[],     /* i  : spectral exp. factor 2                */
        Word16 *A,                 /* i  : A(z) unquantized for the 4 subframes  */
        Word16 *Aq,                /* i  : A(z)   quantized for the 4 subframes  */
        Word16 *speech,            /* i  : speech segment                        */
        Word16 *mem_err,           /* i  : pointer to error signal               */
        Word16 *mem_w0,            /* i  : memory of weighting filter            */
        Word16 *zero,              /* i  : pointer to zero vector                */
        Word16 ai_zero[],          /* o  : history of weighted synth. filter     */
        Word16 exc[],              /* o  : long term prediction residual         */
        Word16 h1[],               /* o  : impulse response                      */
        Word16 xn[],               /* o  : target vector for pitch search        */
        Word16 res2[],             /* o  : long term prediction residual         */
        Word16 error[]             /* o  : error of LPC synthesis filter         */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* spreproc_h */

