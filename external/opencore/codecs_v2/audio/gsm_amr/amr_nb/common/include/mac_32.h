

#ifndef MAC_32_H
#define MAC_32_H

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

    static inline Word32 Mac_32(Word32 L_var3,
    Word16 L_var1_hi,
    Word16 L_var1_lo,
    Word16 L_var2_hi,
    Word16 L_var2_lo,
    Flag *pOverflow)
    {
        Word16  product;

        L_var3 = L_mac(L_var3, L_var1_hi, L_var2_hi, pOverflow);

        product = mult(L_var1_hi, L_var2_lo, pOverflow);
        L_var3 = L_mac(L_var3, product, 1, pOverflow);

        product = mult(L_var1_lo, L_var2_hi, pOverflow);
        L_var3 = L_mac(L_var3, product, 1, pOverflow);

        return (L_var3);
    }

    static inline Word32 Mac_32_16(Word32 L_var3,
                                   Word16 L_var1_hi,
                                   Word16 L_var1_lo,
                                   Word16 var2,
                                   Flag  *pOverflow)
    {
        Word16  product;

        L_var3 = L_mac(L_var3, L_var1_hi, var2, pOverflow);

        product = mult(L_var1_lo, var2, pOverflow);
        L_var3 = L_mac(L_var3, product, 1, pOverflow);

        return (L_var3);
    }

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _MAC_32_H_ */


