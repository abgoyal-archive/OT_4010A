

#ifndef S_TMP3DEC_CHAN_H
#define S_TMP3DEC_CHAN_H

#include "pvmp3_audio_type_defs.h"
#include "pvmp3_dec_defs.h"






#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int32  used_freq_lines;
        int32  overlap[SUBBANDS_NUMBER*FILTERBANK_BANDS];
        int32  work_buf_int32[SUBBANDS_NUMBER*FILTERBANK_BANDS]; /* working buffer */
        int32  circ_buffer[480 + 576];

    } tmp3dec_chan;


#ifdef __cplusplus
}
#endif



#endif


