

#ifndef PVMP3_POLYPHASE_FILTER_WINDOW_H
#define PVMP3_POLYPHASE_FILTER_WINDOW_H


#include "pvmp3_audio_type_defs.h"
#include "s_tmp3dec_chan.h"


#define MAX_16BITS_INT  0x7FFF






#ifdef __cplusplus
extern "C"
{
#endif


#if (defined(PV_ARM_V5)||defined(PV_ARM_V4))


    __inline int16 saturate16(int32 sample)
    {
        int32 a;
        int32 b = 31;
        __asm
        {
            mov   a, sample, asr#15
            teq a, sample, asr b
            eorne sample, MAX_16BITS_INT, sample, asr#31
        }
        return sample ;
    }

#else

    inline int16 saturate16(int32 sample)
    {

        if ((sample >> 15) ^(sample >> 31))
        {
            sample = MAX_16BITS_INT ^(sample >> 31);
        }
        return sample;

    }
#endif


    void pvmp3_polyphase_filter_window(int32 *synth_buffer,
                                       int16 *outPcm,
                                       int32 numChannels);


#ifdef __cplusplus
}
#endif

#endif





