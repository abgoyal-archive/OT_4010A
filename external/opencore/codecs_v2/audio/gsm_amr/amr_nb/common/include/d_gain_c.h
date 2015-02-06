

#ifndef _D_GAIN_C_H_
#define _D_GAIN_C_H_
#define d_gain_c_h "$Id $"

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
    /*
     *  Function    : d_gain_code
     *  Purpose     : Decode the fixed codebook gain using the received index.
     *  Description : The received index gives the gain correction factor
     *                gamma. The quantized gain is given by   g_q = g0 * gamma
     *                where g0 is the predicted gain. To find g0, 4th order
     *                MA prediction is applied to the mean-removed innovation
     *                energy in dB.
     *  Returns     : void
     */
    void d_gain_code(
        gc_predState *pred_state, /* i/o : MA predictor state               */
        enum Mode mode,           /* i   : AMR mode                         */
        Word16 index,             /* i   : received quantization index      */
        Word16 code[],            /* i   : innovation codevector            */
        Word16 *gain_code,        /* o   : decoded innovation gain          */
        Flag   *pOverflow
    );

#ifdef __cplusplus
}
#endif

#endif  /* _D_GAIN_C_H_ */


