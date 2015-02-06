

#ifndef vad_1_h
#define vad_1_h "$Id $"

#include "typedef.h"
#include "cnst_vad.h"

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
    /* state variable */
    typedef struct
    {

        Word16 bckr_est[COMPLEN];    /* background noise estimate                */
        Word16 ave_level[COMPLEN];   /* averaged input components for stationary */
        /*    estimation                            */
        Word16 old_level[COMPLEN];   /* input levels of the previous frame       */
        Word16 sub_level[COMPLEN];   /* input levels calculated at the end of
                                      a frame (lookahead)                   */
        Word16 a_data5[3][2];        /* memory for the filter bank               */
        Word16 a_data3[5];           /* memory for the filter bank               */

        Word16 burst_count;          /* counts length of a speech burst          */
        Word16 hang_count;           /* hangover counter                         */
        Word16 stat_count;           /* stationary counter                       */

        /* Note that each of the following three variables (vadreg, pitch and tone)
           holds 15 flags. Each flag reserves 1 bit of the variable. The newest
           flag is in the bit 15 (assuming that LSB is bit 1 and MSB is bit 16). */
        Word16 vadreg;               /* flags for intermediate VAD decisions     */
        Word16 pitch;                /* flags for pitch detection                */
        Word16 tone;                 /* flags for tone detection                 */
        Word16 complex_high;         /* flags for complex detection              */
        Word16 complex_low;          /* flags for complex detection              */

        Word16 oldlag_count, oldlag; /* variables for pitch detection            */

        Word16 complex_hang_count;   /* complex hangover counter, used by VAD    */
        Word16 complex_hang_timer;   /* hangover initiator, used by CAD          */

        Word16 best_corr_hp;         /* FIP filtered value Q15                   */

        Word16 speech_vad_decision;  /* final decision                           */
        Word16 complex_warning;      /* complex background warning               */

        Word16 sp_burst_count;       /* counts length of a speech burst incl     */
        Word16 corr_hp_fast;         /* filtered value                           */
    } vadState1;
    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    Word16 vad1_init(vadState1 **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to vad in each call.
       returns 0 on success
     */

    Word16 vad1_reset(vadState1 *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */

    void vad1_exit(vadState1 **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    void vad_complex_detection_update(vadState1 *st,       /* i/o : State struct     */
                                      Word16 best_corr_hp /* i   : best Corr Q15    */
                                     );

    void vad_tone_detection(vadState1 *st,  /* i/o : State struct            */
                            Word32 t0,     /* i   : autocorrelation maxima  */
                            Word32 t1,     /* i   : energy                  */
                            Flag   *pOverflow
                           );

    void vad_tone_detection_update(
        vadState1 *st,             /* i/o : State struct              */
        Word16 one_lag_per_frame,  /* i   : 1 if one open-loop lag is
                                              calculated per each frame,
                                              otherwise 0                     */
        Flag *pOverflow
    );

    void vad_pitch_detection(vadState1 *st,   /* i/o : State struct                  */
                             Word16 lags[],  /* i   : speech encoder open loop lags */
                             Flag   *pOverflow
                            );

    Word16 vad1(vadState1 *st,   /* i/o : State struct                      */
                Word16 in_buf[], /* i   : samples of the input frame
                                inbuf[159] is the very last sample,
                                incl lookahead                          */
                Flag *pOverflow
               );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _VAD1_H_ */


