

#ifndef  S_BITS_H
#define  S_BITS_H

#include "pv_audio_type_defs.h"






typedef struct
{
    UChar    *pBuffer;
    UInt      usedBits;      /* Keep this unsigned so can go to 65536 */
    UInt      availableBits; /* Ditto */
    UInt      inputBufferCurrentLength; /* Ditto */
    Int      byteAlignOffset; /* Used in ADTS.  See find_adts_syncword() */
} BITS;


#endif

