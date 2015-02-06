

#ifndef  PVMP3_MDCT_6_H
#define  PVMP3_MDCT_6_H

#include "pvmp3_audio_type_defs.h"


#define Qfmt(a)    (Int32)(a*((Int32)1<<28) )
#define Qfmt1(a)   (Int32)(a*((Int32)0x7FFFFFFF))
#define Qfmt2(a)   (Int32)(a*((Int32)1<<27))





#ifdef __cplusplus
extern "C"
{
#endif


    void pvmp3_mdct_6(int32 vec[], int32 *overlap);

    void pvmp3_dct_6(int32 vec[]);

#ifdef __cplusplus
}
#endif

#endif


