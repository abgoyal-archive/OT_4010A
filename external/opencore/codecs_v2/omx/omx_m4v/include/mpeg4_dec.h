
#ifndef MPEG4_DEC_H_INCLUDED
#define MPEG4_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef _MP4DEC_API_H_
#include "mp4dec_api.h"
#endif

#ifndef _MP4DECLIB_H_
#include "mp4dec_lib.h"
#endif

#ifdef MT6516_MP4_HW_DECODER_V2
#include "MT6516_MPEG4_Decoder.h"
#endif  //#ifdef MT6516_MP4_HW_DECODER_V2

#ifdef MT6573_MFV_HW
#include "val_types.h"
#include "vdec_drv_if.h"
#define MFV_OUTPUT_BUFFER_NUM 8 //same as "NUMBER_OUTPUT_BUFFER_MP4"
#define EMI_ACTIVITY 0
#endif

class Mpeg4Decoder_OMX
{
    public:

#ifdef MT6573_MFV_HW
		OMX_S32 InitializeVideoDecodeHW(
			OMX_S32* aWidth, OMX_S32* aHeight, OMX_U8** aBuffer, OMX_S32* aSize, OMX_S32 mode);
		OMX_BOOL Mp4DecodeVideoFinal();
		OMX_BUFFERHEADERTYPE* GetDisplayBuffer();
		OMX_BUFFERHEADERTYPE* GetFreeBuffer();
		OMX_BOOL isFreeBuffer(OMX_BUFFERHEADERTYPE* ipOutputBuffer);

		OMX_U32 CheckFreeBuffer();
		OMX_BOOL bBufferProcessDone;
#endif
		

        Mpeg4Decoder_OMX();

        OMX_S32 InitializeVideoDecode(OMX_S32* aWidth, OMX_S32* aHeight,
                                      OMX_U8** aBuffer, OMX_S32* aSize, OMX_S32 mode);

        OMX_ERRORTYPE Mp4DecInit();

        OMX_BOOL Mp4DecodeVideo(OMX_U8* aOutBuffer, OMX_U32* aOutputLength,
                                OMX_U8** aInputBuf, OMX_U32* aInBufSize,
                                OMX_PARAM_PORTDEFINITIONTYPE* aPortParam,
                                OMX_S32* aFrameCount, OMX_BOOL aMarkerFlag, OMX_BOOL *aResizeFlag
                                , OMX_PARAM_PORTDEFINITIONTYPE* aInPortParam, OMX_TICKS iTimestamp, OMX_BUFFERHEADERTYPE*   ipOutputBuffer);

        OMX_ERRORTYPE Mp4DecDeinit();

        OMX_S32 GetVideoHeader(int32 aLayer, uint8 *aBuf, int32 aMaxSize);

        OMX_S32 Mpeg4InitFlag;

        OMX_BOOL Mpeg4InitCompleteFlag;
        
        OMX_BOOL Mpeg4InitDecFailFlag;
        
        OMX_BOOL Mpeg4InitedSwDecoder;
        
        OMX_BOOL MP4DecHwSupported();

        OMX_BOOL MP4HWParseM4VHeader(OMX_U8** aInputBuf, OMX_U32* aInBufSize);

        OMX_BOOL MP4HWParseH263Header(OMX_U8** aInputBuf);

        OMX_BOOL MP4HWAllocBitstreamMem(OMX_U8** aInputBuf, OMX_U32 aInputSize);

        OMX_BOOL MP4HWAllocMTKYUVBufMem();

        OMX_BOOL MP4HWDecodeVideo(OMX_U32 aInputSize, OMX_U8* aOutBuffer, OMX_S32* aFrameCount);

        OMX_BOOL MP4HWPostProcessing(OMX_U8* aOutBuffer, OMX_U32* aOutputLength, OMX_S32* aFrameCount);

        OMX_BOOL MP4HWLockResource();

        OMX_BOOL MP4HWFreeBitstreamMem();

        OMX_BOOL MP4HWFreeMTKYUVBufMem();

        OMX_BOOL MP4HWUnLockResource();

        OMX_BOOL MP4HWGetRefFrameBufPA(unsigned int* aRefFrameBufPA);
        //add by qian
        OMX_S32 MP4InitSwDecoder(OMX_S32* aWidth, OMX_S32* aHeight,
                                      OMX_U8** aBuffer, OMX_S32* aSize, OMX_S32 mode);
        
    private:
        MP4DecodingMode CodecMode;
        VideoDecControls VideoCtrl;

        OMX_U8* pFrame0, *pFrame1;
        OMX_S32 iDisplay_Width, iDisplay_Height;
        OMX_S32 iShortVideoHeader;

        OMX_U8 VO_START_CODE1[4];
        OMX_U8 VOSH_START_CODE1[4];
        OMX_U8 VOP_START_CODE1[4];
        OMX_U8 H263_START_CODE1[3];

        OMX_U8 * pMP4_BitstreamVa; 
        OMX_U8 * pMP4_BitstreamPa;
        OMX_BOOL bMP4_BitstreamAllocated;

        OMX_U8 * pMP4_CurMTKYUVBufVa; 
        OMX_U8 * pMP4_CurMTKYUVBufPa;
        OMX_BOOL bMP4_CurMTKYUVBufAllocated;
        
        OMX_U8 * pMP4_RefMTKYUVBufVa; 
        OMX_U8 * pMP4_RefMTKYUVBufPa;
        OMX_BOOL bMP4_RefMTKYUVBufAllocated;

        OMX_BOOL bMP4_VopCodedFlag;

        static OMX_S32 iMP4_LockTID;

#ifdef MT6516_MP4_HW_DECODER_V2
        pmem_bitstream MP4_Bitstream;
        pmem_MTKYUVBuffer MP4_MTKYUVBuffer;
       // bool mIsStageFrightPlayer;
        bool mPlayerInitialized;
#endif  //#ifdef MT6516_MP4_HW_DECODER_V2

#ifdef MT6573_MFV_HW

#if EMI_ACTIVITY        
		OMX_BOOL bMP4_EmiActivityFlag;
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
		
        //VAL_UINT32_T u4BSVa; 
        //VAL_UINT32_T u4BSPa;

        OMX_U8 * u4BSVa; 
        OMX_U8 * u4BSPa;

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


#endif ///#ifndef MPEG4_DEC_H_INCLUDED
