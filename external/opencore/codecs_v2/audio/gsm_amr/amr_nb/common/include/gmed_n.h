
#ifndef gmed_n_h
#define gmed_n_h "$Id $"

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
    Word16 gmed_n(    /* o : index of the median value (0...N-1)      */
        Word16 ind[], /* i : Past gain values                         */
        Word16 n      /* i : The number of gains; this routine        */
        /*     is only valid for a odd number of gains  */
    );


#ifdef __cplusplus
}
#endif

#endif
