

#ifndef _EC_GAINS_H_
#define _EC_GAINS_H_
#define ec_gains_h "$Id $"

#include "typedef.h"
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
    typedef struct
    {
        Word16 pbuf[5];
        Word16 past_gain_pit;
        Word16 prev_gp;
    } ec_gain_pitchState;

    typedef struct
    {
        Word16 gbuf[5];
        Word16 past_gain_code;
        Word16 prev_gc;
    } ec_gain_codeState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/

    /*
     *  Function    : ec_gain_code_reset
     *  Purpose     : Resets state memory
     *
     */
    Word16 ec_gain_code_reset(
        ec_gain_codeState *state
    );


    /*
     *  Function    : ec_gain_code
     *  Purpose     : conceal the codebook gain
     *                Call this function only in BFI (instead of normal gain
     *                decoding function)
     */
    void ec_gain_code(
        ec_gain_codeState *st,    /* i/o : State struct                     */
        gc_predState *pred_state, /* i/o : MA predictor state               */
        Word16 state,             /* i   : state of the state machine       */
        Word16 *gain_code,        /* o   : decoded innovation gain          */
        Flag   *pOverflow
    );

    /*
     *  Function    : ec_gain_code_update
     *  Purpose     : update the codebook gain concealment state;
     *                limit gain_code if the previous frame was bad
     *                Call this function always after decoding (or concealing)
     *                the gain
     */
    void ec_gain_code_update(
        ec_gain_codeState *st,    /* i/o : State struct                     */
        Word16 bfi,               /* i   : flag: frame is bad               */
        Word16 prev_bf,           /* i   : flag: previous frame was bad     */
        Word16 *gain_code,        /* i/o : decoded innovation gain          */
        Flag   *pOverflow
    );


    /*
     *  Function:   ec_gain_pitch_reset
     *  Purpose:    Resets state memory
     */
    Word16 ec_gain_pitch_reset(
        ec_gain_pitchState *state
    );

    /*
     *  Function    : ec_gain_pitch_exit
     *  Purpose     : The memory used for state memory is freed
     */
    void ec_gain_pitch_exit(
        ec_gain_pitchState **state
    );

    /*
     *  Function    : ec_gain_pitch
     *  Purpose     : conceal the pitch gain
     *                Call this function only in BFI (instead of normal gain
     *                decoding function)
     */
    void ec_gain_pitch(
        ec_gain_pitchState *st, /* i/o : state variables                   */
        Word16 state,           /* i   : state of the state machine        */
        Word16 *gain_pitch,     /* o   : pitch gain (Q14)                  */
        Flag   *pOverflow
    );

    /*
     *  Function    : ec_gain_pitch_update
     *  Purpose     : update the pitch gain concealment state;
     *                limit gain_pitch if the previous frame was bad
     *                Call this function always after decoding (or concealing)
     *                the gain
     */
    void ec_gain_pitch_update(
        ec_gain_pitchState *st, /* i/o : state variables                   */
        Word16 bfi,             /* i   : flag: frame is bad                */
        Word16 prev_bf,         /* i   : flag: previous frame was bad      */
        Word16 *gain_pitch,     /* i/o : pitch gain                        */
        Flag   *pOverflow
    );


#ifdef __cplusplus
}
#endif

#endif  /* _EC_GAINS_H_ */


