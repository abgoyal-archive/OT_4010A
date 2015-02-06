
#ifndef AAC_TIMESTAMP_H_INCLUDED
#define AAC_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif




#define DEFAULT_SAMPLING_FREQ 44100
#define DEFAULT_SAMPLES_PER_FRAME 1024

class AacTimeStampCalc
{
    public:

        AacTimeStampCalc()
        {
            iSamplingFreq = DEFAULT_SAMPLING_FREQ;
            iCurrentTs = 0;
            iCurrentSamples = 0;
            iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME;
        };

        void SetParameters(uint32 aFreq, uint32 aSamples);

        void SetFromInputTimestamp(OMX_TICKS aValue);

        void UpdateTimestamp(uint32 aValue);

        OMX_TICKS GetConvertedTs();

        OMX_TICKS GetCurrentTimestamp();

        OMX_TICKS GetFrameDuration();


    private:
        uint32 iSamplingFreq;
        OMX_TICKS iCurrentTs;
        uint32 iCurrentSamples;
        uint32 iSamplesPerFrame;
};

#endif  //#ifndef AAC_TIMESTAMP_H_INCLUDED
