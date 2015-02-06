

#ifndef PVMP3_FRAMEDECODER_H
#define PVMP3_FRAMEDECODER_H

#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"
#include "pvmp3decoder_api.h"
#include "s_mp3bits.h"








#ifdef __cplusplus
extern "C"
{
#endif


    ERROR_CODE pvmp3_framedecoder(tPVMP3DecoderExternal *pExt,
    void                  *pMem);



    uint32 pvmp3_decoderMemRequirements(void);

    void pvmp3_InitDecoder(tPVMP3DecoderExternal *pExt,
                           void  *pMem);


    void pvmp3_resetDecoder(void                  *pMem);


    void fillMainDataBuf(void  *pMem, int32 temp);


#ifdef __cplusplus
}
#endif

#endif

