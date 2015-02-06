

#ifndef OMX_MPEG4ENC_COMPONENT_H_INCLUDED
#define OMX_MPEG4ENC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef MPEG4_ENC_H_INCLUDED
#include "mpeg4_enc.h"
#endif


#define INPUT_BUFFER_SIZE_MP4ENC 38016          //(176 * 144 * 1.5) for YUV 420 format.
#define OUTPUT_BUFFER_SIZE_MP4ENC (128*1024) //32768 //16384 //8192


#define NUMBER_INPUT_BUFFER_MP4ENC  6
#define NUMBER_OUTPUT_BUFFER_MP4ENC  10 // 2



class OmxComponentMpeg4EncAO : public OmxComponentVideo
{
    public:

        typedef struct {
            unsigned long u4Sec;
            unsigned long u4USec;
            unsigned long u4TimeMark;
        } stLOGPAGE;
        typedef struct {
            unsigned long u4LogDepth;
            unsigned long u4Counter;
            unsigned long u4Counter_end;
            unsigned long u4MaxTimeMark;
            stLOGPAGE * pPage;
        } stLOG;

        typedef enum {
            ALLOC_TIMEPROFILEBUFF = 0,
            FREE_TIMEPROFILEBUFF,
            PUT_TIMEPROFILE_START,
            PUT_TIMEPROFILE_END,
            PRINT_TIMEPROFILE,
            RESET_TIMEPROFILE,
        } LOG_CMD;

        unsigned long mhalTimeLog(LOG_CMD a_CMD , unsigned long a_u4Val);
       
         bool getTimeStart();
         bool getTimeEnd();
         bool setGetTimeEnd(bool a_bFlag);
         bool getInputBufTimeDelay(OMX_BUFFERHEADERTYPE* a_prBufHdr);

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

        OmxComponentMpeg4EncAO();
        ~OmxComponentMpeg4EncAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void SetEncoderMode(OMX_S32 aMode);
        void ProcessInBufferFlag();

        void ProcessData();

        OMX_ERRORTYPE SetConfig(
            OMX_IN  OMX_HANDLETYPE hComponent,
            OMX_IN  OMX_INDEXTYPE nIndex,
            OMX_IN  OMX_PTR pComponentConfigStructure);

    private:
OMX_U32 m_start_tickcount;
OMX_U32 m_end_tickcount;
bool m_bGetTimeEnd;

OMX_U32 start_time_sec, start_time_usec, end_time_sec, end_time_usec;
bool time_prof, time_prof_stop;

        OMX_BOOL CopyDataToOutputBuffer();

        Mpeg4Encoder_OMX* ipMpegEncoderObject;
        OMX_S32           iEncMode;

        OMX_BOOL          iBufferOverRun;
        OMX_U8*           ipInternalOutBuffer;
        OMX_U32           iInternalOutBufFilledLen;
        OMX_TICKS         iOutputTimeStamp;
        OMX_BOOL          iSyncFlag;
};

#endif // OMX_MPEG4ENC_COMPONENT_H_INCLUDED
