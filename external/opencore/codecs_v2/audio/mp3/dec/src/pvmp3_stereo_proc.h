

#ifndef PVMP3_STEREO_PROC_H
#define PVMP3_STEREO_PROC_H


#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"








#ifdef __cplusplus
extern "C"
{
#endif


    void pvmp3_stereo_proc(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    int32 xl[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    mp3ScaleFactors *scalefac,
    granuleInfo *gr_info,
    int32 used_freq_lines,
    mp3Header *info);

    void pvmp3_st_intensity(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                            int32 xl[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                            int32 is_pos,
                            int32 Start,
                            int32 Number);

    void pvmp3_st_mid_side(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                           int32 xl[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                           int32 Start,
                           int32 Number);
#ifdef __cplusplus
}
#endif

#endif

