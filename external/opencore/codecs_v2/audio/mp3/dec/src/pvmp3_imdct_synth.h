

#ifndef PVMP3_IMDCT_SYNTH_H
#define PVMP3_IMDCT_SYNTH_H



#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"







#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_imdct_synth(int32 in[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    int32 overlap[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    uint32 blk_type,
    int16 mx_band,
    int32 used_freq_lines,
    int32 *Scratch_mem);

#ifdef __cplusplus
}
#endif


#endif



