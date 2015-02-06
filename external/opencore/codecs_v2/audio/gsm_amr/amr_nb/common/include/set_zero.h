
#ifndef set_zero_h
#define set_zero_h "$Id $"

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
    void Set_zero(
        Word16 x[],        /* (o)  : vector to clear                            */
        Word16 L           /* (i)  : length of vector                           */
    );

#ifdef __cplusplus
}
#endif

#endif
