

#ifndef OL_LTP_H
#define OL_LTP_H "$Id $"

#include "typedef.h"
#include "mode.h"
#include "p_ol_wgh.h"

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
    void ol_ltp(
        pitchOLWghtState *st, /* i/o : State struct                            */
        vadState *vadSt,      /* i/o : VAD state struct                        */
        enum Mode mode,       /* i   : coder mode                              */
        Word16 wsp[],         /* i   : signal used to compute the OL pitch, Q0 */
        /*       uses signal[-pit_max] to signal[-1]     */
        Word16 *T_op,         /* o   : open loop pitch lag,                 Q0 */
        Word16 old_lags[],    /* i   : history with old stored Cl lags         */
        Word16 ol_gain_flg[], /* i   : OL gain flag                            */
        Word16 idx,           /* i   : index                                   */
        Flag dtx,             /* i   : dtx flag; use dtx=1, do not use dtx=0   */
        Flag *pOverflow       /* i/o : overflow Flag                           */
    );


#ifdef __cplusplus
}
#endif

#endif  /* _OL_LTP_H_ */


