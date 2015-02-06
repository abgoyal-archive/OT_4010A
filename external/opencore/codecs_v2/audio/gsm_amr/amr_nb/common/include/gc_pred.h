

#ifndef _GC_PRED_H_
#define _GC_PRED_H_
#define gc_pred_h "$Id $"

#include "typedef.h"
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
    typedef struct
    {
        Word16 past_qua_en[4];         /* normal MA predictor memory,         Q10 */
        /* (contains 20*log10(qua_err))            */
        Word16 past_qua_en_MR122[4];   /* MA predictor memory for MR122 mode, Q10 */
        /* (contains log2(qua_err))                */
    } gc_predState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/

    Word16 gc_pred_reset(gc_predState *st);
    /* reset of codebook gain MA predictor state (i.e. set state memory to zero)
       returns 0 on success
     */
    void gc_pred_exit(gc_predState **st);
    /* de-initialize codebook gain MA predictor state (i.e. free state struct)
       stores NULL in *st
     */

    void
    gc_pred_copy(
        gc_predState *st_src,  /* i : State struct                           */
        gc_predState *st_dest  /* o : State struct                           */
    );

    /*
     * FUNCTION:  gc_pred()
     * PURPOSE: MA prediction of the innovation energy
     *          (in dB/(20*log10(2))) with mean  removed).
     */
    void gc_pred(
        gc_predState *st,   /* i/o: State struct                           */
        enum Mode mode,     /* i  : AMR mode                               */
        Word16 *code,       /* i  : innovative codebook vector (L_SUBFR)   */
        /*      MR122: Q12, other modes: Q13           */
        Word16 *exp_gcode0, /* o  : exponent of predicted gain factor, Q0  */
        Word16 *frac_gcode0,/* o  : fraction of predicted gain factor  Q15 */
        Word16 *exp_en,     /* o  : exponent of innovation energy,     Q0  */
        /*      (only calculated for MR795)            */
        Word16 *frac_en,    /* o  : fraction of innovation energy,     Q15 */
        /*      (only calculated for MR795)            */
        Flag   *pOverflow
    );

    /*
     * FUNCTION:  gc_pred_update()
     * PURPOSE: update MA predictor with last quantized energy
     */
    void gc_pred_update(
        gc_predState *st,      /* i/o: State struct                     */
        Word16 qua_ener_MR122, /* i  : quantized energy for update, Q10 */
        /*      (log2(qua_err))                  */
        Word16 qua_ener        /* i  : quantized energy for update, Q10 */
        /*      (20*log10(qua_err))              */
    );

    /*
     * FUNCTION:  gc_pred_average_limited()
     * PURPOSE: get average of MA predictor state values (with a lower limit)
     *          [used in error concealment]
     */
    void gc_pred_average_limited(
        gc_predState *st,       /* i: State struct                    */
        Word16 *ener_avg_MR122, /* o: averaged quantized energy,  Q10 */
        /*    (log2(qua_err))                 */
        Word16 *ener_avg,       /* o: averaged quantized energy,  Q10 */
        /*    (20*log10(qua_err))             */
        Flag   *pOverflow
    );


#ifdef __cplusplus
}
#endif

#endif  /* _GC_PRED_H_ */



