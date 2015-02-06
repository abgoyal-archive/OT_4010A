

#ifndef PV_DIV_H
#define PV_DIV_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; INCLUDES
    ----------------------------------------------------------------------------*/

#include "pv_audio_type_defs.h"

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES AND SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    struct intg_div
    {
        Int32 quotient;
        Int32 shift_factor;
    };
    typedef struct intg_div Quotient;


    void pv_div(Int32 x, Int32 y, Quotient *quotient);

#ifdef __cplusplus
}
#endif

#endif  /* PV_DIV_H */
