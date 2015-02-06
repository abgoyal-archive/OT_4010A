

#ifndef L_SUB_H
#define L_SUB_H


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
#if defined(PV_ARM_V5) /* Instructions for ARM Assembly on ADS*/

    __inline Word32 L_sub(Word32 L_var1, Word32 L_var2, Flag *pOverflow)
    {
        Word32 result;

        OSCL_UNUSED_ARG(pOverflow);

        __asm
        {
            QSUB result, L_var1, L_var2
        }

        return(result);

    }

#elif defined(PV_ARM_GCC_V5) /* Instructions for ARM-linux cross-compiler*/

    __inline Word32 L_sub(Word32 L_var1, Word32 L_var2, Flag *pOverflow)
    {
        register Word32 ra = L_var1;
        register Word32 rb = L_var2;
        Word32 result;

        OSCL_UNUSED_ARG(pOverflow);

        asm volatile("qsub %0, %1, %2"
             : "=r"(result)
                             : "r"(ra), "r"(rb)
                            );

        return (result);
    }

#else /* C EQUIVALENT */

    static inline Word32 L_sub(register Word32 L_var1, register Word32 L_var2,
                               register Flag *pOverflow)
    {
        Word32 L_diff;

        L_diff = L_var1 - L_var2;

        if ((L_var1 ^ L_var2) < 0)
        {
            if ((L_diff ^ L_var1) & MIN_32)
            {
                L_diff = (L_var1 < 0L) ? MIN_32 : MAX_32;
                *pOverflow = 1;
            }
        }

        return (L_diff);
    }

#endif
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _L_SUB_H_ */


