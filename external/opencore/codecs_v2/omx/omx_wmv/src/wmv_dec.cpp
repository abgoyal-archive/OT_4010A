
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#include "wmv_dec.h"
#include "oscl_mem.h"
#include "omx_wmv_component.h"

#ifdef MT6573_MFV_HW
#include "oscl_pmem.h"
//#include "pmem.h"
#include "val_types.h"
#include "vdec_drv_if.h"

// Morris Yang 20101227 [
#include "../../../../../../frameworks/base/media/libstagefright/pmem_util/pmem_util.h"
#include <cutils/properties.h>
// ]

#endif

#ifdef _WIN32
#define LOGE(x)
#define LOGD(x)
#else
#include <utils/Log.h>
#undef LOG_TAG
#define LOG_TAG "WMV_DEC"
#endif

#define MAX_LAYERS 1

#if 0
#define PVH263DEFAULTHEIGHT 288
#define PVH263DEFAULTWIDTH 352
#else
// [Jackal] Support 640*480 H263 CamCorder 
#define PVH263DEFAULTHEIGHT 1152
#define PVH263DEFAULTWIDTH 1408
#endif

#ifdef MT6573_MFV_HW
#define _512K    (512*1024)
#define _1024K   (1024*1024)

#define WMV_DEC_TRYALLOCMEMCOUNT    200


#define PMEM_ALLOC(va,pa,size) \
do { \
    va = (VAL_UINT32_T)oscl_pmem_alloc(size); \
    if (va == 0) LOGE("[WMV_DEC_PMEM] Alloc va = NULL!! TID = %d", gettid()); \
    pa = (VAL_UINT32_T)oscl_pmem_vir2phy((void*)va); \
	if(false == oscl_pmem_map_into_noncached((void*)va)) LOGE("[WMV_DEC_PMEM] map to non-cache fail, va = 0x%08x, pa = 0x%08x, size = %u, TID = %d", (VAL_UINT32_T)va, (VAL_UINT32_T)pa, (unsigned)size, gettid()); \
    LOGD("[WMV_DEC_PMEM] Alloc va = 0x%08x, pa = 0x%08x, size = %u, TID = %d", (VAL_UINT32_T)va, (VAL_UINT32_T)pa, (unsigned)size, gettid()); \
} while (0)

#define PMEM_FREE(va) \
do { \
    oscl_pmem_free((void*)va); \
    LOGD("[WMV_DEC_PMEM] Free va = 0x%08x, TID = %d", (VAL_UINT32_T)va, gettid()); \
} while (0)

// Morris Yang 20101227 [
#define STAGEFRIGHT_PMEM_ALLOC(va,pa,size) \
do { \
    va = (VAL_UINT32_T)sf_pmem_alloc(size); \
    if (va == 0) LOGE("[WMV_DEC_PMEM] Alloc va = NULL!! TID = %d", gettid()); \
    pa = (VAL_UINT32_T)sf_pmem_vir2phy((void*)va, 13); \
    if(false == oscl_pmem_map_into_noncached((void*)va)) LOGE("[WMV_DEC_PMEM] map to non-cache fail, va = 0x%08x, pa = 0x%08x, size = %u, TID = %d", (VAL_UINT32_T)va, (VAL_UINT32_T)pa, (unsigned)size, gettid()); \
    LOGD("[WMV_DEC_PMEM] Alloc va = 0x%08x, pa = 0x%08x, size = %lu, TID = %d", (VAL_UINT32_T)va, (VAL_UINT32_T)pa, size, gettid()); \
} while (0)

#define STAGEFRIGHT_PMEM_FREE(va) \
do { \
    sf_pmem_free((void*)va); \
    LOGD("[WMV_DEC_PMEM] Free va = 0x%08x, TID = %d", (VAL_UINT32_T)va, gettid()); \
} while (0)
// ]


static pthread_mutex_t WMVDECMutex = PTHREAD_MUTEX_INITIALIZER;

#endif  //#ifdef MT6573_MFV_HW



// from m4v_config_parser.h
//OSCL_IMPORT_REF int16 iGetM4VConfigInfo(uint8 *buffer, int32 length, int32 *width, int32 *height, int32 *, int32 *);


WmvDecoder_OMX::WmvDecoder_OMX()
{
    pFrame0 = NULL;
    pFrame1 = NULL;

    iDisplay_Width = 0;
    iDisplay_Height = 0;


    pWMV_BitstreamVa = NULL; 
    pWMV_BitstreamPa = NULL;
    bWMV_BitstreamAllocated = OMX_FALSE;

    pWMV_CurMTKYUVBufVa = NULL; 
    pWMV_CurMTKYUVBufPa = NULL;
    bWMV_CurMTKYUVBufAllocated = OMX_FALSE;
        
    pWMV_RefMTKYUVBufVa = NULL; 
    pWMV_RefMTKYUVBufPa = NULL;
    bWMV_RefMTKYUVBufAllocated = OMX_FALSE;
    
    bWMV_VopCodedFlag = OMX_TRUE;
}

