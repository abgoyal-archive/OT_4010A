
#ifndef WMV_DEC_H_INCLUDED
#define WMV_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#define PV_TRUE 1
#define PV_FALSE 0


#ifdef MT6573_MFV_HW
#include "val_types.h"
#include "vdec_drv_if.h"
#define MFV_OUTPUT_BUFFER_NUM 8 //same as "NUMBER_OUTPUT_BUFFER_MP4"
#define EMI_ACTIVITY 0
#endif

class WmvDecoder_OMX
{
    public:

#ifdef MT6573_MFV_HW
		OMX_S32 InitializeVideoDecodeHW(OMX_S32* aWidth, OMX_S32* aHeight, OMX_U8** aBuffer, OMX_S32* aSize);
		OMX_BOOL WmvDecodeVideoFinal();
		OMX_BUFFERHEADERTYPE* GetDisplayBuffer();
		OMX_BUFFERHEADERTYPE* GetFreeBuffer();
		OMX_BOOL isFreeBuffer(OMX_BUFFERHEADERTYPE* ipOutputBuffer);

		OMX_U32 CheckFreeBuffer();
		OMX_BOOL bBufferProcessDone;
#endif
		

        WmvDecoder_OMX();

        OMX_S32 InitializeVideoDecode(OMX_S32* aWidth, OMX_S32* aHeight, OMX_U8** aBuffer, OMX_S32* aSize);

        OMX_ERRORTYPE WmvDecInit();

        OMX_BOOL WmvDecodeVideo(OMX_U8* aOutBuffer, OMX_U32* aOutputLength,
                                OMX_U8** aInputBuf, OMX_U32* aInBufSize,
                                OMX_PARAM_PORTDEFINITIONTYPE* aPortParam,
                                OMX_S32* aFrameCount, OMX_BOOL aMarkerFlag, OMX_BOOL *aResizeFlag
                                , OMX_PARAM_PORTDEFINITIONTYPE* aInPortParam, OMX_TICKS iTimestamp, OMX_BUFFERHEADERTYPE*   ipOutputBuffer);

        OMX_ERRORTYPE WmvDecDeinit();

        OMX_S32 GetVideoHeader(int32 aLayer, uint8 *aBuf, int32 aMaxSize);

        OMX_S32 WmvInitFlag;

        OMX_BOOL WmvInitCompleteFlag;
                
        OMX_BOOL WMVDecHwSupported();

        OMX_BOOL WMVHWAllocBitstreamMem(OMX_U8** aInputBuf, OMX_U32 aInputSize);

        OMX_BOOL WMVHWAllocMTKYUVBufMem();

        OMX_BOOL WMVHWDecodeVideo(OMX_U32 aInputSize, OMX_U8* aOutBuffer, OMX_S32* aFrameCount);

        OMX_BOOL WMVHWPostProcessing(OMX_U8* aOutBuffer, OMX_U32* aOutputLength, OMX_S32* aFrameCount);

        OMX_BOOL WMVHWLockResource();

        OMX_BOOL WMVHWFreeBitstreamMem();

        OMX_BOOL WMVHWFreeMTKYUVBufMem();

        OMX_BOOL WMVHWUnLockResource();

        OMX_BOOL WMVHWGetRefFrameBufPA(unsigned int* aRefFrameBufPA);
        
    private:

        OMX_U8* pFrame0, *pFrame1;
        OMX_S32 iDisplay_Width, iDisplay_Height;
        OMX_S32 iShortVideoHeader;

        OMX_U8 * pWMV_BitstreamVa; 
        OMX_U8 * pWMV_BitstreamPa;
        OMX_BOOL bWMV_BitstreamAllocated;

        OMX_U8 * pWMV_CurMTKYUVBufVa; 
        OMX_U8 * pWMV_CurMTKYUVBufPa;
        OMX_BOOL bWMV_CurMTKYUVBufAllocated;
        
        OMX_U8 * pWMV_RefMTKYUVBufVa; 
        OMX_U8 * pWMV_RefMTKYUVBufPa;
        OMX_BOOL bWMV_RefMTKYUVBufAllocated;

        OMX_BOOL bWMV_VopCodedFlag;

        static OMX_S32 iWMV_LockTID;

        OMX_TICKS iFrameTsInterval;
        OMX_TICKS iCurrentFrameTs;
        OMX_BOOL  bFirstFrameRetrieved;
        OMX_BOOL  bResetFirstFrameTs;
        OMX_BOOL  bCorrectTsFromOMX;

#ifdef MT6573_MFV_HW
// Morris Yang 20101227 [
	 bool mIsStageFrightPlayer;
        bool mPlayerInitialized;
// ]        
#if EMI_ACTIVITY        
		OMX_BOOL bWMV_EmiActivityFlag;
#endif

		VDEC_DRV_FRAMEBUF_T* GetFrmBuf(OMX_TICKS iTimestamp, OMX_BUFFERHEADERTYPE*	ipOutputBuffer);
		OMX_BOOL PutFrmBuf(OMX_BUFFERHEADERTYPE*	 ipOutputBuffer);


		typedef struct _FrmBufStruct
		{
			OMX_BOOL				bUsed;
			VDEC_DRV_FRAMEBUF_T 	frame_buffer;
			OMX_TICKS 				iTimestamp;
			OMX_BUFFERHEADERTYPE	*ipOutputBuffer;
		} FrmBufStruct;
		
        VAL_UINT32_T u4BSVa; 
        VAL_UINT32_T u4BSPa;

		VAL_HANDLE_T        rDrvHandle;
		OMX_S32 			codec_mode;
		VDEC_DRV_RINGBUF_T  ringbuf;
		VDEC_DRV_SEQINFO_T  rSeqInfo;
		VDEC_DRV_PROPERTY_T rDrvProperty;		
		FrmBufStruct frm_buf[MFV_OUTPUT_BUFFER_NUM];
		OMX_BOOL bHWEnable;
		bool fgUseExtRefBuf;
		VDEC_DRV_SET_BUFFER_ADDR_T  rDrvExtRefBufAddr;
		OMX_U32 BITSTREAM_BUFFER_SIZE;
#endif

};


#endif ///#ifndef WMV_DEC_H_INCLUDED
