

#ifndef PVMP3_DEQUANTIZE_SAMPLE_H
#define PVMP3_DEQUANTIZE_SAMPLE_H


#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"









#ifdef __cplusplus
extern "C"
{
#endif

    int32 power_1_third(int32 xx);

    void pvmp3_dequantize_sample(int32 is[SUBBANDS_NUMBER*FILTERBANK_BANDS],
                                 mp3ScaleFactors *scalefac,
                                 granuleInfo *gr_info,
                                 int32  num_lines,
                                 mp3Header *info);

#ifdef __cplusplus
}
#endif

#endif


