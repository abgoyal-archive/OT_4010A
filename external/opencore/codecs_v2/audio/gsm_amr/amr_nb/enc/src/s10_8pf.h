

#ifndef S10_8PF_H
#define S10_8PF_H

#include    "basicop_malloc.h"
#include    "cnst.h"

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

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    void search_10and8i40(
        Word16 nbPulse,       /* i : nbpulses to find                       */
        Word16 step,          /* i :  stepsize                              */
        Word16 nbTracks,      /* i :  nbTracks                              */
        Word16 dn[],          /* i : correlation between target and h[]     */
        Word16 rr[][L_CODE],  /* i : matrix of autocorrelation              */
        Word16 ipos[],        /* i : starting position for each pulse       */
        Word16 pos_max[],     /* i : position of maximum of dn[]            */
        Word16 codvec[],      /* o : algebraic codebook vector              */
        Flag   *pOverflow     /* i/o : Overflow flag                        */
    );
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _S10_8PF_H_ */
