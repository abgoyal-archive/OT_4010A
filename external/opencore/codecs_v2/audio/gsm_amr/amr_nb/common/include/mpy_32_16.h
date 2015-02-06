

#ifndef MPY_32_16_H
#define MPY_32_16_H

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

    __inline Word32 Mpy_32_16(Word16 L_var1_hi,
    Word16 L_var1_lo,
    Word16 var2,
    Flag *pOverflow)
    {

        Word32 L_product;
        Word32 L_sum;
        Word32 result;

        OSCL_UNUSED_ARG(pOverflow);

        __asm {SMULBB L_product, L_var1_hi, var2}
        __asm {QDADD L_product, 0, L_product}
        __asm {SMULBB result, L_var1_lo, var2}
        result >>= 15;
        __asm {QDADD L_sum, L_product, result}
        return (L_sum);
    }

#elif defined(PV_ARM_GCC_V5) /* Instructions for ARM-linux cross-compiler*/

    static inline Word32 Mpy_32_16(Word16 L_var1_hi,
                                   Word16 L_var1_lo,
                                   Word16 var2,
                                   Flag *pOverflow)
    {

        register Word32 ra = L_var1_hi;
        register Word32 rb = L_var1_lo;
        register Word32 rc = var2;
        Word32 result, L_product;

        OSCL_UNUSED_ARG(pOverflow);

        asm volatile("smulbb %0, %1, %2"
             : "=r"(L_product)
                             : "r"(ra), "r"(rc)
                            );
        asm volatile("mov %0, #0"
             : "=r"(result)
                    );

        asm volatile("qdadd %0, %1, %2"
             : "=r"(L_product)
                             : "r"(result), "r"(L_product)
                            );

        asm volatile("smulbb %0, %1, %2"
             : "=r"(result)
                             : "r"(rb), "r"(rc)
                            );

        asm volatile("mov %0, %1, ASR #15"
             : "=r"(ra)
                             : "r"(result)
                            );
        asm volatile("qdadd %0, %1, %2"
             : "=r"(result)
                             : "r"(L_product), "r"(ra)
                            );

        return (result);
    }

#else /* C_EQUIVALENT */
    __inline Word32 Mpy_32_16(Word16 L_var1_hi,
                              Word16 L_var1_lo,
                              Word16 var2,
                              Flag *pOverflow)
    {

        Word32 L_product;
        Word32 L_sum;
        Word32 result;
        L_product = (Word32) L_var1_hi * var2;

        if (L_product != (Word32) 0x40000000L)
        {
            L_product <<= 1;
        }
        else
        {
            *pOverflow = 1;
            L_product = MAX_32;
        }

        result = ((Word32)L_var1_lo * var2) >> 15;

        L_sum  =  L_product + (result << 1);

        if ((L_product ^ result) > 0)
        {
            if ((L_sum ^ L_product) < 0)
            {
                L_sum = (L_product < 0) ? MIN_32 : MAX_32;
                *pOverflow = 1;
            }
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

#endif /* _MPY_32_16_H_ */


