

#ifndef PVMP3_ALIAS_REDUCTION_H
#define PVMP3_ALIAS_REDUCTION_H

#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"






#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_alias_reduction(int32 *input_buffer,
    granuleInfo *gr_info,
    int32 *used_freq_lines,
    mp3Header *info);

#ifdef __cplusplus
}
#endif



#endif

