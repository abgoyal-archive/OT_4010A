

#ifndef S_TMP3DEC_FILE_H
#define S_TMP3DEC_FILE_H

#include "s_tmp3dec_chan.h"
#include "s_mp3bits.h"
#include "s_huffcodetab.h"






#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        int32           num_channels;
        int32           predicted_frame_size;
        int32           frame_start;
        int32           Scratch_mem[168];
        tmp3dec_chan    perChan[CHAN];
        mp3ScaleFactors scaleFactors[CHAN];
        mp3SideInfo     sideInfo;
        tmp3Bits        mainDataStream;
        uint8           mainDataBuffer[BUFSIZE];
        tmp3Bits        inputStream;
        huffcodetab     ht[HUFF_TBL];
    } tmp3dec_file;


#ifdef __cplusplus
}
#endif



#endif




