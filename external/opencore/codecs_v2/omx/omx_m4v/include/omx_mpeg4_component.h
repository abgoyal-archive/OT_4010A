

#ifndef OMX_MPEG4_COMPONENT_H_INCLUDED
#define OMX_MPEG4_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef MPEG4_DEC_H_INCLUDED
#include "mpeg4_dec.h"
#endif

#ifdef MT6516_MP4_HW_DECODER_V2
#define TRICKY_DISABLE_MEMCPY
#endif

#define INPUT_BUFFER_SIZE_MP4 16000
// qcif size - 176*144*3/2
#define OUTPUT_BUFFER_SIZE_MP4 38016

#define NUMBER_INPUT_BUFFER_MP4  10
#define NUMBER_OUTPUT_BUFFER_MP4  8

#define MINIMUM_H263_SHORT_HEADER_SIZE 12
enum
{
    MODE_H263 = 0,
    MODE_MPEG4,
    MODE_S263,
    MODE_DIVX,
    MODE_XVID
};


class OpenmaxMpeg4AO : public OmxComponentVideo
{
    public:

        OpenmaxMpeg4AO();
        ~OpenmaxMpeg4AO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void SetDecoderMode(int);
        void ComponentBufferMgmtWithoutMarker();
        void ProcessData();
        void DecodeWithoutMarker();
        void DecodeWithMarker();

        OMX_ERRORTYPE GetConfig(
            OMX_IN  OMX_HANDLETYPE hComponent,
            OMX_IN  OMX_INDEXTYPE nIndex,
            OMX_INOUT OMX_PTR pComponentConfigStructure);

        OMX_ERRORTYPE ReAllocatePartialAssemblyBuffers(OMX_BUFFERHEADERTYPE* aInputBufferHdr);

        bool getInputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getInputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getOutputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getOutputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);        
        bool getInputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getOutputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getReturnInputBufferTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getReturnOutputBufferTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
        bool getMp4DecodeVideoInTime();
        bool getMp4DecodeVideoOutTime();

	OMX_ERRORTYPE FlushPortFinal(OMX_S32 PortIndex);

    private:

        OMX_BOOL DecodeH263Header(OMX_U8* aInputBuffer, OMX_U32* aBufferSize);

        void ReadBits(OMX_U8* aStream, uint8 aNumBits, uint32* aOutData);

        OMX_BOOL                iUseExtTimestamp;
        Mpeg4Decoder_OMX* ipMpegDecoderObject;
        OMX_S32 iDecMode;

        //Parameters required for H.263 source format parsing
        OMX_U32 iH263DataBitPos;
        OMX_U32 iH263BitPos;
        OMX_U32 iH263BitBuf;
#ifdef MT6573_MFV_HW	
		OMX_BOOL bFinalDecDone;
#endif
      //  bool mIsStageFrightPlayer;
};




#endif // OMX_MPEG4_COMPONENT_H_INCLUDED
