

#ifndef _DEC_GAIN_H_
#define _DEC_GAIN_H_
#define dec_gain_h "$Id $"

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
     *   FUNCTION:  Dec_gain()
     *   PURPOSE: Decode the pitch and codebook gains
     */
    void Dec_gain(
        gc_predState *pred_state, /* i/o: MA predictor state           */
        enum Mode mode,           /* i  : AMR mode                     */
        Word16 index,             /* i  : index of quantization.       */
        Word16 code[],            /* i  : Innovative vector.           */
        Word16 evenSubfr,         /* i  : Flag for even subframes      */
        Word16 * gain_pit,        /* o  : Pitch gain.                  */
        Word16 * gain_cod,        /* o  : Code gain.                   */
        Flag   * pOverflow
    );



#ifdef __cplusplus
}
#endif

#endif  /* _DEC_GAIN_H_ */



