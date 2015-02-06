
#ifndef AVC_DEC_H_INCLUDED
#define AVC_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef _AVCDEC_API_H_
#include "avcdec_api.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

//#define AVC_DEC_TIMESTAMP_ARRAY_SIZE 17
#define AVC_DEC_TIMESTAMP_ARRAY_SIZE 19
#if defined(MT6516_AVC_HW_DECODER) || defined(MT6573_MFV_HW)
#define AVC_OUTPUT_BUFFER_NUM 21
#endif
class AVCCleanupObject_OMX
{
        AVCHandle* ipavcHandle;

    public:
        AVCCleanupObject_OMX(AVCHandle* aAvcHandle = NULL)
        {
            ipavcHandle = aAvcHandle;
        }

        //! Use destructor to do all the clean up work
        ~AVCCleanupObject_OMX();
};


class AvcDecoder_OMX
{
#if defined(MT6516_AVC_HW_DECODER) || defined(MT6573_MFV_HW)
	private:
		OMX_ERRORTYPE AvcDecInit_OMX_HW();

		OMX_BOOL AvcDecodeVideo_OMX_HW(OMX_U8* aOutBuffer, OMX_U32* aOutputLength,
			OMX_U8** aInputBuf, OMX_U32* aInBufSize,
			OMX_PARAM_PORTDEFINITIONTYPE* aPortParam,
			OMX_S32* iFrameCount, OMX_BOOL aMarkerFlag,
			OMX_TICKS* aOutTimestamp,
			OMX_BOOL *aResizeFlag,
			OMX_BUFFERHEADERTYPE *ippOutputBuffer);

		OMX_BOOL FlushOutput_OMX_HW(OMX_U8* aOutBuffer, OMX_U32* aOutputLength, OMX_TICKS* aOutTimestamp, OMX_S32 OldWidth, OMX_S32 OldHeight);

		OMX_ERRORTYPE AvcDecDeinit_OMX_HW();

		//static bool g_bUseAVCHWDec;
		//static void *g_bAVCHWDecObj;

		bool bHWDecEnable;
		AVCDec_Status CurStatus;
		struct sched_param ori_sched_p; //original scheduling.

		int32 frame_crop_enable;
		int32 frame_width;
		int32 frame_height;
		int32 frame_real_width;
		int32 frame_real_height;
        uint32 max_dpb_number;
		int32 frame_crop_top;
		int32 frame_crop_bottom;
		int32 frame_crop_left;
		int32 frame_crop_right;
		unsigned char *yuv420_buf;
		int32 iTSIn, iTSOut;
        bool bSPS_PPS_Decoded;

        uint8   *pH264_SliceMapVa; 
        uint8   *pH264_SliceMapPa;
        uint32  u4H264_SliceMapSize;
        bool    bH264_SliceMapAllocated;

        uint8   *pH264_BitstreamVa; 
        uint8   *pH264_BitstreamPa;
        uint32  u4H264_BitstreamSize;
        bool    bH264_BitstreamAllocated;

        uint8   *pH264_FrameBufVa; 
        uint8   *pH264_FrameBufPa;
        uint32  u4H264_FrameBufSize;
        bool    bH264_FrameBufAllocated; 

        /*
        [20100623][Jackal] Add this flag in order to "if we can't alloc PMEM 
        (SLICEMAP_AND_BS_MEM, FRAMEBUFFER_MEM) in AvcDecInit_OMX_HW, 
        we don't call driver init, so we will use SW Decoder. 
        When play done, in AvcDecDeinit_OMX_HW, we will call driver deinit,
        so will cause exception due to memory invalid access, so we use this 
        flag to avoid this condition"
        */
        bool    bH264_DriverInited;

        bool    bH264_EOS;
        bool    bH264_DumpYUVBuf;

        //bool mIsStageFrightPlayer;
#endif
		void    *g_bAVCHWDecObj;
        bool    bSpsPpsDecodedSw;
        unsigned int g_FrameCount;

    public:
        AvcDecoder_OMX()
        {
            CurrInputTimestamp = 0;
            pDpbBuffer = NULL;
            FrameSize = 0;
            iAvcActiveFlag = OMX_FALSE;
            g_FrameCount = 0;
            oscl_memset(DisplayTimestampArray, 0, sizeof(OMX_TICKS)*AVC_DEC_TIMESTAMP_ARRAY_SIZE);
#if defined(MT6516_AVC_HW_DECODER) || defined(MT6573_MFV_HW)
			bHWDecEnable = 0;
			CurStatus = AVCDEC_FAIL;
			yuv420_buf = NULL;
			iTSIn = 0;
			iTSOut = 0;
            bSPS_PPS_Decoded = false;

            pH264_SliceMapVa = NULL; 
            pH264_SliceMapPa = NULL;
            u4H264_SliceMapSize = 0;
            bH264_SliceMapAllocated = false;

            pH264_BitstreamVa = NULL; 
            pH264_BitstreamPa = NULL;
            u4H264_BitstreamSize = 0;
            bH264_BitstreamAllocated = false;

            pH264_FrameBufVa = NULL; 
            pH264_FrameBufPa = NULL;
            u4H264_FrameBufSize = 0;
            bH264_FrameBufAllocated = false;

            bH264_DriverInited = false;
            bH264_EOS = false;
            g_bAVCHWDecObj = NULL;
            mColorFormat = OMX_COLOR_FormatYUV420Planar;
#endif
        };

