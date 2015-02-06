

#ifndef PH_DISP_H
#define PH_DISP_H "$Id $"

#include    "typedef.h"
#include    "mode.h"

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
#define PHDGAINMEMSIZE 5
#define PHDTHR1LTP     9830  /* 0.6 in Q14 */
#define PHDTHR2LTP     14746 /* 0.9 in Q14 */
#define ONFACTPLUS1    16384 /* 2.0 in Q13   */
#define ONLENGTH 2

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/
    extern Word16 ph_imp_low_MR795[];
    extern Word16 ph_imp_mid_MR795[];
    extern Word16 ph_imp_low[];
    extern Word16 ph_imp_mid[];

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
        Word16 gainMem[PHDGAINMEMSIZE];
        Word16 prevState;
        Word16 prevCbGain;
        Word16 lockFull;
        Word16 onset;
    } ph_dispState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ;
    ;  Function:   ph_disp_reset
    ;  Purpose:    Initializes state memory
    ;
    ----------------------------------------------------------------------------*/
    Word16 ph_disp_reset(ph_dispState *state);

    /*----------------------------------------------------------------------------
    ;
    ;  Function:   ph_disp_exit
    ;  Purpose:    The memory used for state memory is freed
    ;
    ----------------------------------------------------------------------------*/
    void ph_disp_exit(ph_dispState **state);

    /*----------------------------------------------------------------------------
    ;
    ;  Function:   ph_disp_lock
    ;  Purpose:    mark phase dispersion as locked in state struct
    ;
    ----------------------------------------------------------------------------*/
    void ph_disp_lock(ph_dispState *state);

    /*----------------------------------------------------------------------------
    ;
    ;  Function:   ph_disp_release
    ;  Purpose:    mark phase dispersion as unlocked in state struct
    ;
    ----------------------------------------------------------------------------*/

    void ph_disp_release(ph_dispState *state);

    /*----------------------------------------------------------------------------
    ;
    ;  Function:   ph_disp
    ;  Purpose:    perform phase dispersion according to the specified codec
    ;              mode and computes total excitation for synthesis part
    ;              if decoder
    ;
    ----------------------------------------------------------------------------*/

    void ph_disp(
        ph_dispState *state,    /* i/o     : State struct                       */
        enum Mode mode,         /* i       : codec mode                         */
        Word16 x[],             /* i/o Q0  : in:  LTP excitation signal         */
        /*           out: total excitation signal       */
        Word16 cbGain,          /* i   Q1  : Codebook gain                      */
        Word16 ltpGain,         /* i   Q14 : LTP gain                           */
        Word16 inno[],          /* i/o Q13 : Innovation vector (Q12 for 12.2)   */
        Word16 pitch_fac,       /* i   Q14 : pitch factor used to scale the
                                         LTP excitation (Q13 for 12.2)      */
        Word16 tmp_shift,       /* i   Q0  : shift factor applied to sum of
                                         scaled LTP ex & innov. before
                                         rounding                           */
        Flag   *pOverflow       /* i/o     : oveflow indicator                  */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _PH_DISP_H_ */

