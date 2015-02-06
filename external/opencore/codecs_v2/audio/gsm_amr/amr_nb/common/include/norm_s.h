

#ifndef NORM_S_H
#define NORM_S_H

#include    "basicop_malloc.h"

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
#if !( defined(PV_ARM_V5) || defined(PV_ARM_GCC_V5) )

    /* C EQUIVALENT */

    Word16 norm_s(Word16 var1);

#elif defined(PV_ARM_V5)

    __inline Word16  norm_s(Word16 var)
    {
        register Word32 var_out = 0;
        Word32 var1 = var << 16;

        __asm
        {
            CMP    var1, #0
            EORNE  var1, var1, var1, LSL #1
            CLZNE  var_out, var1
        }

        return ((Word16)var_out);
    }

#elif defined(PV_ARM_GCC_V5)

    static inline Word16 norm_s(Word16 var1)
    {
        register Word32 var_out = 0;
        register Word32 ra = var1 << 16;
        if (ra)
        {
            ra ^= (ra << 1);
            asm volatile(
                "clz %0, %1"
    : "=r"(var_out)
                        : "r"(ra)
                    );
        }
        return (var_out);
    }

#endif
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif


