

#ifndef SET_SIGN_H
#define SET_SIGN_H "@(#)$Id $"

#include    "basicop_malloc.h"

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
    void set_sign(Word16 dn[],   /* i/o : correlation between target and h[]    */
    Word16 sign[], /* o   : sign of dn[]                          */
    Word16 dn2[],  /* o   : maximum of correlation in each track. */
    Word16 n       /* i   : # of maximum correlations in dn2[]    */
                 );

    void set_sign12k2(
        Word16 dn[],        /* i/o : correlation between target and h[]         */
        Word16 cn[],        /* i   : residual after long term prediction        */
        Word16 sign[],      /* o   : sign of d[n]                               */
        Word16 pos_max[],   /* o   : position of maximum correlation            */
        Word16 nb_track,    /* i   : number of tracks tracks                    */
        Word16 ipos[],      /* o   : starting position for each pulse           */
        Word16 step,        /* i   : the step size in the tracks                */
        Flag   *pOverflow   /* i/o : overflow flag                              */
    );
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _SET_SIGN_H_ */
