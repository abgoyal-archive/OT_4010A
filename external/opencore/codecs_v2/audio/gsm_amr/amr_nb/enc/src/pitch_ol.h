

#ifndef PITCH_OL_H
#define PITCH_OL_H "$Id $"

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
    Word16 Pitch_ol(       /* o   : open loop pitch lag                         */
        vadState *vadSt,   /* i/o : VAD state struct                            */
        enum Mode mode,    /* i   : coder mode                                  */
        Word16 signal[],   /* i   : signal used to compute the open loop pitch  */
        /*    signal[-pit_max] to signal[-1] should be known */
        Word16 pit_min,    /* i   : minimum pitch lag                           */
        Word16 pit_max,    /* i   : maximum pitch lag                           */
        Word16 L_frame,    /* i   : length of frame to compute pitch            */
        Word16 idx,        /* i   : frame index                                 */
        Flag dtx,          /* i   : dtx flag; use dtx=1, do not use dtx=0       */
        Flag *pOverflow    /* i/o : overflow Flag                               */
    );


#ifdef __cplusplus
}
#endif

#endif  /* PITCH_OL_H_ */


