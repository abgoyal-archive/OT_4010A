

#ifndef _PITCH_FR_H_
#define _PITCH_FR_H_
#define pitch_fr_h "$Id $"

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
        Word16 T0_prev_subframe;   /* integer pitch lag of previous sub-frame */
    } Pitch_frState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/

    Word16 Pitch_fr_init(Pitch_frState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to Pitch_fr in each call.
       returns 0 on success
     */

    Word16 Pitch_fr_reset(Pitch_frState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */

    void Pitch_fr_exit(Pitch_frState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    Word16 Pitch_fr(         /* o   : pitch period (integer)                    */
        Pitch_frState *st,   /* i/o : State struct                              */
        enum Mode mode,      /* i   : codec mode                                */
        Word16 T_op[],       /* i   : open loop pitch lags                      */
        Word16 exc[],        /* i   : excitation buffer                         */
        Word16 xn[],         /* i   : target vector                             */
        Word16 h[],          /* i   : impulse response of synthesis and
                                  weighting filters                         */
        Word16 L_subfr,      /* i   : Length of subframe                        */
        Word16 i_subfr,      /* i   : subframe offset                           */
        Word16 *pit_frac,    /* o   : pitch period (fractional)                 */
        Word16 *resu3,       /* o   : subsample resolution 1/3 (=1) or 1/6 (=0) */
        Word16 *ana_index,   /* o   : index of encoding                         */
        Flag   *pOverflow
    );

#ifdef __cplusplus
}
#endif

#endif  /* _PITCH_FR_H_ */


