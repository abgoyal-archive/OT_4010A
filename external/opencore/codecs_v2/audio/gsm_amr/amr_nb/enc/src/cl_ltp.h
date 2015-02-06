

#ifndef cl_ltp_h
#define cl_ltp_h "$Id $"


#include "typedef.h"
#include "mode.h"
#include "pitch_fr.h"
#include "ton_stab.h"

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
        Pitch_frState *pitchSt;
    } clLtpState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    Word16 cl_ltp_init(clLtpState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to cl_ltp in each call.
       returns 0 on success
     */

    Word16 cl_ltp_reset(clLtpState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */
    void cl_ltp_exit(clLtpState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    void cl_ltp(
        clLtpState *clSt,    /* i/o : State struct                              */
        tonStabState *tonSt, /* i/o : State struct                              */
        enum Mode mode,      /* i   : coder mode                                */
        Word16 frameOffset,  /* i   : Offset to subframe                        */
        Word16 T_op[],       /* i   : Open loop pitch lags                      */
        Word16 *h1,          /* i   : Impulse response vector               Q12 */
        Word16 *exc,         /* i/o : Excitation vector                      Q0 */
        Word16 res2[],       /* i/o : Long term prediction residual          Q0 */
        Word16 xn[],         /* i   : Target vector for pitch search         Q0 */
        Word16 lsp_flag,     /* i   : LSP resonance flag                        */
        Word16 xn2[],        /* o   : Target vector for codebook search      Q0 */
        Word16 y1[],         /* o   : Filtered adaptive excitation           Q0 */
        Word16 *T0,          /* o   : Pitch delay (integer part)                */
        Word16 *T0_frac,     /* o   : Pitch delay (fractional part)             */
        Word16 *gain_pit,    /* o   : Pitch gain                            Q14 */
        Word16 g_coeff[],    /* o   : Correlations between xn, y1, & y2         */
        Word16 **anap,       /* o   : Analysis parameters                       */
        Word16 *gp_limit,    /* o   : pitch gain limit                          */
        Flag   *pOverflow    /* o   : overflow indicator                        */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _CL_LTP_H_ */

