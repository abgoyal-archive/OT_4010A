

#ifndef L_MAC_H
#define L_MAC_H

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

    __inline Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2, Flag *pOverflow)
    {
        Word32 result;
        Word32 L_sum;

        OSCL_UNUSED_ARG(pOverflow);

        __asm {SMULBB result, var1, var2}
        __asm {QDADD L_sum, L_var3, result}
        return (L_sum);
    }

#elif defined(PV_ARM_GCC_V5) /* Instructions for ARM-linux cross-compiler*/

    static inline Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2, Flag *pOverflow)
    {
        register Word32 ra = L_var3;
        register Word32 rb = var1;
        register Word32 rc = var2;
        Word32 result;

        OSCL_UNUSED_ARG(pOverflow);

        asm volatile("smulbb %0, %1, %2"
             : "=r"(result)
                             : "r"(rb), "r"(rc)
                            );

        asm volatile("qdadd %0, %1, %2"
             : "=r"(rc)
                             : "r"(ra), "r"(result)
                            );

        return (rc);
    }

#else /* C_EQUIVALENT */

    __inline Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2, Flag *pOverflow)
    {
        Word32 result;
        Word32 L_sum;
        result = (Word32) var1 * var2;
        if (result != (Word32) 0x40000000L)
        {
            L_sum = (result << 1) + L_var3;

            /* Check if L_sum and L_var_3 share the same sign */
            if ((L_var3 ^ result) > 0)
            {
                if ((L_sum ^ L_var3) < 0)
                {
                    L_sum = (L_var3 < 0) ? MIN_32 : MAX_32;
                    *pOverflow = 1;
                }
            }
        }
        else
        {
            *pOverflow = 1;
            L_sum = MAX_32;
        }
        return (L_sum);
    }

#endif
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _L_MAC_H_ */