#ifdef MT6573_MFV_HW
OMX_BUFFERHEADERTYPE* WmvDecoder_OMX::GetDisplayBuffer()
{

	VAL_UINT32_T tmp_frame_addr, i, cnt;

	if( VDEC_DRV_MRESULT_OK != eVDecDrvGetParam(rDrvHandle, VDEC_DRV_GET_TYPE_DISP_FRAME_BUFFER, NULL, &tmp_frame_addr) )
	{
		LOGE("Error!! Cannot get param: VDEC_DRV_GET_TYPE_DISP_FRAME_BUFFER\n");
		return NULL;
	}
	/*
	for(i = 0, cnt=0; i < MFV_OUTPUT_BUFFER_NUM; i++)
	{
		if(OMX_TRUE == frm_buf[i].bUsed)
		{
		cnt++;
		}
	}
	LOGD("DISP CNT:%d", cnt);
	*/
	if(VAL_NULL != tmp_frame_addr)
	{		
		for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
		{
			if(OMX_TRUE == frm_buf[i].bUsed)
			{
				if(tmp_frame_addr == (VAL_UINT32_T)&frm_buf[i].frame_buffer)
				{					
					//LOGD(" VC1 GET_DISP, i:%d, frm_buf:%08x, omx_buf:%08x, nTimeStamp=%lld", i, tmp_frame_addr, (unsigned int)frm_buf[i].ipOutputBuffer, frm_buf[i].ipOutputBuffer->nTimeStamp);
					// modify display frame  timestamp
					if (bCorrectTsFromOMX) {
				       	if (OMX_FALSE == bFirstFrameRetrieved) {
				       		bFirstFrameRetrieved = OMX_TRUE;
				       	}
				       	else {
				       		iCurrentFrameTs += iFrameTsInterval;
				       	}
				       	
						frm_buf[i].ipOutputBuffer->nTimeStamp = iCurrentFrameTs;
				       	LOGE ("@@ Correct TS to %lld", iCurrentFrameTs);
					}
				       
					return frm_buf[i].ipOutputBuffer;
				}
			}
		}
	}
	return NULL;
}
OMX_BUFFERHEADERTYPE* WmvDecoder_OMX::GetFreeBuffer()
{
	VAL_UINT32_T tmp_frame_addr, i;

	//get un-used free frame.
	if( VDEC_DRV_MRESULT_OK != eVDecDrvGetParam(rDrvHandle, VDEC_DRV_GET_TYPE_FREE_FRAME_BUFFER, NULL, &tmp_frame_addr) )
	{
		LOGE("Error!! Cannot get param\n");
		return NULL;
	}
	if(VAL_NULL != tmp_frame_addr)
	{		
		for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
		{
			if(OMX_TRUE == frm_buf[i].bUsed)
			{
				if(tmp_frame_addr == (VAL_UINT32_T)&frm_buf[i].frame_buffer)
				{
					frm_buf[i].bUsed = OMX_FALSE;
					//LOGD("GET_FREE, i:%d, frm_buf:%08x, omx_buf:%08x", i, tmp_frame_addr, frm_buf[i].ipOutputBuffer);
					return frm_buf[i].ipOutputBuffer;
				}
			}
		}
	}
	return NULL;
}
OMX_U32 WmvDecoder_OMX::CheckFreeBuffer()
{
	VAL_UINT32_T i, cnt = 0;
	LOGD("CheckFreeBuffer+");
	for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
	{
		if(OMX_TRUE == frm_buf[i].bUsed)
		{
			cnt++;
			//frm_buf[i].bUsed = OMX_FALSE;
			LOGW("NOT_FREE, i:%d, frm_buf:%08x, omx_buf:%08x", i, (unsigned int)&(frm_buf[i].frame_buffer), (unsigned int)frm_buf[i].ipOutputBuffer);
			//return frm_buf[i].ipOutputBuffer;
		}
	}

	// handle seek
	if (bCorrectTsFromOMX) {
		bFirstFrameRetrieved = OMX_FALSE;
		bResetFirstFrameTs = OMX_TRUE;
		iCurrentFrameTs = 0;
	}

	LOGD("CheckFreeBuffer-, cnt:%u", cnt);
	return cnt;
}

OMX_BOOL WmvDecoder_OMX::isFreeBuffer(OMX_BUFFERHEADERTYPE* ipOutputBuffer)
{
	VAL_UINT32_T tmp_frame_addr, i;

	if(NULL != ipOutputBuffer)
	{		
		for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
		{
			if(OMX_TRUE == frm_buf[i].bUsed)
			{
				if(ipOutputBuffer == (OMX_BUFFERHEADERTYPE *)frm_buf[i].ipOutputBuffer)
				{					
					return OMX_FALSE;
				}
			}
		}
	}
	return OMX_TRUE;
}

