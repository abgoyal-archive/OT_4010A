

#ifndef PVMP3_SEEK_SYNCH_H
#define PVMP3_SEEK_SYNCH_H

#include "pvmp3_audio_type_defs.h"
#include "pvmp3decoder_api.h"
#include "s_tmp3dec_file.h"
#include "pvmp3_dec_defs.h"








#ifdef __cplusplus
extern "C"
{
#endif

    ERROR_CODE pvmp3_frame_synch(tPVMP3DecoderExternal *pExt,
    void                  *pMem);

    ERROR_CODE pvmp3_header_sync(tmp3Bits  *inputStream);


#ifdef __cplusplus
}
#endif

#endif  /* DECODE_READ_INPUT_H */


