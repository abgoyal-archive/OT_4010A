

#ifndef _LPC_H_
#define _LPC_H_
#define lpc_h "$Id $"

#include "typedef.h"
#include "levinson.h"
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
        LevinsonState *levinsonSt;
    } lpcState;


    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 lpc_init(lpcState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to lpc in each call.
       returns 0 on success
     */

    Word16 lpc_reset(lpcState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */
    void lpc_exit(lpcState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    void lpc(
        lpcState *st,     /* i/o: State struct                */
        enum Mode mode,   /* i  : coder mode                  */
        Word16 x[],       /* i  : Input signal           Q15  */
        Word16 x_12k2[],  /* i  : Input signal (EFR)     Q15  */
        Word16 a[],       /* o  : predictor coefficients Q12  */
        Flag   *pOverflow
    );


#ifdef __cplusplus
}
#endif

#endif  /* _LPC_H_ */