VDEC_DRV_FRAMEBUF_T* WmvDecoder_OMX::GetFrmBuf(OMX_TICKS iTimestamp, OMX_BUFFERHEADERTYPE*   ipOutputBuffer)
{
	VAL_INT32_T i;
	
	for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
	{
		if(OMX_FALSE == frm_buf[i].bUsed)
		{
			frm_buf[i].bUsed = OMX_TRUE;
			frm_buf[i].iTimestamp = iTimestamp;
			frm_buf[i].ipOutputBuffer = ipOutputBuffer;
			return (&frm_buf[i].frame_buffer);
		}
	}
	LOGE("Error!! GetFrmBuf not found\n");
	return NULL;
}
OMX_BOOL WmvDecoder_OMX::PutFrmBuf(OMX_BUFFERHEADERTYPE*   ipOutputBuffer)
{
	VAL_INT32_T i;
	
	for(i = 0; i < MFV_OUTPUT_BUFFER_NUM; i++)
	{
		if(OMX_TRUE == frm_buf[i].bUsed)
		{
			if(ipOutputBuffer == frm_buf[i].ipOutputBuffer)
			{
				frm_buf[i].bUsed = OMX_FALSE;
				return OMX_TRUE;
			}
		}
	}
	LOGE("Error!! PutFrmBuf not found\n");
	return OMX_FALSE;
}

OMX_BOOL WmvDecoder_OMX::WmvDecodeVideoFinal()
{
	LOGD("WmvDecodeVideoFinal+");

	if( VDEC_DRV_MRESULT_OK != eVDecDrvDecode(rDrvHandle, NULL, NULL) )
	{
		LOGE("Error!! Cannot decode\n");
		return OMX_FALSE;
	}
	bBufferProcessDone = OMX_FALSE;
	LOGD("WmvDecodeVideoFinal-");
	return OMX_TRUE;
}
#endif


/* Initialization routine */
OMX_ERRORTYPE WmvDecoder_OMX::WmvDecInit()
{
	LOGD("WmvDecInit+");

    WmvInitCompleteFlag = OMX_FALSE;
    WmvInitFlag = 0;
#ifdef MT6573_MFV_HW
		rDrvHandle = NULL;
		codec_mode = 0;
		bHWEnable = OMX_FALSE;
		fgUseExtRefBuf = OMX_FALSE;	
		BITSTREAM_BUFFER_SIZE = 0;
		bBufferProcessDone = OMX_TRUE;
		rDrvExtRefBufAddr.rBufferAddr.u4VA = rDrvExtRefBufAddr.rBufferAddr.u4PA = 0;
		u4BSVa = u4BSPa = 0;
#endif

	if (bCorrectTsFromOMX) {
		iFrameTsInterval = 0;
        	iCurrentFrameTs = 0;
        	bFirstFrameRetrieved = OMX_FALSE;
        	bResetFirstFrameTs = OMX_TRUE;
        	bCorrectTsFromOMX = OMX_FALSE;
	}
        
	LOGD("WmvDecInit-");

    return OMX_ErrorNone;
}


