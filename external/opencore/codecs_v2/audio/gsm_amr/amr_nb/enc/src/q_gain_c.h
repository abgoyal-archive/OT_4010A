

#ifndef q_gain_c_h
#define q_gain_c_h "$Id $"

#include "typedef.h"
#include "mode.h"
#include "gc_pred.h"

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

    /*--------------------------------------------------------------------------*
     * Function q_gain_code()                                                   *
     * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                  *
     * Scalar quantization of the innovative codebook gain.                     *
     *                                                                          *
     * gc_pred() is used for MA prediction of the innovation energy             *
     *--------------------------------------------------------------------------*/
    Word16 q_gain_code(         /* o  : quantization index,            Q0  */
        enum Mode mode,         /* i  : AMR mode                           */
        Word16 exp_gcode0,      /* i  : predicted CB gain (exponent),  Q0  */
        Word16 frac_gcode0,     /* i  : predicted CB gain (fraction),  Q15 */
        Word16 *gain,           /* i/o: quantized fixed codebook gain, Q1  */
        Word16 *qua_ener_MR122, /* o  : quantized energy error,        Q10 */
        /*      (for MR122 MA predictor update)    */
        Word16 *qua_ener,       /* o  : quantized energy error,        Q10 */
        /*      (for other MA predictor update)    */
        Flag   *pOverflow
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* q_gain_c_h */


