

#ifndef MPY_32_H
#define MPY_32_H

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

    __inline Word32 Mpy_32(Word16 L_var1_hi,
    Word16 L_var1_lo,
    Word16 L_var2_hi,
    Word16 L_var2_lo,
    Flag   *pOverflow)

    {
        /*----------------------------------------------------------------------------
        ; Define all local variables
        ----------------------------------------------------------------------------*/
        Word32 L_product;
        Word32 L_sum;
        Word32 product32;

        OSCL_UNUSED_ARG(pOverflow);
        /*----------------------------------------------------------------------------
        ; Function body here
        ----------------------------------------------------------------------------*/
        /* L_product = L_mult (L_var1_hi, L_var2_hi, pOverflow);*/

        __asm {SMULBB L_product, L_var1_hi, L_var2_hi}
        __asm {QDADD L_product, 0, L_product}
        __asm {SMULBB product32, L_var1_hi, L_var2_lo}
        product32 >>= 15;
        __asm {QDADD L_sum, L_product, product32}
        L_product = L_sum;
        __asm {SMULBB product32, L_var1_lo, L_var2_hi}
        product32 >>= 15;
        __asm {QDADD L_sum, L_product, product32}
        return (L_sum);
    }

#elif defined(PV_ARM_GCC_V5) /* Instructions for ARM-linux cross-compiler*/

    static inline Word32 Mpy_32(Word16 L_var1_hi,
                                Word16 L_var1_lo,
                                Word16 L_var2_hi,
                                Word16 L_var2_lo,
                                Flag   *pOverflow)
    {
        register Word32 product32;
        register Word32 L_sum;
        register Word32 L_product, result;
        register Word32 ra = L_var1_hi;
        register Word32 rb = L_var1_lo;
        register Word32 rc = L_var2_hi;
        register Word32 rd = L_var2_lo;



        OSCL_UNUSED_ARG(pOverflow);

        asm volatile("smulbb %0, %1, %2"
             : "=r"(L_product)
                             : "r"(ra), "r"(rc)
                            );
        asm volatile("mov %0, #0"
             : "=r"(result)
                    );

        asm volatile("qdadd %0, %1, %2"
             : "=r"(L_sum)
                             : "r"(result), "r"(L_product)
                            );

        asm volatile("smulbb %0, %1, %2"
             : "=r"(product32)
                             : "r"(ra), "r"(rd)
                            );

        asm volatile("mov %0, %1, ASR #15"
             : "=r"(ra)
                             : "r"(product32)
                            );
        asm volatile("qdadd %0, %1, %2"
             : "=r"(L_product)
                             : "r"(L_sum), "r"(ra)
                            );

        asm volatile("smulbb %0, %1, %2"
             : "=r"(product32)
                             : "r"(rb), "r"(rc)
                            );

        asm volatile("mov %0, %1, ASR #15"
             : "=r"(rb)
                             : "r"(product32)
                            );

        asm volatile("qdadd %0, %1, %2"
             : "=r"(L_sum)
                             : "r"(L_product), "r"(rb)
                            );

        return (L_sum);
    }

#else /* C_EQUIVALENT */

    __inline Word32 Mpy_32(Word16 L_var1_hi,
                           Word16 L_var1_lo,
                           Word16 L_var2_hi,
                           Word16 L_var2_lo,
                           Flag   *pOverflow)
    {
        Word32 L_product;
        Word32 L_sum;
        Word32 product32;

        OSCL_UNUSED_ARG(pOverflow);
        L_product = (Word32) L_var1_hi * L_var2_hi;

        if (L_product != (Word32) 0x40000000L)
        {
            L_product <<= 1;
        }
        else
        {
            L_product = MAX_32;
        }

        /* result = mult (L_var1_hi, L_var2_lo, pOverflow); */
        product32 = ((Word32) L_var1_hi * L_var2_lo) >> 15;

        /* L_product = L_mac (L_product, result, 1, pOverflow); */
        L_sum = L_product + (product32 << 1);

        if ((L_product ^ product32) > 0)
        {
            if ((L_sum ^ L_product) < 0)
            {
                L_sum = (L_product < 0) ? MIN_32 : MAX_32;
            }
        }

        L_product = L_sum;

        /* result = mult (L_var1_lo, L_var2_hi, pOverflow); */
        product32 = ((Word32) L_var1_lo * L_var2_hi) >> 15;

        /* L_product = L_mac (L_product, result, 1, pOverflow); */
        L_sum = L_product + (product32 << 1);

        if ((L_product ^ product32) > 0)
        {
            if ((L_sum ^ L_product) < 0)
            {
                L_sum = (L_product < 0) ? MIN_32 : MAX_32;
            }
        }

        /*----------------------------------------------------------------------------
        ; Return nothing or data or data pointer
        ----------------------------------------------------------------------------*/
        return (L_sum);
    }

#endif
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _MPY_32_H_ */
