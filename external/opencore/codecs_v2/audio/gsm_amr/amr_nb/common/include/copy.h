
#ifndef copy_h
#define copy_h "$Id $"

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

    /*
    **************************************************************************
    *
    *  Function    : Copy
    *  Purpose     : Copy vector x[] to y[], vector length L
    *  Returns     : void
    *
    **************************************************************************
    */
    void Copy(
        const Word16 x[],  /* i : input vector (L)    */
        Word16 y[],        /* o : output vector (L)   */
        Word16 L           /* i : vector length       */
    );

#ifdef __cplusplus
}
#endif

#endif
