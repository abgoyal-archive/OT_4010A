
#ifndef residu_h
#define residu_h "$Id $"

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
    void Residu(
        Word16 a[],        /* (i)  : prediction coefficients                    */
        Word16 x[],        /* (i)  : speech signal                              */
        Word16 y[],        /* (o)  : residual signal                            */
        Word16 lg          /* (i)  : size of filtering                          */
    );

#ifdef __cplusplus
}
#endif

#endif