        ~AvcDecoder_OMX() {		
#if defined(MT6516_AVC_HW_DECODER) || defined(MT6573_MFV_HW)
			if(NULL != yuv420_buf)
			{
				free(yuv420_buf);
			} 
#endif
		};

        AVCCleanupObject_OMX*   pCleanObject;
        AVCHandle       AvcHandle;
        AVCDecSPSInfo   SeqInfo;
        uint32          FrameSize;
        uint8*          pDpbBuffer;
        OMX_TICKS       DisplayTimestampArray[AVC_DEC_TIMESTAMP_ARRAY_SIZE];
        OMX_TICKS       CurrInputTimestamp;
        OMX_U32         InputBytesConsumed;
        OMX_BOOL        iAvcActiveFlag;
        OMX_U32         hDrvHandle;
        OMX_U32         hHWDrvHandle;
        OMX_U32         hSWDrvHandle;
        OMX_COLOR_FORMATTYPE mColorFormat;


        OMX_ERRORTYPE AvcDecInit_OMX();

        OMX_BOOL AvcDecodeVideo_OMX(OMX_U8* aOutBuffer, OMX_U32* aOutputLength,
                                    OMX_U8** aInputBuf, OMX_U32* aInBufSize,
                                    OMX_PARAM_PORTDEFINITIONTYPE* aPortParam,
                                    OMX_S32* iFrameCount, OMX_BOOL aMarkerFlag,
                                    OMX_TICKS* aOutTimestamp,
                                    OMX_BOOL *aResizeFlag,
                                    OMX_BUFFERHEADERTYPE *ippOutputBuffer);

        OMX_ERRORTYPE AvcDecDeinit_OMX();

        OMX_BOOL InitializeVideoDecode_OMX();

        OMX_BOOL FlushOutput_OMX(OMX_U8* aOutBuffer, OMX_U32* aOutputLength, OMX_TICKS* aOutTimestamp, OMX_S32 OldWidth, OMX_S32 OldHeight);

        AVCDec_Status GetNextFullNAL_OMX(uint8** aNalBuffer, int32* aNalSize, OMX_U8* aInputBuf, OMX_U32* aInBufSize);

        static int32 AllocateBuffer_OMX(void* aUserData, int32 i, uint8** aYuvBuffer);

        static int32 ActivateSPS_OMX(void* aUserData, uint aSizeInMbs, uint aNumBuffers);

        int32 NSAllocateBuffer_OMX(void* aUserData, int32 i, uint8** aYuvBuffer);

        int32 NSActivateSPS_OMX(void* aUserData, uint aSizeInMbs, uint aNumBuffers);

        void ResetDecoder(); // for repositioning

		OMX_BOOL HwSupported();

        OMX_BOOL SPS_PPS_Decoded(bool aLastCodeConfigFlag);
#if defined(MT6573_MFV_HW)
        public:
        OMX_BOOL ShowYUVBuffer();
        void* InsertYUVBuffer(OMX_U32 u4VA,OMX_U32 u4PA, OMX_U32 u4Size, OMX_BUFFERHEADERTYPE *ipOutputBuffer);
        OMX_S32 ReleaseYUVBuffer(OMX_U32 u4VA,OMX_U32 u4PA);
        OMX_BOOL CheckYUVBuffer(OMX_U32 u4VA,OMX_U32 u4PA);
        OMX_U32 CheckFreeBuffer();
        OMX_BUFFERHEADERTYPE *GetDisplayBuffer();
        void *GetFreeBuffer(int &i);
        OMX_BOOL CheckOutputBufStatus_OMX_HW(OMX_BUFFERHEADERTYPE* pOutputBuffer);
        OMX_BOOL ShowOutputBufStatus_OMX_HW();
        void InsertionSortForInputPTS(int32 TSin, OMX_TICKS timeTS);
        void ResetTSArray();
        OMX_BUFFERHEADERTYPE *GetOMXBufferHeader(OMX_U32 u4VA);
        OMX_BOOL AvcDecodeVideoFinal(OMX_BUFFERHEADERTYPE *ipOutputBuffer, OMX_U32 nBufferSize);
        typedef struct _FrmBufStruct
		{
			OMX_BOOL	bUsed;
            OMX_BOOL    bDisplayed;
            void        *pOutputBuffer;
            OMX_BUFFERHEADERTYPE *ipOutputBuffer;
		} FrmBufStruct;
        FrmBufStruct frm_buf[AVC_OUTPUT_BUFFER_NUM];
        OMX_BOOL bBufferProcessDone;
#endif

};

typedef class AvcDecoder_OMX AvcDecoder_OMX;

#endif  //#ifndef AVC_DEC_H_INCLUDED

