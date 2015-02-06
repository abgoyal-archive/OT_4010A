

#ifndef _AGC_H_
#define _AGC_H_

#include "typedef.h"

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
        Word16 past_gain;
    } agcState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    /*----------------------------------------------------------------------------
    ;
    ;  Function    : agc_reset
    ;  Purpose     : Reset of agc (i.e. set state memory to 1.0)
    ;  Returns     : 0 on success
    ;
    ----------------------------------------------------------------------------*/
    Word16 agc_reset(agcState *st);


    /*----------------------------------------------------------------------------
    ;
    ;  Function    : agc
    ;  Purpose     : Scales the postfilter output on a subframe basis
    ;  Description : sig_out[n] = sig_out[n] * gain[n];
    ;                where gain[n] is the gain at the nth sample given by
    ;                gain[n] = agc_fac * gain[n-1] + (1 - agc_fac) g_in/g_out
    ;                g_in/g_out is the square root of the ratio of energy at
    ;                the input and output of the postfilter.
    ;
    ----------------------------------------------------------------------------*/
    void agc(
        agcState *st,      /* i/o : agc state                         */
        Word16 *sig_in,    /* i   : postfilter input signal, (l_trm)  */
        Word16 *sig_out,   /* i/o : postfilter output signal, (l_trm) */
        Word16 agc_fac,    /* i   : AGC factor                        */
        Word16 l_trm,      /* i   : subframe size                     */
        Flag *pOverflow    /* i   : overflow flag                     */
    );

    /*----------------------------------------------------------------------------
    ;
    ;  Function:  agc2
    ;  Purpose:   Scales the excitation on a subframe basis
    ;
    ----------------------------------------------------------------------------*/
    void agc2(
        Word16 *sig_in,    /* i   : postfilter input signal   */
        Word16 *sig_out,   /* i/o : postfilter output signal  */
        Word16 l_trm,      /* i   : subframe size             */
        Flag *pOverflow    /* i   : overflow flag             */
    );

#ifdef __cplusplus
}
#endif

#endif  /* _AGC_H_ */


