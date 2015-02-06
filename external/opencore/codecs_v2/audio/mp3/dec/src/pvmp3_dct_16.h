

#ifndef PVMP3_DCT_16_H
#define PVMP3_DCT_16_H

#include "pvmp3_audio_type_defs.h"






#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_dct_16(int32 vec[], int32 flag);

    void pvmp3_merge_in_place_N32(int32 vec[]);

    void pvmp3_split(int32 *vect);


#ifdef __cplusplus
}
#endif



#endif

