

#ifndef PVMP3_GETBITS_H
#define PVMP3_GETBITS_H

#include "pvmp3_dec_defs.h"
#include "s_mp3bits.h"
#include "pvmp3_audio_type_defs.h"


#define INBUF_ARRAY_INDEX_SHIFT  (3)
#define INBUF_BIT_WIDTH         (1<<(INBUF_ARRAY_INDEX_SHIFT))
#define INBUF_BIT_MODULO_MASK   ((INBUF_BIT_WIDTH)-1)





#ifdef __cplusplus
extern "C"
{
#endif

    uint32 getNbits(tmp3Bits *pMainData,
    int32 neededBits);

    uint16 getUpTo9bits(tmp3Bits *pMainData,
                        int32 neededBits);

    uint32 getUpTo17bits(tmp3Bits *pMainData,
                         int32 neededBits);

    uint8 get1bit(tmp3Bits *pMainData);

#ifdef __cplusplus
}
#endif



#endif

