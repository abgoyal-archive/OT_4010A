

#ifndef _SAD_HALFPEL_INLINE_H_
#define _SAD_HALFPEL_INLINE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(__GNUC__) && defined(__arm__) /* ARM GNU COMPILER  */

    __inline int32 INTERP1_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
        tmp = (tmp2 >> 1) - tmp;
        if (tmp > 0) sad += tmp;
        else sad -= tmp;

        return sad;
    }

    __inline int32 INTERP2_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
        tmp = (tmp >> 2) - tmp2;
        if (tmp > 0) sad += tmp;
        else sad -= tmp;

        return sad;
    }

#elif defined(__CC_ARM)  /* only work with arm v5 */

    __inline int32 INTERP1_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
        __asm
        {
            rsbs    tmp, tmp, tmp2, asr #1 ;
            rsbmi   tmp, tmp, #0 ;
            add     sad, sad, tmp ;
        }

        return sad;
    }

    __inline int32 INTERP2_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
        __asm
        {
            rsbs    tmp, tmp2, tmp, asr #2 ;
            rsbmi   tmp, tmp, #0 ;
            add     sad, sad, tmp ;
        }

        return sad;
    }

#elif defined(__GNUC__) && defined(__arm__) /* ARM GNU COMPILER  */

    __inline int32 INTERP1_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
__asm__ volatile("rsbs	%1, %1, %2, asr #1\n\trsbmi %1, %1, #0\n\tadd  %0, %0, %1": "=r"(sad), "=r"(tmp): "r"(tmp2));

        return sad;
    }

    __inline int32 INTERP2_SUB_SAD(int32 sad, int32 tmp, int32 tmp2)
    {
__asm__ volatile("rsbs	%1, %2, %1, asr #2\n\trsbmi %1, %1, #0\n\tadd	%0, %0, %1": "=r"(sad), "=r"(tmp): "r"(tmp2));

        return sad;
    }

#endif

#ifdef __cplusplus
}
#endif

#endif //_SAD_HALFPEL_INLINE_H_

