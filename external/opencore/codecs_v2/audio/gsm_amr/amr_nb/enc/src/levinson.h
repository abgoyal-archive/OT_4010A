

#ifndef _LEVINSON_H_
#define _LEVINSON_H_
#define levinson_h "$Id $"

#include "typedef.h"
#include "cnst.h"

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
        Word16 old_A[M + 1];     /* Last A(z) for case of unstable filter */
    } LevinsonState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 Levinson_init(LevinsonState **st);
    /* initialize one instance of the pre processing state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to Levinson in each call.
       returns 0 on success
     */

    Word16 Levinson_reset(LevinsonState *st);
    /* reset of pre processing state (i.e. set state memory to zero)
       returns 0 on success
     */
    void Levinson_exit(LevinsonState **st);
    /* de-initialize pre processing state (i.e. free status struct)
       stores NULL in *st
     */

    Word16 Levinson(
        LevinsonState *st,
        Word16 Rh[],       /* i : Rh[m+1] Vector of autocorrelations (msb) */
        Word16 Rl[],       /* i : Rl[m+1] Vector of autocorrelations (lsb) */
        Word16 A[],        /* o : A[m]    LPC coefficients  (m = 10)       */
        Word16 rc[],        /* o : rc[4]   First 4 reflection coefficients  */
        Flag   *pOverflow
    );



#ifdef __cplusplus
}
#endif

#endif  /* _LEVINSON_H_ */




