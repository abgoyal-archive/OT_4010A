

#ifndef NORMALIZE_AMR_WB_H
#define NORMALIZE_AMR_WB_H



#include "pv_amr_wb_type_defs.h"




#if defined(PV_ARM_V5)

__inline int16 normalize_amr_wb(int32 x)
{
    int32 y;
    __asm
    {
        clz y, x;
        sub y, y, #1
    }
    return (y);
}


#elif defined(PV_ARM_GCC_V5)


__inline int16 normalize_amr_wb(int32 x)
{
    register int32 y;
    register int32 ra = x;


    asm volatile(
        "clz %0, %1\n\t"
        "sub %0, %0, #1"
    : "=&r*i"(y)
                : "r"(ra));
    return (y);

}

#else

#ifdef __cplusplus
extern "C"
{
#endif

    int16 normalize_amr_wb(int32 x);

#ifdef __cplusplus
}
#endif

#endif



#endif  /* PV_NORMALIZE_H */
