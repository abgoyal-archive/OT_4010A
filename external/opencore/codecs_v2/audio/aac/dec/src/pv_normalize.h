

#ifndef PV_NORMALIZE_H
#define PV_NORMALIZE_H



#include "pv_audio_type_defs.h"




#if defined(_ARM)

__inline Int pv_normalize(Int32 x)
{
    Int32 y;
    __asm
    {
        clz y, x;
        sub y, y, #1
    }
    return (y);
}


#elif (defined(PV_ARM_GCC_V5)||defined(PV_ARM_GCC_V4))

__inline Int pv_normalize(Int32 x)
{
    register Int32 y;
    register Int32 ra = x;


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

    Int pv_normalize(Int32 x);

#ifdef __cplusplus
}
#endif

#endif



#endif  /* PV_NORMALIZE_H */
