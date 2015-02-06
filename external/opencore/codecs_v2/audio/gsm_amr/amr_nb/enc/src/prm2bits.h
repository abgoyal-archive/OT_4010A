
#ifndef prm2bits_h
#define prm2bits_h "$Id $"

#include "typedef.h"
#include "mode.h"

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
    void Prm2bits(
        enum Mode mode,    /* i : AMR mode */
        Word16 prm[],      /* input : analysis parameters                       */
        Word16 bits[]      /* output: serial bits                               */
    );

#ifdef __cplusplus
}
#endif

#endif
