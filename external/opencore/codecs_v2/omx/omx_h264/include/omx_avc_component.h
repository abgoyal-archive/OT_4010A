

#ifndef OMX_AVC_COMPONENT_H_INCLUDED
#define OMX_AVC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef AVC_DEC_H_INCLUDED
#include "avc_dec.h"
#endif
//#define DumpInput__
#ifdef DumpInput__
#include <stdio.h>
#endif


#if (defined(TEST_FULL_AVC_FRAME_MODE) || defined(TEST_FULL_AVC_FRAME_MODE_SC))
#define INPUT_BUFFER_SIZE_AVC (2000 * MAX_NAL_PER_FRAME)
#else
#define INPUT_BUFFER_SIZE_AVC (2000)
#undef MAX_NAL_PER_FRAME
#define MAX_NAL_PER_FRAME 1 /* this component doesn't need to suppoart more than 1 NAL per frame in this mode, so overwrite default */
#endif

#define NAL_START_CODE_SIZE 4

//qcif - output 176*144*3/2
#define OUTPUT_BUFFER_SIZE_AVC 38016

// Honda: 
// ALPS00005757 needs 65 NAL in config info in format specific data memfragment (SPS, PPS)
// ALPS00006185 needs 105.
#define NUMBER_INPUT_BUFFER_AVC  10//120 //10
#define NUMBER_OUTPUT_BUFFER_AVC  8 //10 //2
#ifdef AVC_USE_EXT_BUF
#undef NUMBER_OUTPUT_BUFFER_AVC
#define NUMBER_OUTPUT_BUFFER_AVC 21
#endif


typedef struct __OMX_BUFFER_LIST_T
{
    OMX_BUFFERHEADERTYPE *ipOutBuf;
    OMX_BOOL             bReleased;
} OMX_BUFFER_LIST_T;


class OpenmaxAvcAO : public OmxComponentVideo
{
    public:

        OpenmaxAvcAO();
        ~OpenmaxAvcAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void ComponentBufferMgmtWithoutMarker();
        OMX_BOOL ParseFullAVCFramesIntoNALs(OMX_BUFFERHEADERTYPE* aInputBuffer);
        void ProcessData();
        void DecodeWithoutMarker();
        void DecodeWithMarker();
        void ResetComponent();
        OMX_ERRORTYPE GetNewOutputBuffer();
        OMX_ERRORTYPE FlushPortFinal(OMX_S32 PortIndex);
        OMX_ERRORTYPE GetConfig(
            OMX_IN  OMX_HANDLETYPE hComponent,
            OMX_IN  OMX_INDEXTYPE nIndex,
            OMX_INOUT OMX_PTR pComponentConfigStructure);

		bool getTimeStart();
		bool getTimeEnd();
		bool getInputQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getInputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getInputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getInputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputQueueStartTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputQueueEndTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputDeQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputToQueueTime(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputToQueueTime0(OMX_BUFFERHEADERTYPE *a_prBufHdr);         
		bool getOutputToQueueTime1(OMX_BUFFERHEADERTYPE *a_prBufHdr);
		bool getOutputToQueueTime2(OMX_BUFFERHEADERTYPE *a_prBufHdr); 

    private:
#ifdef DumpInput__
        FILE *pf_out;
#endif
OMX_U32 m_start_tickcount;
OMX_U32 m_end_tickcount;
bool m_bGetTimeEnd;
//bool mIsStageFrightPlayer;

        AvcDecoder_OMX* ipAvcDec;
        OMX_BOOL                iDecodeReturn;

		OMX_U32 iHwSupportedChecked;

        // variables for "frame" mode i.e. iOMXComponentNeedsFullAVCFrames is turned on
        OMX_U32 iNALSizeArray[MAX_NAL_PER_FRAME]; // 100 should be more than enough NALs per frame
        OMX_U32 iCurrNAL;
        OMX_U32 iNumNALs;
        OMX_U32 iNALOffset;
        //mtk80366 add for alps00122 start->
        OMX_U32 iCurrNALType;
        OMX_U32 iLastNALType;
        OMX_BOOL iHeader_reconfg;
        //mtk80366 add for alps00122 end<-
        OMX_BOOL iHasIFrame;
#ifdef AVC_USE_EXT_BUF
        void InsertOMXBuffer(OMX_BUFFERHEADERTYPE *ipOutputBuf);
        OMX_BOOL CheckOMXBufferOutput(OMX_U32 nPos, OMX_BUFFERHEADERTYPE *ipOutputBuf, OMX_BOOL bRelease);
        OMX_U32 CheckOMXBufferOutput();
        OMX_BUFFER_LIST_T OutBuffer[NUMBER_OUTPUT_BUFFER_AVC];
#endif
};



#endif // OMX_AVC_COMPONENT_H_INCLUDED
