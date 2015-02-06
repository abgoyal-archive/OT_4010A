
#ifndef _BITSTREAM_IO_H_
#define _BITSTREAM_IO_H_

#define BitstreamPut1Bits(x,y)  BitstreamPutBits(x,1,y)
#define BitstreamPutGT8Bits(x,y,z) BitstreamPutBits(x,y,z)

#include "mp4lib_int.h"
#ifdef __cplusplus
extern "C"
{
#endif

    BitstreamEncVideo *BitStreamCreateEnc(Int bufferSize);
    Void  BitstreamCloseEnc(BitstreamEncVideo *stream);
    PV_STATUS BitstreamPutBits(BitstreamEncVideo *stream, Int Length, UInt Value);
    PV_STATUS BitstreamPutGT16Bits(BitstreamEncVideo *stream, Int Length, ULong Value);
    PV_STATUS BitstreamSaveWord(BitstreamEncVideo *stream);
    PV_STATUS BitstreamSavePartial(BitstreamEncVideo *stream, Int *fraction);
    Int BitstreamGetPos(BitstreamEncVideo *stream);
    void BitstreamEncReset(BitstreamEncVideo *stream);

    Int BitstreamShortHeaderByteAlignStuffing(BitstreamEncVideo *stream);
    Int BitstreamMpeg4ByteAlignStuffing(BitstreamEncVideo *stream);
    PV_STATUS BitstreamAppendEnc(BitstreamEncVideo *bitstream1, BitstreamEncVideo *bitstream2);
    PV_STATUS BitstreamAppendPacket(BitstreamEncVideo *bitstream1, BitstreamEncVideo *bitstream2);
    PV_STATUS BitstreamAppendPacketNoOffset(BitstreamEncVideo *bitstream1, BitstreamEncVideo *bitstream2);
    PV_STATUS BitstreamRepos(BitstreamEncVideo *bitstream, Int byteCount, Int bitCount);
    PV_STATUS BitstreamFlushBits(BitstreamEncVideo *bitstream1, Int num_bit_left);
    PV_STATUS BitstreamPrependPacket(BitstreamEncVideo *bitstream1, BitstreamEncVideo *bitstream2);


    Void  BitstreamSetOverrunBuffer(BitstreamEncVideo *stream, UChar *overrunBuffer, Int oBSize, VideoEncData *video);
    PV_STATUS BitstreamUseOverrunBuffer(BitstreamEncVideo* stream, Int numExtraBytes);


#ifdef __cplusplus
}
#endif

#endif /* _BITSTREAM_IO_H_ */
