

#ifndef  S_MP3BITS_H
#define  S_MP3BITS_H

#include "pvmp3_audio_type_defs.h"








typedef struct
{
    uint8       *pBuffer;
    uint32      usedBits;
    uint32      inputBufferCurrentLength;
    uint32      offset;
} tmp3Bits;



#endif

