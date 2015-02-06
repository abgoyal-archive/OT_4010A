

#ifndef PVMP3_MPEG2_STEREO_PROC_H
#define PVMP3_MPEG2_STEREO_PROC_H

#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"








#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_mpeg2_stereo_proc(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    int32 xl[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    mp3ScaleFactors *scalefac,
    granuleInfo *gr_info_l,
    granuleInfo *gr_info_r,
    uint32 *scalefac_IIP_buffer,
    int32 used_freq_lines,
    mp3Header *info);


    void pvmp3_st_intensity_ver2(int32 xr[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                                 int32 xl[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                                 int32 m,
                                 int32 is_pos,
                                 int32 Start,
                                 int32 Number);
#ifdef __cplusplus
}
#endif

#endif


