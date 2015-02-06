

#ifndef P_OL_WGH_H
#define P_OL_WGH_H "$Id $"

#include "typedef.h"
#include "mode.h"
#include "vad.h"

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
    extern const Word16 corrweight[];

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/
    /* state variable */

    typedef struct
    {
        Word16 old_T0_med;
        Word16 ada_w;
        Word16 wght_flg;
    } pitchOLWghtState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 p_ol_wgh_init(pitchOLWghtState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to p_ol_wgh in each call.
       returns 0 on success
     */

    Word16 p_ol_wgh_reset(pitchOLWghtState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */

    void p_ol_wgh_exit(pitchOLWghtState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    Word16 Pitch_ol_wgh(      /* o   : open loop pitch lag                            */
        pitchOLWghtState *st, /* i/o : State struct                                   */
        vadState *vadSt,      /* i/o : VAD state struct                               */
        Word16 signal[],      /* i   : signal used to compute the open loop pitch     */
        /*       signal[-pit_max] to signal[-1] should be known */
        Word16 pit_min,       /* i   : minimum pitch lag                              */
        Word16 pit_max,       /* i   : maximum pitch lag                              */
        Word16 L_frame,       /* i   : length of frame to compute pitch               */
        Word16 old_lags[],    /* i   : history with old stored Cl lags                */
        Word16 ol_gain_flg[], /* i   : OL gain flag                                   */
        Word16 idx,           /* i   : index                                          */
        Flag dtx,             /* i   : dtx flag; use dtx=1, do not use dtx=0          */
        Flag   *pOverflow     /* o   : overflow flag                                  */
    );

#ifdef __cplusplus
}
#endif

#endif  /* _P_OL_WGH_H_ */
