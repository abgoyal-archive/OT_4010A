


#ifndef PVAMRWBDECODER_BASIC_OP_H
#define PVAMRWBDECODER_BASIC_OP_H


#include "normalize_amr_wb.h"


#define MAX_32 (int32)0x7fffffffL
#define MIN_32 (int32)0x80000000L

#define MAX_16 (int16)+32767    /* 0x7fff */
#define MIN_16 (int16)-32768    /* 0x8000 */





__inline int16 negate_int16(int16 var1)
{
    return (((var1 == MIN_16) ? MAX_16 : -var1));
}



__inline int16 shl_int16(int16 var1, int16 var2)
{
    int16 var_out;

    if (var2 < 0)
    {
        var2 = (-var2) & (0xf);
        var_out = var1 >> var2;
    }
    else
    {
        var2 &= 0xf;
        var_out = var1 << var2;
        if (var_out >> var2 != var1)
        {
            var_out = (var1 >> 15) ^ MAX_16;
        }
    }
    return (var_out);
}



__inline int32 shl_int32(int32 L_var1, int16 var2)
{
    int32 L_var_out;

    if (var2 > 0)
    {
        L_var_out = L_var1 << var2;
        if (L_var_out >> var2 != L_var1)
        {
            L_var_out = (L_var1 >> 31) ^ MAX_32;
        }
    }
    else
    {
        var2 = (-var2) & (0xf);
        L_var_out = L_var1 >> var2;
    }

    return (L_var_out);
}



__inline int32 shr_int32(int32 L_var1, int16 var2)
{
    int32 L_var_out;

    if (var2 >= 0)
    {
        L_var_out = L_var1 >> (var2 & 0x1f);
    }
    else
    {
        var2 = (int16)(-var2);
        var2 &= 0x1f;
        L_var_out = L_var1 << var2;
        if (L_var_out >> var2 != L_var1)
        {
            L_var_out = (L_var1 >> 31) ^ MAX_32;
        }

    }
    return (L_var_out);
}






#if defined(PV_ARM_V5)

#include "pvamrwbdecoder_basic_op_armv5.h"

#elif defined(PV_ARM_GCC_V5)

#include "pvamrwbdecoder_basic_op_gcc_armv5.h"

#else

#ifndef C_EQUIVALENT
#define C_EQUIVALENT        // default to C_EQUIVALENT
#endif

#include "pvamrwbdecoder_basic_op_cequivalent.h"

#endif


#endif   /*  PVAMRWBDECODER_BASIC_OP_H  */

