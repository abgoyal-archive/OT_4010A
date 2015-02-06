

#ifndef NORM_L_H
#define NORM_L_H

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
    Word16 norm_l(Word32 L_var1);

#elif defined(PV_ARM_V5)

    __inline Word16 norm_l(Word32 L_var1)
    {
        register Word32 var_out = 0;

        __asm
        {
            CMP    L_var1, #0
            EORNE  L_var1, L_var1, L_var1, LSL #1
            CLZNE  var_out, L_var1
        }

        return ((Word16)var_out);
    }

#elif defined(PV_ARM_GCC_V5)

    static inline Word16 norm_l(Word32 L_var1)
    {
        register Word32 var_out = 0;
        register Word32 ra = L_var1;
        if (L_var1)
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


