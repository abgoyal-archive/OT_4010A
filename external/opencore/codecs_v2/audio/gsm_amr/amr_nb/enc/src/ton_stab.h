

#ifndef TON_STAB_H
#define TON_STAB_H
#define ton_stab_h "$Id $"

#include "typedef.h"
#include "mode.h"
#include "cnst.h"

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
    /* state variable */
    typedef struct
    {
        /* counters */
        Word16 count;
        /* gain history Q11 */
        Word16 gp[N_FRAME];
    } tonStabState;

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    Word16 ton_stab_init(tonStabState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to ton_stab in each call.
       returns 0 on success
     */

    Word16 ton_stab_reset(tonStabState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */

    void ton_stab_exit(tonStabState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    Word16 check_lsp(tonStabState *st, /* i/o : State struct            */
                     Word16 *lsp,      /* i   : unquantized LSP's       */
                     Flag   *pOverflow
                    );

    Word16 check_gp_clipping(tonStabState *st, /* i/o : State struct          */
                             Word16 g_pitch,   /* i   : pitch gain            */
                             Flag  *pOverflow
                            );

    void update_gp_clipping(tonStabState *st, /* i/o : State struct            */
                            Word16 g_pitch,   /* i   : pitch gain              */
                            Flag  *pOverflow
                           );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _TON_STAB_H_ */




