
#ifndef PVAVCDECODER_H_INCLUDED
#define PVAVCDECODER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PVAVCDECODERINTERFACE_H_INCLUDED
#include "pvavcdecoderinterface.h"
#endif

// AVC video decoder
class PVAVCDecoder : public PVAVCDecoderInterface
{
    public:
        virtual ~PVAVCDecoder();
        static  PVAVCDecoder* New(void);
        virtual bool    InitAVCDecoder(FunctionType_SPS, FunctionType_Alloc, FunctionType_Unbind,
                                       FunctionType_Malloc, FunctionType_Free, void *);
        virtual void    CleanUpAVCDecoder(void);
        virtual void    ResetAVCDecoder(void);
        virtual int32   DecodeSPS(uint8 *bitstream, int32 buffer_size);
        virtual int32   DecodePPS(uint8 *bitstream, int32 buffer_size);
        virtual int32   DecodeAVCSlice(uint8 *bitstream, int32 *buffer_size);
        virtual bool    GetDecOutput(int *indx, int *release);
        virtual void    GetVideoDimensions(int32 *width, int32 *height, int32 *top, int32 *left, int32 *bottom, int32 *right);
        int     AVC_Malloc(int32 size, int attribute);
        void    AVC_Free(int mem);

    private:
        PVAVCDecoder();
        bool Construct(void);
        void *iAVCHandle;
};

#endif
