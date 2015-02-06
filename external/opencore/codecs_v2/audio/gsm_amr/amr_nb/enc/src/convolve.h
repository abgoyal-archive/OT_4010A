
#ifndef convolve_h
#define convolve_h "$Id $"

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
    void Convolve(
        Word16 x[],        /* (i)  : input vector                               */
        Word16 h[],        /* (i)  : impulse response                           */
        Word16 y[],        /* (o)  : output vector                              */
        Word16 L           /* (i)  : vector size                                */
    );

#ifdef __cplusplus
}
#endif

#endif
