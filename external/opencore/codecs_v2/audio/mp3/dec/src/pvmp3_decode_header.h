

#ifndef PVMP3_DECODE_HEADER_H
#define PVMP3_DECODE_HEADER_H


#include "pvmp3_audio_type_defs.h"
#include "pvmp3_dec_defs.h"
#include "s_mp3bits.h"








#ifdef __cplusplus
extern "C"
{
#endif

    ERROR_CODE pvmp3_decode_header(tmp3Bits  *inputStream,
    mp3Header  *info,
    uint32 *crc);

#ifdef __cplusplus
}
#endif

#endif