/*Decode routine */
OMX_BOOL WmvDecoder_OMX::WmvDecodeVideo(OMX_U8* aOutBuffer, OMX_U32* aOutputLength,
        OMX_U8** aInputBuf, OMX_U32* aInBufSize,
        OMX_PARAM_PORTDEFINITIONTYPE* aPortParam,
        OMX_S32* aFrameCount, OMX_BOOL aMarkerFlag, OMX_BOOL *aResizeFlag
        , OMX_PARAM_PORTDEFINITIONTYPE* aInPortParam, OMX_TICKS iTimestamp, OMX_BUFFERHEADERTYPE*   ipOutputBuffer)
{
	//LOGD("+WmvDecodeVideo(), omx buf:%08x, tid=%d", ipOutputBuffer, gettid());
// Morris Yang 20101227 [
//#ifdef MT6516_MP4_HW_DECODER_V2
#if defined(MT6573_MFV_HW)
// ]
    if (!mPlayerInitialized)
    {
        char value[PROPERTY_VALUE_MAX];
        if (property_get("media.stagefright.enable-player", value, NULL)
            && (!strcmp(value, "1") || !strcasecmp(value, "true"))) {
            mIsStageFrightPlayer = true;
        }
        LOGE ("WmvDecoder_OMX::WmvDecodeVideo mIsStageFrightPlayer=%d, this=0x%x", mIsStageFrightPlayer, (unsigned int)this);
        mPlayerInitialized = true;
    }
#endif

    
    OMX_BOOL Status = OMX_TRUE;
    OMX_S32 OldWidth, OldHeight, OldFrameSize;

    OldWidth = aPortParam->format.video.nFrameWidth;
    OldHeight = aPortParam->format.video.nFrameHeight;
    *aResizeFlag = OMX_FALSE;

#ifdef _DEBUG
    static OMX_U32 FrameCount = 0;
#endif
    uint32 UseExtTimestamp = 0;
    uint32 TimeStamp;
    //OMX_S32 MaxSize = BIT_BUFF_SIZE;
    OMX_S32 FrameSize, InputSize, InitSize = 0;
    OMX_U8* pTempFrame, *pSrc[3];

    if (WmvInitCompleteFlag == OMX_FALSE)
    {
        int32 aligned_width, aligned_height;
        int32 display_width, display_height;
        
        if (!aMarkerFlag)
        {
            //InitSize = GetVideoHeader(0, *aInputBuf, *aInBufSize);
            // TODO
        }
        else
        {
            InitSize = *aInBufSize;
        }
#ifdef MT6573_MFV_HW
		LOGD("InPort FORMAT:%X", aInPortParam->format.video.eCompressionFormat);

		if (PV_TRUE != InitializeVideoDecode(&iDisplay_Width, &iDisplay_Height, aInputBuf, (OMX_S32*)aInBufSize))
			return OMX_FALSE;
	if(OMX_TRUE == bHWEnable)
        {
        }
        else
        {
#else


#endif
        /*
        if (iGetM4VConfigInfo(*aInputBuf, *aInBufSize, &aligned_width, &aligned_height, &display_width, &display_height))
        {
            return OMX_FALSE;
        }
        */
#ifdef MT6573_MFV_HW
        }
#endif

#ifdef MT6573_MFV_HW
        iDisplay_Width = display_width = rSeqInfo.u4PicWidth;
        iDisplay_Height = display_height = rSeqInfo.u4PicHeight;
#endif

        WmvInitCompleteFlag = OMX_TRUE;
        WmvInitFlag = 1;
        aPortParam->format.video.nFrameWidth = iDisplay_Width;
        aPortParam->format.video.nFrameHeight = iDisplay_Height;

        OMX_U32 min_stride = ((aPortParam->format.video.nFrameWidth + 15) & (~15));
        OMX_U32 min_sliceheight = ((aPortParam->format.video.nFrameHeight + 15) & (~15));

        aPortParam->format.video.nStride = min_stride;
        aPortParam->format.video.nSliceHeight = min_sliceheight;

        // finally, compute the new minimum buffer size.

        // Decoder components always output YUV420 format
        aPortParam->nBufferSize = (aPortParam->format.video.nSliceHeight * aPortParam->format.video.nStride * 3) >> 1;

        if ((iDisplay_Width != OldWidth) || (iDisplay_Height != OldHeight))
        {
            *aResizeFlag = OMX_TRUE;
	    LOGD("*aResizeFlag = OMX_TRUE");
        }

        *aFrameCount = 1;
        *aInBufSize -= InitSize;
        return OMX_TRUE;
    }

    //MaxSize = *aInBufSize;

    if ((*(OMX_S32*)aInBufSize) <= 0)
    {
        return OMX_FALSE;
    }

    TimeStamp = 0xFFFFFFFF;
    InputSize = *aInBufSize;

#ifdef MT6573_MFV_HW
	    // in case of H263, read the 1st frame to find out the sizes (use the m4v_config)
	    if ((0 == *aFrameCount))
	    {
	        int32 display_width, display_height;
	        //if (iGetM4VConfigInfo(*aInputBuf, *aInBufSize, &aligned_width, &aligned_height, &display_width, &display_height))
	        //{
	        //    return OMX_FALSE;
	        //}
		LOGD("InPort FORMAT:%X", aInPortParam->format.video.eCompressionFormat);

		if (InitializeVideoDecodeHW(&iDisplay_Width, &iDisplay_Height, aInputBuf, (OMX_S32*)aInBufSize))
		{
			LOGE("[WMV_DEC][ERROR] InitializeVideoDecodeHW NOT OK, TID = %d", gettid());
			bHWEnable = OMX_FALSE;
		}
	        
			display_width = rSeqInfo.u4PicWidth;
			display_height = rSeqInfo.u4PicHeight;
				
	        iDisplay_Width = display_width;
	        iDisplay_Height = display_height;
	        aPortParam->format.video.nFrameWidth = iDisplay_Width; // use non 16byte aligned values (display_width) for H263
	        aPortParam->format.video.nFrameHeight = iDisplay_Height; // like in the case of M4V (PVGetVideoDimensions also returns display_width/height)

	        OMX_U32 min_stride = ((aPortParam->format.video.nFrameWidth + 15) & (~15));
	        OMX_U32 min_sliceheight = ((aPortParam->format.video.nFrameHeight + 15) & (~15));

	        aPortParam->format.video.nStride = min_stride;
	        aPortParam->format.video.nSliceHeight = min_sliceheight;

	        // finally, compute the new minimum buffer size.
	        // Decoder components always output YUV420 format
	        aPortParam->nBufferSize = (aPortParam->format.video.nSliceHeight * aPortParam->format.video.nStride * 3) >> 1;

	        if ((iDisplay_Width != OldWidth) || (iDisplay_Height != OldHeight))
	        {
	            *aResizeFlag = OMX_TRUE;
				LOGD("*aResizeFlag = OMX_TRUE");
	        }

	        *aFrameCount = 1;
	        return OMX_TRUE;
	    }
	if(OMX_TRUE == bHWEnable)
	{
		//setup input buffer		
		if (OMX_TRUE != WMVHWAllocBitstreamMem(aInputBuf, InputSize))
		{
			LOGE("[MP4_DEC][ERROR] MP4HWAllocBitstreamMem ERROR TID = %d", gettid());
			return OMX_FALSE;
		}		
		*aInBufSize = 0;	
        ringbuf.rBase.u4VA  = u4BSVa;
        ringbuf.rBase.u4PA  = u4BSPa;
        ringbuf.rBase.u4Size = BITSTREAM_BUFFER_SIZE;
		ringbuf.u4Read  = u4BSVa;
		ringbuf.u4Write = u4BSVa + InputSize;

		//setup output buffer
		VDEC_DRV_FRAMEBUF_T		*frame;
		frame = GetFrmBuf(iTimestamp, ipOutputBuffer);
		if(NULL == frame)
		{
            LOGE("error get free frame buffer!");
            return OMX_FALSE;
		}
		// Morris Yang 20101227 [
		if (mIsStageFrightPlayer)
			frame->rBaseAddr.u4PA = (VAL_UINT32_T)sf_pmem_vir2phy(aOutBuffer, 14);
		// ]
		else 
			frame->rBaseAddr.u4PA = (VAL_UINT32_T)oscl_pmem_vir2phy(aOutBuffer);	
		frame->rBaseAddr.u4VA = (VAL_UINT32_T)aOutBuffer;

		//LOGD("frm BUF:%08x, dec buf:%08x, omx buf:%08x, ts:%lld, ", frame, aOutBuffer, ipOutputBuffer, iTimestamp);
	        if( VDEC_DRV_MRESULT_OK != eVDecDrvDecode(rDrvHandle, &ringbuf, frame) )
	        {
			LOGE("Error!! Cannot decode\n");
			Status = (OMX_BOOL)PV_FALSE;
			//PutFrmBuf(ipOutputBuffer);
        	}
		else
		{
			*aOutputLength = aPortParam->nBufferSize;
			Status = (OMX_BOOL)PV_TRUE;
		}
		bBufferProcessDone = OMX_FALSE; //need to get disp and get free no matter error or not


		if (bCorrectTsFromOMX) {
			if (OMX_TRUE == bResetFirstFrameTs) {
				iCurrentFrameTs = iTimestamp;
				LOGE ("@@ iFirstFrameTs = %lld", iCurrentFrameTs);
				bResetFirstFrameTs = OMX_FALSE;
			}
		}

	}
	else
	{	
#endif //#ifdef MT6573_MFV_HW

    // in case of H263, read the 1st frame to find out the sizes (use the m4v_config)
    if ((0 == *aFrameCount))
    {
        int32 aligned_width, aligned_height;
        int32 display_width, display_height;
	// TODO: get resolution here

        aPortParam->format.video.nFrameWidth = iDisplay_Width; // use non 16byte aligned values (display_width) for H263
        aPortParam->format.video.nFrameHeight = iDisplay_Height; // like in the case of M4V (PVGetVideoDimensions also returns display_width/height)

        OMX_U32 min_stride = ((aPortParam->format.video.nFrameWidth + 15) & (~15));
        OMX_U32 min_sliceheight = ((aPortParam->format.video.nFrameHeight + 15) & (~15));


        aPortParam->format.video.nStride = min_stride;
        aPortParam->format.video.nSliceHeight = min_sliceheight;

        // finally, compute the new minimum buffer size.

        // Decoder components always output YUV420 format
        aPortParam->nBufferSize = (aPortParam->format.video.nSliceHeight * aPortParam->format.video.nStride * 3) >> 1;


        if ((iDisplay_Width != OldWidth) || (iDisplay_Height != OldHeight))
            *aResizeFlag = OMX_TRUE;

        *aFrameCount = 1;

        return OMX_TRUE;
    }

#ifdef MT6573_MFV_HW
    }
#endif  //#ifdef MT6573_MFV_HW

#ifdef MT6516_MP4_HW_DECODER_V2
#endif //#ifdef MT6516_MP4_HW_DECODER_V2

    if (Status == PV_TRUE)
    {

#ifdef _DEBUG
        //printf("Frame number %d\n", ++FrameCount);
#endif
        // advance input buffer ptr
        *aInputBuf += (InputSize - *aInBufSize);

        pTempFrame = (OMX_U8*) pFrame0;
        pFrame0 = (OMX_U8*) pFrame1;
        pFrame1 = (OMX_U8*) pTempFrame;

#ifdef MT6573_MFV_HW
	if(OMX_TRUE == bHWEnable)
	{
	    	//LOGW("temp use copy for output");
		//oscl_memcpy((void *)aOutBuffer, (void *)rDrvExtRefBufAddr.rBufferAddr.u4VA, *aOutputLength);
		/*
		FILE *fp = fopen("/sdcard/out.bin", "ab");
		if(fp)
		{
			fwrite((void *)aOutBuffer, 1, *aOutputLength, fp);
			fclose(fp);
		}
		*/
	}
	else
	{
		//LOGD("ORIGINAL OUTPUT");
	}
#endif

        (*aFrameCount)++;
    }
    else
    {
        *aInBufSize = InputSize;
        *aOutputLength = 0;
    }

    //LOGD("-WmvDecodeVideo(), tid=%d", gettid());
    return Status;
}

