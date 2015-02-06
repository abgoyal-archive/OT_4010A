
#ifndef syn_filt_h
#define syn_filt_h "$Id $"

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
    void Syn_filt(
        Word16 a[],        /* (i)  : a[m+1] prediction coefficients   (m=10)    */
        Word16 x[],        /* (i)  : input signal                               */
        Word16 y[],        /* (o)  : output signal                              */
        Word16 lg,         /* (i)  : size of filtering                          */
        Word16 mem[],      /* (i/o): memory associated with this filtering.     */
        Word16 update      /* (i)  : 0=no update, 1=update of memory.           */
    );

#ifdef __cplusplus
}
#endif

#endif
