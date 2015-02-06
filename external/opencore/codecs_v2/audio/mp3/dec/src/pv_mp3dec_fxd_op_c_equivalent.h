

#ifndef PV_MP3DEC_FXD_OP_C_EQUIVALENT
#define PV_MP3DEC_FXD_OP_C_EQUIVALENT


#ifdef __cplusplus
extern "C"
{
#endif

#include "pvmp3_audio_type_defs.h"
#define Qfmt_31(a)   (Int32)((float)a*0x7FFFFFFF)

#define Qfmt15(x)   (Int16)(x*((Int32)1<<15) + (x>=0?0.5F:-0.5F))



    __inline int32 pv_abs(int32 a)
    {
        int32 b = (a < 0) ? -a : a;
        return b;
    }




    __inline Int32 fxp_mul32_Q30(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 30);
    }

    __inline Int32 fxp_mac32_Q30(const Int32 a, const Int32 b, Int32 L_add)
    {
        return (L_add + (Int32)(((int64)(a) * b) >> 30));
    }

    __inline Int32 fxp_mul32_Q32(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 32);
    }


    __inline Int32 fxp_mul32_Q28(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 28);
    }

    __inline Int32 fxp_mul32_Q27(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 27);
    }

    __inline Int32 fxp_mul32_Q26(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 26);
    }


    __inline Int32 fxp_mac32_Q32(Int32 L_add, const Int32 a, const Int32 b)
    {
        return (L_add + (Int32)(((int64)(a) * b) >> 32));
    }

    __inline Int32 fxp_msb32_Q32(Int32 L_sub, const Int32 a, const Int32 b)
    {
        return (L_sub - ((Int32)(((int64)(a) * b) >> 32)));
    }


    __inline Int32 fxp_mul32_Q29(const Int32 a, const Int32 b)
    {
        return (Int32)(((int64)(a) * b) >> 29);
    }






#ifdef __cplusplus
}
#endif


#endif   /*  PV_MP3DEC_FXD_OP_C_EQUIVALENT  */