#ifdef MT6573_MFV_HW
OMX_S32 WmvDecoder_OMX::InitializeVideoDecodeHW(
    OMX_S32* aWidth, OMX_S32* aHeight, OMX_U8** aBuffer, OMX_S32* aSize)
{
	VDEC_DRV_MRESULT_T				eResult;

	LOGD("InitializeVideoDecodeHW+");

	if(WmvInitFlag)
	{
		//need free first
		if(OMX_TRUE == bHWEnable)
		{
			if(rDrvExtRefBufAddr.rBufferAddr.u4VA)
			{
				PMEM_FREE(rDrvExtRefBufAddr.rBufferAddr.u4VA);
				rDrvExtRefBufAddr.rBufferAddr.u4VA = 0;
			}
			if(u4BSVa)
			{
				PMEM_FREE(u4BSVa);
				u4BSVa = 0;
			}
		    if( VDEC_DRV_MRESULT_OK != eVDecDrvDeInit(rDrvHandle) )
			{
			    LOGE("Error!! deinit fail: eVDecDrvDeInit\n");
			    eVDecDrvRelease(rDrvHandle);
			    return 1;
			}
			if( VDEC_DRV_MRESULT_OK != eVDecDrvRelease(rDrvHandle) )
			{
			    LOGE("Error!! release fail: eVDecDrvRelease\n");
			}
			rDrvHandle = NULL;
			bHWEnable = OMX_FALSE;
		}
	}		
	
	// create driver
	eResult = eVDecDrvCreate(&rDrvHandle, VDEC_DRV_VIDEO_FORMAT_VC1);
	
	if(VDEC_DRV_MRESULT_OK != eResult)
	{
		LOGE("Error!! Cannot create driver\n");
		return 1;
	}
	

	// init driver using sequance layer
    ringbuf.rBase.u4VA  = (VAL_UINT32_T)(*aBuffer);
    ringbuf.rBase.u4PA  = (VAL_UINT32_T)(*aBuffer);
    ringbuf.rBase.u4Size = *aSize;
	ringbuf.u4Read  = (VAL_UINT32_T)(*aBuffer);
	ringbuf.u4Write = ringbuf.u4Read + (VAL_UINT32_T)*aSize;//(!(VAL_UINT32_T)is_short_header) ? (VAL_UINT32_T)end : (VAL_UINT32_T)len;	
	if( VDEC_DRV_MRESULT_OK != eVDecDrvInit(rDrvHandle, &ringbuf, &rSeqInfo) )
	{
		LOGE("Error!! Cannot init driver\n");
		goto ON_InitializeVideoDecodeHW_ERROR;
	}
	
	*aWidth  = rSeqInfo.u4PicWidth;
	*aHeight = rSeqInfo.u4PicHeight;

	/*
	for(int i = 0; i<10; i++)
	{
	LOGD("bs[%d]:%08x", i, *((unsigned *)(ringbuf.u4Read+i*4)));
	}
	*/

	VAL_UINT32_T is_support;
	VDEC_DRV_QUERY_VIDEO_FORMAT_T  qinfo;
	VDEC_DRV_QUERY_BUFFER_MODE_T  rBufferMode;
	VDEC_DRV_QUERY_POOL_SIZE_T	rPoolSize;
	
	// query driver capability
	qinfo.u4VideoFormat = VDEC_DRV_VIDEO_FORMAT_VC1;
	qinfo.u4Profile = VDEC_DRV_MS_VIDEO_PROFILE_VC1_SIMPLE;
	qinfo.u4Level = VDEC_DRV_VIDEO_LEVEL_0;
	qinfo.u4Resolution = VDEC_DRV_RESOLUTION_SUPPORT_480P;
	qinfo.u4Width = *aWidth;
	qinfo.u4Height = *aHeight;
	is_support = eVDecDrvQueryCapability(VDEC_DRV_QUERY_TYPE_VIDEO_FORMAT, &qinfo, 0);
	if(VDEC_DRV_MRESULT_FAIL == is_support )
	{
		LOGE("Error!! driver capability Cannot support.\n");
		goto ON_InitializeVideoDecodeHW_ERROR;
	}
	

    // query driver property
    is_support = eVDecDrvQueryCapability(VDEC_DRV_QUERY_TYPE_PROPERTY, &qinfo, &rDrvProperty);
    if(VDEC_DRV_MRESULT_FAIL == is_support )
    {
        LOGD("Error!! Cannot get decoder property.\n");
		goto ON_InitializeVideoDecodeHW_ERROR;
    }
    LOGD("buffer alignment requirement:%d\n", rDrvProperty.u4BufAlign);
    LOGD("buffer unit size is N bytes:%d\n", rDrvProperty.u4BufUnitSize);
    LOGD("support post-process:%d\n", rDrvProperty.fgPostprocessSupport);

    //init frame buffer array.
	oscl_memset((void *)frm_buf, 0, sizeof(FrmBufStruct)*MFV_OUTPUT_BUFFER_NUM);

	bBufferProcessDone = OMX_TRUE;
	BITSTREAM_BUFFER_SIZE = 1024*1000;
	bHWEnable = OMX_TRUE;
	return 0;
	
ON_InitializeVideoDecodeHW_ERROR:
	eVDecDrvRelease(rDrvHandle);
	return 1;
}
#endif


