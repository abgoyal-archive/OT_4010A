

#ifndef OMX_WMV_COMPONENT_H_INCLUDED
#define OMX_WMV_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef WMV_DEC_H_INCLUDED
#include "wmv_dec.h"
#endif


#define INPUT_BUFFER_SIZE_WMV 16000
// qcif size - 176*144*3/2
#define OUTPUT_BUFFER_SIZE_WMV 38016

#define NUMBER_INPUT_BUFFER_WMV  10
#define NUMBER_OUTPUT_BUFFER_WMV  8



class OpenmaxWmvAO : public OmxComponentVideo
{
    public:

        OpenmaxWmvAO();
        ~OpenmaxWmvAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

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
        bool getWmvDecodeVideoInTime();
        bool getWmvDecodeVideoOutTime();

	OMX_ERRORTYPE FlushPortFinal(OMX_S32 PortIndex);

    private:
        OMX_BOOL                iUseExtTimestamp;
        WmvDecoder_OMX* ipWmvDecoderObject;

#ifdef MT6573_MFV_HW
		OMX_BOOL bFinalDecDone;
#endif
        bool mIsStageFrightPlayer;
};




#endif // OMX_MPEG4_COMPONENT_H_INCLUDED
