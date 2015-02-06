
#ifndef weight_a_h
#define weight_a_h "$Id $"

#include "typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ********************************************************************************
    *                         DEFINITION OF DATA TYPES
    ********************************************************************************
    */

    /*
    ********************************************************************************
    *                         DECLARATION OF PROTOTYPES
    ********************************************************************************
    */
    void Weight_Ai(
        Word16 a[],        /* (i)  : a[m+1]  LPC coefficients   (m=10)          */
        const Word16 fac[],/* (i)  : Spectral expansion factors.                */
        Word16 a_exp[]     /* (o)  : Spectral expanded LPC coefficients         */
    );

#ifdef __cplusplus
}
#endif

#endif