OMX_S32 WmvDecoder_OMX::InitializeVideoDecode( OMX_S32* aWidth, OMX_S32* aHeight, OMX_U8** aBuffer, OMX_S32* aSize)
{
    LOGD("-------InitializeVideoDecode--IN- aSize = %ld", *aSize);
 
    OMX_U32 VideoDecOutputSize;
    OMX_S32 OK = PV_TRUE;

// Annex L, page 458
// Morris Yang: make Sequence Layer from BITMAPINFOHDR (aBuffer is BITMAPINFOHDR)
    OMX_S32 _SeqLen = 36;
    OMX_U8* _pSequenceLayer = new OMX_U8[36];
    memset(_pSequenceLayer, 0x00, _SeqLen);
    *((OMX_U32*)(((OMX_U8*)_pSequenceLayer))) = 0xC5000000;
    *((OMX_U32*)(((OMX_U8*)_pSequenceLayer+4))) = 0x00000004;
    memcpy (_pSequenceLayer+8, (*aBuffer+40), 4);
    *((OMX_U32*)(((OMX_U8*)_pSequenceLayer+12))) = *((OMX_U32*)((OMX_U8*)*aBuffer+8));
    *((OMX_U32*)(((OMX_U8*)_pSequenceLayer+16))) = *((OMX_U32*)((OMX_U8*)*aBuffer+4));
    *((OMX_U32*)(((OMX_U8*)_pSequenceLayer+20))) = 0x0000000C;


    OMX_U32 uSTRUCT_C = *((OMX_U32*)(*aBuffer+40));
    //LOGE ("STRUCT_C = 0x%x, MAXBFRAMES=%d", uSTRUCT_C, ((uSTRUCT_C & 0x70000000) >> 28));
    bCorrectTsFromOMX = (((uSTRUCT_C & 0x70000000) >> 28) > 0 ? OMX_TRUE : OMX_FALSE);
    if (bCorrectTsFromOMX) {
        OMX_U8 biSize = *((OMX_U32*)((OMX_U8*)*aBuffer));	// total size of BITMAPINFOHDR (including extra data)
        iFrameTsInterval = (OMX_TICKS)(1000*1000*100 / *((OMX_U32*)(*aBuffer+biSize)));   // (*aBuffer+44) is FPS*100
        LOGD ("InitializeVideoDecode 100*FPS=%lu, uFrameTsInterval=%lld us", *((OMX_U32*)(*aBuffer+biSize)), iFrameTsInterval);
    }

   #ifdef MT6573_MFV_HW
	if (1)
	{
		//if(InitializeVideoDecodeHW(aWidth, aHeight, aBuffer, aSize))
		if(InitializeVideoDecodeHW(aWidth, aHeight, &_pSequenceLayer, &_SeqLen))
		{
			LOGE("[WMV_DEC][ERROR] InitializeVideoDecodeHW ERROR, TID = %d", gettid());
			//return PV_FALSE;
		}
		else
		{
			LOGD("[WMV_DEC] InitializeVideoDecodeHW OK, TID = %d", gettid());
			OK = PV_TRUE;
			//return PV_TRUE;
		}
	}
	else
	{
		LOGD("[WMV_DEC] not init HW yet, TID = %d", gettid());
		if (*aWidth == 0 || *aHeight == 0)
		{
			*aWidth = PVH263DEFAULTWIDTH;
			*aHeight = PVH263DEFAULTHEIGHT;
		}
	}

	delete [] _pSequenceLayer;
#endif

	if (OK)
	{
		return PV_TRUE;
	}
	else
	{
		return PV_FALSE;
	}

}

