

#ifndef PVMP3_REORDER_H
#define PVMP3_REORDER_H


#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"








#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_reorder(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    granuleInfo *gr_info,
    int32 *used_freq_lines,
    mp3Header *info,
    int32  Scratch_mem[168]);

#ifdef __cplusplus
}
#endif

#endif


