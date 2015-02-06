
#ifndef bits2prm_h
#define bits2prm_h "$Id $"

#include "typedef.h"
#include "mode.h"

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    *****************************************************************************
    *                         DEFINITION OF DATA TYPES
    *****************************************************************************
    */

    /*
    *****************************************************************************
    *                         DECLARATION OF PROTOTYPES
    *****************************************************************************
    */
    /*
    **************************************************************************
    *
    *  Function    : Bits2prm
    *  Purpose     : Retrieves the vector of encoder parameters from
    *                the received serial bits in a frame.
    *  Returns     : void
    *
    **************************************************************************
    */
    OSCL_IMPORT_REF void Bits2prm(
        enum Mode mode,
        Word16 bits[],   /* input : serial bits, (244 + bfi)               */
        Word16 prm[]     /* output: analysis parameters, (57+1 parameters) */
    );

#ifdef __cplusplus
}
#endif

#endif
