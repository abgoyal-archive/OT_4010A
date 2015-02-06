

#ifndef L_MULT_H
#define L_MULT_H

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

    __inline Word32 L_mult(Word16 var1, Word16 var2, Flag *pOverflow)
    {
        Word32 result;
        Word32 product;

        OSCL_UNUSED_ARG(pOverflow);

        __asm
        {
            SMULBB product, var1, var2
            QADD   result, product, product
        }

        return (result);
    }

#elif defined(PV_ARM_GCC_V5) /* Instructions for ARM-linux cross-compiler*/

    __inline Word32 L_mult(Word16 var1, Word16 var2, Flag *pOverflow)
    {
        register Word32 ra = var1;
        register Word32 rb = var2;
        Word32 result;
        Word32 product;

        OSCL_UNUSED_ARG(pOverflow);

        asm volatile("smulbb %0, %1, %2"
             : "=r"(product)
                             : "r"(ra), "r"(rb)
                            );

        asm volatile("qadd %0, %1, %2"
             : "=r"(result)
                             : "r"(product), "r"(product)
                            );

        return(result);
    }

#else /* C EQUIVALENT */

    static inline Word32 L_mult(Word16 var1, Word16 var2, Flag *pOverflow)
    {
        register Word32 L_product;

        L_product = (Word32) var1 * var2;

        if (L_product != (Word32) 0x40000000L)
        {
            L_product <<= 1;          /* Multiply by 2 */
        }
        else
        {
            *pOverflow = 1;
            L_product = MAX_32;
        }

        return (L_product);
    }
#endif
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _L_MULT_H */

