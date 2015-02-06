

#ifndef PVMP3_EQUALIZER_H
#define PVMP3_EQUALIZER_H


#include "pvmp3_audio_type_defs.h"
#include "pvmp3decoder_api.h"









#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_equalizer(int32 *inData,
    e_equalization equalizerType,
    int32 *pt_work_buff);

#ifdef __cplusplus
}
#endif

#endif

