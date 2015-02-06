

#ifndef _PSTFILT_H_
#define _PSTFILT_H_
#define pstfilt_h "$Id $"

#include "typedef.h"
#include "mode.h"
#include "cnst.h"
#include "preemph.h"
#include "agc.h"

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
        Word16 res2[L_SUBFR];
        Word16 mem_syn_pst[M];
        preemphasisState preemph_state;
        agcState agc_state;
        Word16 synth_buf[M + L_FRAME];
    } Post_FilterState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 Post_Filter_reset(Post_FilterState *st);
    /* reset post filter (i.e. set state memory to zero)
       returns 0 on success
     */

    void Post_Filter(
        Post_FilterState *st, /* i/o : post filter states                        */
        enum Mode mode,       /* i   : AMR mode                                  */
        Word16 *syn,          /* i/o : synthesis speech (postfiltered is output) */
        Word16 *Az_4,         /* i   : interpolated LPC parameters in all subfr. */
        Flag   *pOverflow
    );
    /* filters the signal syn using the parameters in Az_4 to calculate filter
       coefficients.
       The filter must be set up using Post_Filter_init prior to the first call
       to Post_Filter. Post_FilterState is updated to mirror the current state
       of the filter

       return 0 on success
     */

#ifdef __cplusplus
}
#endif

#endif  /* _PSTFILT_H_ */



