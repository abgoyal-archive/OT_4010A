

#ifndef  MDCT_18_H
#define  MDCT_18_H

#include "pvmp3_audio_type_defs.h"


#define Qfmt(a)    (Int32)(a*((Int32)1<<28) )
#define Qfmt1(a)   (Int32)(a*((Int32)0x7FFFFFFF))
#define Qfmt2(a)   (Int32)(a*((Int32)1<<27))





#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_mdct_18(int32 vec[], int32 *history, const int32 *window);

    void pvmp3_dct_9(int32 vec[]);

    void pvmp3_mdct_6(int32 vec[], int32 *overlap);

    void pvmp3_dct_6(int32 vec[]);

#ifdef __cplusplus
}
#endif

#endif