OMX_ERRORTYPE WmvDecoder_OMX::WmvDecDeinit()
{
    OMX_BOOL Status;
	LOGD("Mp4DecDeinit+");
#ifdef MT6573_MFV_HW
	if(OMX_TRUE == bHWEnable)
	{
		CheckFreeBuffer();
		if(rDrvExtRefBufAddr.rBufferAddr.u4VA)
		{
			PMEM_FREE(rDrvExtRefBufAddr.rBufferAddr.u4VA);
			rDrvExtRefBufAddr.rBufferAddr.u4VA = 0;
		}
		if(u4BSVa)
		{
			PMEM_FREE(u4BSVa);
			u4BSVa = 0;
		}		
	    if( VDEC_DRV_MRESULT_OK != eVDecDrvDeInit(rDrvHandle) )
	    {
	        LOGE("Error!! deinit fail: eVDecDrvDeInit\n");
	        eVDecDrvRelease(rDrvHandle);
	        //goto ON_LocalMainMp4Dec_ERROR;
	    }
	    if( VDEC_DRV_MRESULT_OK != eVDecDrvRelease(rDrvHandle) )
	    {
	        LOGE("Error!! release fail: eVDecDrvRelease\n");
	    }
		rDrvHandle = NULL;
		bHWEnable = OMX_FALSE;		
	}
#endif


    if (pFrame0)
    {
        oscl_free(pFrame0);
        pFrame0 = NULL;
    }
    if (pFrame1)
    {
        oscl_free(pFrame1);
        pFrame1 = NULL;
    }
	LOGD("WmvDecDeinit-");

    return OMX_ErrorNone;
}

