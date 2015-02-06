

#ifndef spstproc_h
#define spstproc_h "$Id $"

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

    void subframePostProc(
        Word16 *speech,   /* i   : speech segment                        */
        enum Mode mode,   /* i   : coder mode                            */
        Word16 i_subfr,   /* i   : Subframe nr                           */
        Word16 gain_pit,  /* i   : Pitch gain                       Q14  */
        Word16 gain_code, /* i   : Decoded innovation gain               */
        Word16 *Aq,       /* i   : A(z) quantized for the 4 subframes    */
        Word16 synth[],   /* i   : Local snthesis                        */
        Word16 xn[],      /* i   : Target vector for pitch search        */
        Word16 code[],    /* i   : Fixed codebook exitation              */
        Word16 y1[],      /* i   : Filtered adaptive exitation           */
        Word16 y2[],      /* i   : Filtered fixed codebook excitation    */
        Word16 *mem_syn,  /* i/o : memory of synthesis filter            */
        Word16 *mem_err,  /* o   : pointer to error signal               */
        Word16 *mem_w0,   /* o   : memory of weighting filter            */
        Word16 *exc,      /* o   : long term prediction residual         */
        Word16 *sharp,    /* o   : pitch sharpening value                */
        Flag   *pOverflow
    );
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* spstproc_h */

