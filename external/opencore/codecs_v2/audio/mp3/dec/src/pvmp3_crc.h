

#ifndef PVMP3_CRC_H
#define PVMP3_CRC_H

#include "pvmp3_audio_type_defs.h"
#include "s_mp3bits.h"
#include "pvmp3decoder_api.h"


#define         CRC16_POLYNOMIAL        0x8005




#ifdef __cplusplus
extern "C"
{
#endif

    uint32 getbits_crc(tmp3Bits *inputStream,
    int32 neededBits,
    uint32 *crc,
    uint32 crc_enabled);


    void calculate_crc(uint32 data,
                       uint32 length,
                       uint32 *crc);

#ifdef __cplusplus
}
#endif



#endif



