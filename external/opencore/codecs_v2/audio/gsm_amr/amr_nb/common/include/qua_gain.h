

#ifndef qua_gain_h
#define qua_gain_h "$Id $"

#include "typedef.h"
#include "gc_pred.h"
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
#define VQ_SIZE_HIGHRATES 128
#define VQ_SIZE_LOWRATES 64

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

    Word16
    Qua_gain(                   /* o  : index of quantization.                 */
        enum Mode mode,         /* i  : AMR mode                               */
        Word16 exp_gcode0,      /* i  : predicted CB gain (exponent),      Q0  */
        Word16 frac_gcode0,     /* i  : predicted CB gain (fraction),      Q15 */
        Word16 frac_coeff[],    /* i  : energy coeff. (5), fraction part,  Q15 */
        Word16 exp_coeff[],     /* i  : energy coeff. (5), exponent part,  Q0  */
        /*      (frac_coeff and exp_coeff computed in  */
        /*       calc_filt_energies())                 */
        Word16 gp_limit,        /* i  : pitch gain limit                       */
        Word16 *gain_pit,       /* o  : Pitch gain,                        Q14 */
        Word16 *gain_cod,       /* o  : Code gain,                         Q1  */
        Word16 *qua_ener_MR122, /* o  : quantized energy error,            Q10 */
        /*      (for MR122 MA predictor update)        */
        Word16 *qua_ener,       /* o  : quantized energy error,            Q10 */
        /*      (for other MA predictor update)        */
        Flag   *pOverflow       /* o  : overflow indicator                     */
    );
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* qua_gain_h */