OMX_S32 WmvDecoder_OMX::GetVideoHeader(int32 aLayer, uint8* aBuf, int32 aMaxSize)
{
    OSCL_UNUSED_ARG(aLayer);

    return 0;
}

OMX_BOOL WmvDecoder_OMX::WMVDecHwSupported()
{
#ifdef MT6573_MFV_HW
	if(1){
		return bHWEnable;
	}
#endif
    return OMX_FALSE;
}


OMX_BOOL WmvDecoder_OMX::WMVHWAllocBitstreamMem(OMX_U8** aInputBuf, OMX_U32 aInputSize)
{
#ifdef MT6573_MFV_HW 
		OMX_U32 bitstream_for_padding_zero;
		OMX_U32 u4TryAllocMemCount;
	
		if (bWMV_BitstreamAllocated == OMX_FALSE || BITSTREAM_BUFFER_SIZE < aInputSize)
		{
			if(bWMV_BitstreamAllocated != OMX_FALSE)
			{
				BITSTREAM_BUFFER_SIZE = aInputSize;
			// Morris Yang 20101227 [
				if (mIsStageFrightPlayer)
					STAGEFRIGHT_PMEM_FREE(u4BSVa);
			// ]
				else
					PMEM_FREE(u4BSVa);
			}
			u4BSVa = 0;
			u4TryAllocMemCount = WMV_DEC_TRYALLOCMEMCOUNT;
			
			while (u4TryAllocMemCount)
			{
			// Morris Yang 20101227 [
				if (mIsStageFrightPlayer)
					STAGEFRIGHT_PMEM_ALLOC(u4BSVa, u4BSPa, BITSTREAM_BUFFER_SIZE);
			// ]
				else
					PMEM_ALLOC(u4BSVa, u4BSPa, BITSTREAM_BUFFER_SIZE);
	
				if (u4BSVa == 0)
				{
					LOGE("Alloc u4BSVa fail %lu times!!, Try alloc again!!", (WMV_DEC_TRYALLOCMEMCOUNT-u4TryAllocMemCount));
					u4TryAllocMemCount--;
					usleep(10*1000);
				}
				else
				{
					LOGD("Alloc u4BSVa 0x%08x Success", u4BSVa);
					break;
				}
			}
	
			if (u4BSVa == 0)
			{
				LOGE("Alloc u4BSVa fail %d times!!, Return error!!\n", WMV_DEC_TRYALLOCMEMCOUNT);
				return OMX_FALSE;
			}
				
			bWMV_BitstreamAllocated = OMX_TRUE;
		}
		oscl_memcpy((void*)u4BSVa, *aInputBuf, aInputSize);
			
		for (bitstream_for_padding_zero = 0;bitstream_for_padding_zero < 32;bitstream_for_padding_zero++)
		{
			*((char *)(u4BSVa + bitstream_for_padding_zero + aInputSize)) = 0;
		}
	
		return OMX_TRUE;
#endif
    return OMX_FALSE;
}


OMX_BOOL WmvDecoder_OMX::WMVHWDecodeVideo(OMX_U32 aInputSize, OMX_U8* aOutBuffer, OMX_S32* aFrameCount)
{
#ifdef MT6516_MP4_HW_DECODER_V2 
#else
    return OMX_FALSE;
#endif
}

OMX_BOOL WmvDecoder_OMX::WMVHWPostProcessing(OMX_U8* aOutBuffer, OMX_U32* aOutputLength, OMX_S32* aFrameCount)
{
    return OMX_FALSE;
}
