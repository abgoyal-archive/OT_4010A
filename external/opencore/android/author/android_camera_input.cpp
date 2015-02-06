

//#define LOG_NDEBUG 0
//#define SAVE_BUF_TIME
#define LOG_TAG "CameraInput"
#include <utils/Log.h>
#include <camera/CameraParameters.h>
#include <utils/Errors.h>
#include <media/mediarecorder.h>
#include <surfaceflinger/ISurface.h>
#include <camera/ICamera.h>
#include <camera/Camera.h>

#include "pv_mime_string_utils.h"
#include "oscl_dll.h"
#include "oscl_tickcount.h"

#include "android_camera_input.h"

#include "oscl_pmem.h"

#ifdef MHAL_H_INCLUDED
#include "MediaHal.h"
#endif

#ifdef SAVE_BUF_TIME
FILE *out_fp = NULL;
#endif

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MEM_DEV_PATH	"/dev/mem"
#define KMEM_DEV_PATH	"/dev/kmem"

#define REG_BASE       (0x00000000)/* physical address */

#define READ_REG_DATA(a,v) v = *((volatile unsigned int *)((base) + ((a)-REG_BASE)))
#define WRITE_REG_DATA(a,v) *((volatile unsigned int *)((base) + ((a)-REG_BASE))) = v

int dump_md_image()
{
	int fd;
	char c;
	unsigned char *base;
	volatile unsigned int addr, value;
	fd = open(MEM_DEV_PATH, O_RDWR);
	if (fd < 0) {
		LOGD("MEM_DEV_PATH open failed");
		return 1;
	}

	base = (unsigned char *) mmap(NULL, 0x1000000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, REG_BASE);
	if (base == MAP_FAILED) {
		LOGD("mmap failed");
		return 1;
	}

	for(addr = 0; addr<=0xFC; addr+=4)
	{
	READ_REG_DATA(addr,value);
	LOGD("Read 0x%x as 0x%x\n", addr, value);		
	}
	munmap(base, REG_BASE);
	close(fd);

	return 0;
}

#define CCIF_BASE       (0x80036000)/* physical address */
/* define HW register */
#define CCIF_CON(a) ((volatile unsigned int *)((a) + 0x0000))
#define CCIF_BUSY(a) ((volatile unsigned int *)((a) + 0x0004))
#define CCIF_START(a) ((volatile unsigned int *)((a) + 0x0008))
#define CCIF_TCHNUM(a) ((volatile unsigned int *)((a) + 0x000C))
#define CCIF_RCHNUM(a) ((volatile unsigned int *)((a) + 0x0010))
#define CCIF_ACK(a) ((volatile unsigned int *)((a) + 0x0014))
/* for CHDATA, the first half space belongs to AP and the remaining space belongs to MD */
#define CCIF_TXCHDATA(a) ((volatile unsigned int *)((a) + 0x0100))
#define CCIF_RXCHDATA(a) ((volatile unsigned int *)((a) + 0x0100 + 128))

int dump_ccci_reg()
{
	int fd, i;
	unsigned char *base;
	unsigned int k;

	fd = open(MEM_DEV_PATH, O_RDWR);
	if (fd < 0) {
		LOGD("MEM_DEV_PATH open failed");
		return 1;
	}

	base = (unsigned char *) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, CCIF_BASE);
	if (base == MAP_FAILED) {
		LOGD("mmap failed");
		return 1;
	}

	LOGD("CCIF_CON    (0x%p) = %X\n", CCIF_CON(base), *CCIF_CON(base));
	LOGD("CCIF_BUSY   (0x%p) = %X\n", CCIF_BUSY(base), *CCIF_BUSY(base));
	LOGD("CCIF_START  (0x%p) = %X\n", CCIF_START(base), *CCIF_START(base));
	LOGD("CCIF_RCHNUM (0x%p) = %X\n", CCIF_RCHNUM(base), *CCIF_RCHNUM(base));

	munmap(base, CCIF_BASE);
	close(fd);

	return 0;
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

using namespace android;

static const int VIEW_FINDER_FREEZE_DETECTION_TOLERANCE = 250;  // ms

// Define entry point for this DLL
OSCL_DLL_ENTRY_POINT_DEFAULT()

bool bCompressVideoStart = false;
bool bCompressVideoReady = true;
bool bIsMp4 = false;
sp<android::Camera>     mCameraExtra = 0;
bool bCompressVideoHasAudio = false;

PVRefBufferAlloc::~PVRefBufferAlloc()
{
    if(numAllocated != 0)
    {
        LOGE("Ln %d ERROR PVRefBufferAlloc numAllocated %d",__LINE__, numAllocated );
    }
}

// camera MIO
AndroidCameraInput::AndroidCameraInput()
    : OsclTimerObject(OsclActiveObject::EPriorityNominal, "AndroidCameraInput"),
    iWriteState(EWriteOK),
    iAuthorClock(NULL),
    iClockNotificationsInf(NULL),
    iAudioFirstFrameTs(0),
    iAudioLossDuration(0),
    pPmemInfo(NULL)
    //Start to modify by Mingliang.Zhong@2010-09-14
    ,i1stFramesend(true)
    ,iCheck1stVideoFrameWrite(false)
    , iCheck1stVideoFrameWriteParams(NULL)
    ,ipCameraInputMIONotify(NULL)
    ,iFrameCounter(0)
    //End to modify by Mingliang.Zhong@2010-09-14
{
    LOGV("constructor(%p)", this);
    iCmdIdCounter = 0;
    iPeer = NULL;
    iThreadLoggedOn = false;
    iDataEventCounter = 0;
    iTimeStamp = 0;
    iMilliSecondsPerDataEvent = 0;
    iMicroSecondsPerDataEvent = 0;
    iState = STATE_IDLE;
    mFrameWidth = ANDROID_DEFAULT_FRAME_WIDTH;
    mFrameHeight= ANDROID_DEFAULT_FRAME_HEIGHT;
    mFrameRate  = ANDROID_DEFAULT_FRAME_RATE;
    mBitRate = ANDROID_DEFAULT_BITRATE;
    mCamera = NULL;

    mFlags = 0;
    iFrameQueue.reserve(5);
    iFrameQueueMutex.Create();    
    iAudioLossMutex.Create();

    // setup callback listener
    mListener = new AndroidCameraInputListener(this);
#ifdef COMPRESS_INPUT
#ifdef MHAL_H_INCLUDED
    //if(ANDROID_VIDEO_FORMAT == PVMF_MIME_M4V)
    if(1)
    {
        MHalLockParam_t lock_param;
        uint32 RetBytes = 0;

        lock_param.mode         = MHAL_MODE_MP4_RECORD;
        lock_param.waitMilliSec = 0x0FFFFFFF;
        lock_param.waitMode     = (MHAL_MODE_JPEG_DECODE | MHAL_MODE_JPEG_ENCODE | MHAL_MODE_CAM_CAPTURE | MHAL_MODE_MP4_PLAYBACK | MHAL_MODE_MP4_RECORD);

        if(MHAL_NO_ERROR != mHalIoCtrl(MHAL_IOCTL_LOCK_RESOURCE, (void *)&lock_param, sizeof(lock_param), NULL, 0, NULL))
        {
            LOGE("AndroidCameraInput::DoInit, mhal lock fail");
            //gUseMP4HWEnc = false;
            return;
        }
        else
        {
            LOGD("AndroidCameraInput::DoInit, mhal lock OK");
        }
    }
#endif    
	bCompressVideoStart = false;
	bCompressVideoReady = true;
	bIsMp4 = false;
	mCameraExtra = 0;
	bCompressVideoHasAudio = false;
#endif
}

void AndroidCameraInput::ReleaseQueuedFrames()
{    
    iFrameQueueMutex.Lock();
    LOGD("ReleaseQueuedFrames...");
    while (!iFrameQueue.empty()) {
        AndroidCameraInputMediaData data = iFrameQueue[0];
        iFrameQueue.erase(iFrameQueue.begin());
#if (LOG_NDEBUG == 0)
        ssize_t offset = 0;
        size_t size = 0;
        sp<IMemoryHeap> heap = data.iFrameBuffer->getMemory(&offset, &size);
        LOGD("writeComplete: ID = %d, base = %p, offset = %ld, size = %d ReleaseQueuedFrames", heap->getHeapID(), heap->base(), offset, size);
#endif
        mCamera->releaseRecordingFrame(data.iFrameBuffer);
    }
    if(pPmemInfo)
    {
        delete[]  pPmemInfo;
        pPmemInfo = NULL;
    }
    iFrameQueueMutex.Unlock();
}

AndroidCameraInput::~AndroidCameraInput()
{
    LOGV("destructor");
    if (mCamera != NULL) {
        mCamera->setListener(NULL);
        ReleaseQueuedFrames();
        if ((mFlags & FLAGS_HOT_CAMERA) == 0) {
            LOGV("camera was cold when we started, stopping preview");
            mCamera->stopPreview();
        }
        if (mFlags & FLAGS_SET_CAMERA) {
            LOGV("unlocking camera to return to app");
            mCamera->unlock();
        } else {
            LOGV("disconnect from camera");
            mCamera->disconnect();
        }
        mFlags = 0;
        mCamera.clear();
    }
    iFrameQueueMutex.Close();
    iAudioLossMutex.Close();
    mListener.clear();

#ifdef COMPRESS_INPUT
#ifdef MHAL_H_INCLUDED
    //if(ANDROID_VIDEO_FORMAT == PVMF_MIME_M4V)
    if(1)
    {
        MHalLockParam_t lock_param;
        lock_param.mode = MHAL_MODE_MP4_RECORD;
        if(MHAL_NO_ERROR != mHalIoCtrl(MHAL_IOCTL_UNLOCK_RESOURCE, (void *)&lock_param, sizeof(lock_param), NULL, 0, NULL))
        {
            LOGD("AndroidCameraInput::DoStop, mhal unlock fail,...  check mhal");
            //return PVMFFailure;
        }
        else
        {
            LOGD("AndroidCameraInput::DoStop, mhal unlock OK");
        }
    }
#endif
#endif
}

PVMFStatus AndroidCameraInput::connect(PvmiMIOSession& aSession,
        PvmiMIOObserver* aObserver)
{
    LOGV("connect");
    if (!aObserver) {
        LOGE("observer is a NULL pointer");
        return PVMFFailure;
    }

    int32 err = 0;
    OSCL_TRY(err, iObservers.push_back(aObserver));
    OSCL_FIRST_CATCH_ANY(err,
        LOGE("Out of memory"); return PVMFErrNoMemory);

    // Session ID is the index of observer in the vector
    aSession = (PvmiMIOSession)(iObservers.size() - 1);
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::disconnect(PvmiMIOSession aSession)
{
    LOGV("disconnect");
    uint32 index = (uint32) aSession;
    uint32 size  = iObservers.size();
    if (index >= size) {
        LOGE("Invalid session ID %d. Valid range is [0, %d]", index, size - 1);
        return PVMFFailure;
    }

    iObservers.erase(iObservers.begin() + index);
    return PVMFSuccess;
}

PvmiMediaTransfer* AndroidCameraInput::createMediaTransfer(
        PvmiMIOSession& aSession,
        PvmiKvp* read_formats,
        int32 read_flags,
        PvmiKvp* write_formats,
        int32 write_flags)
{
    LOGV("createMediaTransfer");
    OSCL_UNUSED_ARG(read_formats);
    OSCL_UNUSED_ARG(read_flags);
    OSCL_UNUSED_ARG(write_formats);
    OSCL_UNUSED_ARG(write_flags);

    uint32 index = (uint32) aSession;
    uint32 size  = iObservers.size();
    if (index >= size) {
        LOGE("Invalid session ID %d. Valid range is [0, %d]", index, size - 1);
        OSCL_LEAVE(OsclErrArgument);
        return NULL;
    }

    return (PvmiMediaTransfer*)this;
}

void AndroidCameraInput::deleteMediaTransfer(PvmiMIOSession& aSession,
        PvmiMediaTransfer* media_transfer)
{
    LOGV("deleteMediaTransfer");
    uint32 index = (uint32) aSession;
    uint32 size  = iObservers.size();
    if (index >= size) {
        LOGE("Invalid session ID %d. Valid range is [0, %d]", index, size - 1);
        OSCL_LEAVE(OsclErrArgument);
        return;
    }
    if (!media_transfer) {
        LOGE("media transfer is a NULL pointer");
        OSCL_LEAVE(OsclErrArgument);
    }

    // TODO:
    // 1. I did not see how the media transfer session has been terminated
    //    after this method call.
    // 2. according to pvmi_mio_control.h, this should also check with there
    //    is any outstanding buffer?
}

PVMFCommandId AndroidCameraInput::QueryUUID(const PvmfMimeString& aMimeType,
        Oscl_Vector<PVUuid,
        OsclMemAllocator>& aUuids,
        bool aExactUuidsOnly,
        const OsclAny* aContext)
{
    LOGV("QueryUUID");
    OSCL_UNUSED_ARG(aMimeType);
    OSCL_UNUSED_ARG(aExactUuidsOnly);

    int32 err = 0;
    OSCL_TRY(err, aUuids.push_back(PVMI_CAPABILITY_AND_CONFIG_PVUUID););
    OSCL_FIRST_CATCH_ANY(err,
        LOGE("Out of memory"); OSCL_LEAVE(OsclErrNoMemory));

    return AddCmdToQueue(CMD_QUERY_UUID, aContext);
}

PVMFCommandId AndroidCameraInput::QueryInterface(const PVUuid& aUuid,
        PVInterface*& aInterfacePtr,
        const OsclAny* aContext)
{
    LOGV("QueryInterface");
    if (aUuid == PVMI_CAPABILITY_AND_CONFIG_PVUUID) {
        PvmiCapabilityAndConfig*
           myInterface = OSCL_STATIC_CAST(PvmiCapabilityAndConfig*,this);

        aInterfacePtr = OSCL_STATIC_CAST(PVInterface*, myInterface);
    } else {
        aInterfacePtr = NULL;
    }

    return AddCmdToQueue(CMD_QUERY_INTERFACE,
                         aContext,
                         (OsclAny*)&aInterfacePtr);
}

PVMFCommandId AndroidCameraInput::Init(const OsclAny* aContext)
{
    LOGV("Init");
    if (iState != STATE_IDLE) {
        LOGE("Init called in an invalid state(%d)", iState);
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    return AddCmdToQueue(CMD_INIT, aContext);
}


PVMFCommandId AndroidCameraInput::Start(const OsclAny* aContext)
{
    LOGV("Start");
    if (iState != STATE_INITIALIZED && iState != STATE_PAUSED) {
        LOGE("Start called in an invalid state(%d)", iState);
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    return AddCmdToQueue(CMD_START, aContext);
}

PVMFCommandId AndroidCameraInput::Pause(const OsclAny* aContext)
{
    LOGV("Pause");
    if (iState != STATE_STARTED) {
        LOGE("Pause called in an invalid state(%d)", iState);
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    return AddCmdToQueue(CMD_PAUSE, aContext);
}

PVMFCommandId AndroidCameraInput::Flush(const OsclAny* aContext)
{
    LOGV("Flush");
    if (iState != STATE_STARTED && iState != STATE_PAUSED) {
        LOGE("Flush called in an invalid state(%d)", iState);
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    return AddCmdToQueue(CMD_FLUSH, aContext);
}

PVMFCommandId AndroidCameraInput::Reset(const OsclAny* aContext)
{
    LOGV("Reset");
    return AddCmdToQueue(CMD_RESET, aContext);
}

PVMFCommandId AndroidCameraInput::DiscardData(PVMFTimestamp aTimestamp,
        const OsclAny* aContext)
{
    LOGV("DiscardData with time stamp");
    OSCL_UNUSED_ARG(aContext);
    OSCL_UNUSED_ARG(aTimestamp);
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

PVMFCommandId AndroidCameraInput::DiscardData(const OsclAny* aContext)
{
    LOGV("DiscardData");
    OSCL_UNUSED_ARG(aContext);
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

PVMFCommandId AndroidCameraInput::Stop(const OsclAny* aContext)
{
    LOGV("Stop");
    if (iState != STATE_STARTED && iState != STATE_PAUSED) {
        LOGE("Stop called in an invalid state(%d)", iState);
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    return AddCmdToQueue(CMD_STOP, aContext);
}

void AndroidCameraInput::ThreadLogon()
{
    LOGV("ThreadLogon");
    if (!iThreadLoggedOn) {
        AddToScheduler();
        iThreadLoggedOn = true;
    }
}

void AndroidCameraInput::ThreadLogoff()
{
    LOGV("ThreadLogoff");
    if (iThreadLoggedOn) {
        RemoveFromScheduler();
        iThreadLoggedOn = false;
    }
}

PVMFCommandId AndroidCameraInput::CancelAllCommands(const OsclAny* aContext)
{
    LOGV("CancelAllCommands");
    OSCL_UNUSED_ARG(aContext);
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

PVMFCommandId AndroidCameraInput::CancelCommand(PVMFCommandId aCmdId,
        const OsclAny* aContext)
{
    LOGV("CancelCommand");
    OSCL_UNUSED_ARG(aCmdId);
    OSCL_UNUSED_ARG(aContext);
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

void AndroidCameraInput::setPeer(PvmiMediaTransfer* aPeer)
{
    LOGV("setPeer iPeer %p aPeer %p", iPeer, aPeer);
    if(iPeer && aPeer){
        LOGE("setPeer iPeer %p aPeer %p", iPeer, aPeer);
        OSCL_LEAVE(OsclErrGeneral);
        return;
    }

    iPeer = aPeer;
}

void AndroidCameraInput::useMemoryAllocators(OsclMemAllocator* write_alloc)
{
    LOGV("useMemoryAllocators");
    OSCL_UNUSED_ARG(write_alloc);
    OSCL_LEAVE(OsclErrNotSupported);
}

PVMFCommandId AndroidCameraInput::writeAsync(uint8 aFormatType,
        int32 aFormatIndex,
        uint8* aData,
        uint32 aDataLen,
        const PvmiMediaXferHeader& data_header_info,
        OsclAny* aContext)
{
    LOGV("writeAsync");
    OSCL_UNUSED_ARG(aFormatType);
    OSCL_UNUSED_ARG(aFormatIndex);
    OSCL_UNUSED_ARG(aData);
    OSCL_UNUSED_ARG(aDataLen);
    OSCL_UNUSED_ARG(data_header_info);
    OSCL_UNUSED_ARG(aContext);
    // This is an active data source. writeAsync is not supported.
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

void AndroidCameraInput::writeComplete(PVMFStatus aStatus,
       PVMFCommandId write_cmd_id,
       OsclAny* aContext)
{
    LOGV("writeComplete");
    OSCL_UNUSED_ARG(aContext);

    iFrameQueueMutex.Lock();
    if (iSentMediaData.empty()) {
        LOGE("Nothing to complete");
        iFrameQueueMutex.Unlock();
        return;
    }

    AndroidCameraInputMediaData data = iSentMediaData[0];

    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = data.iFrameBuffer->getMemory(&offset, &size);

#if (LOG_NDEBUG == 0)
    LOGD("writeComplete: ID = %d, base = %p, offset = %ld, size = %d", heap->getHeapID(), heap->base(), offset, size);
#endif
    // View finder freeze detection
    // Note for low frame rate, we don't bother to log view finder freezes
#if 0
    int processingTimeInMs = (systemTime()/1000000LL - (iVideoFirstFrameTs + iAudioLossDuration)) - data.iXferHeader.timestamp+2;
    if (processingTimeInMs >= VIEW_FINDER_FREEZE_DETECTION_TOLERANCE && mFrameRate >= 10.0) {
        //LOGW("Frame %p takes too long (%d ms) to process, staring at %d", data.iFrameBuffer.get(), processingTimeInMs, iAudioFirstFrameTs);
    }
#else
    struct timeval endTimeVal;
    gettimeofday(&endTimeVal, NULL);
    long long timeInMs = (endTimeVal.tv_sec - iVideoFirstFrameTs.tv_sec) * 1000LL + (endTimeVal.tv_usec - iVideoFirstFrameTs.tv_usec)/1000LL - iAudioLossDuration;
    int processingTimeInMs = timeInMs - data.iXferHeader.timestamp+2;
    if (processingTimeInMs >= 250)
        LOGW("warning: timeInMs:%d, ts:%d, res:%d", timeInMs, data.iXferHeader.timestamp, processingTimeInMs);
    #ifdef SAVE_BUF_TIME        
        if(out_fp)
            fprintf(out_fp, "%d\n", processingTimeInMs);
    #endif
#endif

    mCamera->releaseRecordingFrame(data.iFrameBuffer);

    iSentMediaData.erase(iSentMediaData.begin());
    iFrameQueueMutex.Unlock();

    // reference count is always updated, even if the write fails
    if (aStatus != PVMFSuccess) {
        LOGE("writeAsync failed. aStatus=%d", aStatus);
    }
}

PVMFCommandId AndroidCameraInput::readAsync(uint8* data,
        uint32 max_data_len,
        OsclAny* aContext,
        int32* formats,
        uint16 num_formats)
{
    LOGV("readAsync");
    OSCL_UNUSED_ARG(data);
    OSCL_UNUSED_ARG(max_data_len);
    OSCL_UNUSED_ARG(aContext);
    OSCL_UNUSED_ARG(formats);
    OSCL_UNUSED_ARG(num_formats);
    // This is an active data source. readAsync is not supported.
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

void AndroidCameraInput::readComplete(PVMFStatus aStatus,
        PVMFCommandId read_cmd_id,
        int32 format_index,
        const PvmiMediaXferHeader& data_header_info,
        OsclAny* aContext)
{
    LOGV("readComplete");
    OSCL_UNUSED_ARG(aStatus);
    OSCL_UNUSED_ARG(read_cmd_id);
    OSCL_UNUSED_ARG(format_index);
    OSCL_UNUSED_ARG(data_header_info);
    OSCL_UNUSED_ARG(aContext);
    // This is an active data source. readComplete is not supported.
    OSCL_LEAVE(OsclErrNotSupported);
    return;
}

void AndroidCameraInput::statusUpdate(uint32 status_flags)
{
    LOGV("statusUpdate");
    if (status_flags != PVMI_MEDIAXFER_STATUS_WRITE)
    {
        OSCL_LEAVE(OsclErrNotSupported);
    }
    else
    {
        // Restart the flow of data
        iWriteState = EWriteOK;
    }
}

void AndroidCameraInput::cancelCommand(PVMFCommandId aCmdId)
{
    LOGV("cancelCommand");
    OSCL_UNUSED_ARG(aCmdId);

    // This cancel command ( with a small "c" in cancel ) is for the media
    // transfer interface. implementation is similar to the cancel command
    // of the media I/O interface.
    OSCL_LEAVE(OsclErrNotSupported);
}

void AndroidCameraInput::cancelAllCommands()
{
    LOGV("cancelAllCommands");
    OSCL_LEAVE(OsclErrNotSupported);
}

void AndroidCameraInput::setObserver(
        PvmiConfigAndCapabilityCmdObserver* aObserver)
{
    LOGV("setObserver");
    OSCL_UNUSED_ARG(aObserver);
}

PVMFStatus AndroidCameraInput::getParametersSync(PvmiMIOSession session,
        PvmiKeyType identifier,
        PvmiKvp*& params,
        int& num_params,
        PvmiCapabilityContext context)
{
    LOGV("getParametersSync");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);

    params = NULL;
    num_params = 0;
    PVMFStatus status = PVMFFailure;

    if (!pv_mime_strcmp(identifier, OUTPUT_FORMATS_CAP_QUERY) ||
        !pv_mime_strcmp(identifier, OUTPUT_FORMATS_CUR_QUERY)) {
        num_params = 1;
        status = AllocateKvp(params, (PvmiKeyType)OUTPUT_FORMATS_VALTYPE, num_params);
        if (status != PVMFSuccess) {
            LOGE("AllocateKvp failed for OUTPUT_FORMATS_VALTYP");
            return status;
        }
#ifdef COMPRESS_INPUT
	if(false == bIsMp4)
	{
		params[0].value.pChar_value = (char*)ANDROID_VIDEO_FORMAT_H263;
	}
	else
	{
		params[0].value.pChar_value = (char*)ANDROID_VIDEO_FORMAT_M4V;
	}
#else        
        params[0].value.pChar_value = (char*)ANDROID_VIDEO_FORMAT;
#endif        
    } else if (!pv_mime_strcmp(identifier, VIDEO_OUTPUT_WIDTH_CUR_QUERY)) {
        num_params = 1;
        status = AllocateKvp(params, (PvmiKeyType)VIDEO_OUTPUT_WIDTH_CUR_VALUE, num_params);
        if (status != PVMFSuccess) {
            LOGE("AllocateKvp failed for VIDEO_OUTPUT_WIDTH_CUR_VALUE");
            return status;
        }
        params[0].value.uint32_value = mFrameWidth;
    } else if (!pv_mime_strcmp(identifier, VIDEO_OUTPUT_HEIGHT_CUR_QUERY)) {
        num_params = 1;
        status = AllocateKvp(params, (PvmiKeyType)VIDEO_OUTPUT_HEIGHT_CUR_VALUE, num_params);
        if (status != PVMFSuccess) {
            LOGE("AllocateKvp failed for VIDEO_OUTPUT_HEIGHT_CUR_VALUE");
            return status;
        }
        params[0].value.uint32_value = mFrameHeight;
    } else if (!pv_mime_strcmp(identifier, VIDEO_OUTPUT_FRAME_RATE_CUR_QUERY)) {
        num_params = 1;
        status = AllocateKvp(params,
            (PvmiKeyType)VIDEO_OUTPUT_FRAME_RATE_CUR_VALUE, num_params);
        if (status != PVMFSuccess) {
            LOGE("AllocateKvp failed for VIDEO_OUTPUT_FRAME_RATE_CUR_VALUE");
            return status;
        }
        params[0].value.float_value = mFrameRate;
    } else if (!pv_mime_strcmp(identifier, OUTPUT_TIMESCALE_CUR_QUERY)) {
        num_params = 1;
        status = AllocateKvp(params, (PvmiKeyType)OUTPUT_TIMESCALE_CUR_VALUE, num_params);
        if (status != PVMFSuccess) {
            LOGE("AllocateKvp failed for OUTPUT_TIMESCALE_CUR_VALUE");
            return status;
        }
        // TODO:
        // is it okay to hardcode this as the timescale?
        params[0].value.uint32_value = 1000;
    } else if (!pv_mime_strcmp(identifier, PVMF_BUFFER_ALLOCATOR_KEY)) {
        /*
         * if( camera MIO does NOT allocate YUV buffers )
         * {
         *      OSCL_LEAVE(OsclErrNotSupported);
         *      return PVMFErrNotSupported;
         * }
         */

        params = (PvmiKvp*)oscl_malloc(sizeof(PvmiKvp));
        if (!params )
        {
            OSCL_LEAVE(OsclErrNoMemory);
            return PVMFErrNoMemory;
        }
        params [0].value.key_specific_value = (PVInterface*)&mbufferAlloc;
        status = PVMFSuccess;
    	} else if (!pv_mime_strcmp(identifier, OUTPUT_BITRATE_CUR_QUERY)) {
        	num_params = 1;
        	status = AllocateKvp(params, (PvmiKeyType)OUTPUT_BITRATE_CUR_VALUE, num_params);
	       if (status != PVMFSuccess) {
       		LOGE("AllocateKvp failed for OUTPUT_TIMESCALE_CUR_VALUE");
            	return status;
        	}
        	params[0].value.uint32_value = mBitRate;
    }

    return status;
}

PVMFStatus AndroidCameraInput::releaseParameters(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements)
{
    LOGV("releaseParameters");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(num_elements);
    
    if (!parameters) {
        LOGE("parameters is a NULL pointer");
        return PVMFFailure;
    }
    iAlloc.deallocate((OsclAny*)parameters);
    return PVMFSuccess;
}

void AndroidCameraInput::createContext(PvmiMIOSession session,
        PvmiCapabilityContext& context)
{
    LOGV("createContext");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
}

void AndroidCameraInput::setContextParameters(PvmiMIOSession session,
        PvmiCapabilityContext& context,
        PvmiKvp* parameters,
        int num_parameter_elements)
{
    LOGV("setContextParameters");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
    OSCL_UNUSED_ARG(parameters);
    OSCL_UNUSED_ARG(num_parameter_elements);
}

void AndroidCameraInput::DeleteContext(PvmiMIOSession session,
        PvmiCapabilityContext& context)
{
    LOGV("DeleteContext");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
}

void AndroidCameraInput::setParametersSync(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements,
        PvmiKvp*& ret_kvp)
{
    LOGV("setParametersSync");
    OSCL_UNUSED_ARG(session);
    PVMFStatus status = PVMFSuccess;
    ret_kvp = NULL;
    for (int32 i = 0; i < num_elements; ++i) {
        status = VerifyAndSetParameter(&(parameters[i]), true);
        if (status != PVMFSuccess) {
            LOGE("VerifiyAndSetParameter failed on parameter #%d", i);
            ret_kvp = &(parameters[i]);
            OSCL_LEAVE(OsclErrArgument);
            return;
        }
    }
}

PVMFCommandId AndroidCameraInput::setParametersAsync(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements,
        PvmiKvp*& ret_kvp,
        OsclAny* context)
{
    LOGV("setParametersAsync");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(parameters);
    OSCL_UNUSED_ARG(num_elements);
    OSCL_UNUSED_ARG(ret_kvp);
    OSCL_UNUSED_ARG(context);
    OSCL_LEAVE(OsclErrNotSupported);
    return -1;
}

uint32 AndroidCameraInput::getCapabilityMetric (PvmiMIOSession session)
{
    LOGV("getCapabilityMetric");
    OSCL_UNUSED_ARG(session);
    return 0;
}

PVMFStatus AndroidCameraInput::verifyParametersSync(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements)
{
    LOGV("verifyParametersSync");
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(parameters);
    OSCL_UNUSED_ARG(num_elements);
    return PVMFErrNotSupported;
}


void AndroidCameraInput::SetFrameSize(int w, int h)
{
    LOGV("SetFrameSize");
    if (iState != STATE_IDLE) {
        LOGE("SetFrameSize called in an invalid state(%d)", iState);
        return;
    }

    mFrameWidth = w;
    mFrameHeight = h;
    FrameSizeChanged();
}

void AndroidCameraInput::SetFrameRate(int fps)
{
    LOGV("SetFrameRate");
    if (iState != STATE_IDLE) {
        LOGE("SetFrameRate called in an invalid state(%d)", iState);
        return;
    }

    mFrameRate = (float)fps;
}

void AndroidCameraInput::SetBitRate(int bitrate)
{
    LOGV ("SetBitRate=%d", bitrate);
    if (iState != STATE_IDLE) {
        LOGE("SetBitRate called in an invalid state(%d)", iState);
        return;
    }

    mBitRate = bitrate;
}

void AndroidCameraInput::FrameSizeChanged()
{
    LOGV("FrameSizeChanged");
    if (iState != STATE_IDLE) {
        LOGE("FrameSizeChanged called in an invalid state(%d)", iState);
        return;
    }

    // Reinitialize the preview surface in case it was set up before now
    if (mSurface != NULL) {
        SetPreviewSurface(mSurface);
    }
}
// Start modify by Mingliang.Zhong@2010-09-14(ISSUE:alps00121109)
void AndroidCameraInput::Check1stVideoFrameWrite( CameraInputMIONotify* pCallback,void*p)
{
	iCheck1stVideoFrameWriteParams = p;
	ipCameraInputMIONotify = pCallback;
	if(i1stFramesend)
	{
		LOGE("#@# Can Stop,Notify AuthorDriver!");
		ipCameraInputMIONotify->_CameraInputMIOCallback(iCheck1stVideoFrameWriteParams);
	}else
	{
 		iCheck1stVideoFrameWrite = true;
	}
}
//end  to modify by Mingliang.Zhong@2010-09-14(ISSUE:alps00121109)

void AndroidCameraInput::Run()
{
    LOGV("Run");

    // dequeue frame buffers and write to peer
    if (NULL != iPeer) {
    	  if (iState != STATE_STARTED) {
            ReleaseQueuedFrames();
        }
        iFrameQueueMutex.Lock();
        while (!iFrameQueue.empty()) {
            AndroidCameraInputMediaData data = iFrameQueue[0];

            uint32 writeAsyncID = 0;
            OsclLeaveCode error = OsclErrNone;
            uint8 *ptr = (uint8*) (data.iFrameBuffer->pointer());
            if (ptr) {
                OSCL_TRY(error,writeAsyncID = iPeer->writeAsync(PVMI_MEDIAXFER_FMT_TYPE_DATA, 0, ptr,
                            data.iFrameSize, data.iXferHeader););
//Start modify by Mingliang.Zhong@2010-09-14(ISSUE:alps00121109)
			iFrameCounter++;
			if(iFrameCounter >1)
			{
				i1stFramesend = true;
				//LOGE("#@# writeAsync!");
			}
			if(iCheck1stVideoFrameWrite && i1stFramesend)
			{
				LOGE("#@# Ready to Stop,Notify AuthorDriver!");
				if(ipCameraInputMIONotify)
					ipCameraInputMIONotify->_CameraInputMIOCallback(iCheck1stVideoFrameWriteParams);
					iCheck1stVideoFrameWrite = false;					
			}
//End to modify by Mingliang.Zhong@2010-09-14(ISSUE:alps00121109)
            } else {
                //FIXME Check why camera sends NULL frames
                LOGE("Ln %d ERROR null pointer", __LINE__);
                error = OsclErrBadHandle;
            }

            if (OsclErrNone == error) {
                iFrameQueue.erase(iFrameQueue.begin());
                data.iId = writeAsyncID;
                iSentMediaData.push_back(data);
		if(!iFrameQueue.empty())
		{
		   RunIfNotReady();
		}
		break;
                LOGV("Ln %d Run writeAsync writeAsyncID %d", __LINE__, writeAsyncID);
            } else {
                //FIXME resend the frame later if ( OsclErrBusy == error)
                LOGE("Ln %d Run writeAsync error %d", __LINE__, error);
                //release buffer immediately if write fails
                mCamera->releaseRecordingFrame(data.iFrameBuffer);
                iFrameQueue.erase(iFrameQueue.begin());
                iWriteState = EWriteBusy;
                break;
            }
        }
        iFrameQueueMutex.Unlock();
    }

    PVMFStatus status = PVMFFailure;

    if (!iCmdQueue.empty()) {
        AndroidCameraInputCmd cmd = iCmdQueue[0];

	//start to modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
	if( (cmd.iType == CMD_FLUSH) && ( i1stFramesend == false))
	{
		LOGE("#@# wait for write 1st video frame to composer before stop!");
		return;
	}
	//end to modify  by zhongmingliang @2010-09-14(ISSUE:alps00121109)

        iCmdQueue.erase(iCmdQueue.begin());

        switch(cmd.iType) {

        case CMD_INIT:
            status = DoInit();
            break;

        case CMD_START:
            status = DoStart();
            break;

        case CMD_PAUSE:
            status = DoPause();
            break;

        case CMD_FLUSH:
            status = DoFlush(cmd);
            break;

        case CMD_RESET:
            status = DoReset();
            break;

        case CMD_STOP:
            status = DoStop(cmd);
            break;

        case DATA_EVENT:
            // this is internal only - don't send RequestCompleted
            DoRead();
            status = PVMFPending;
            break;

        case CMD_QUERY_UUID:
        case CMD_QUERY_INTERFACE:
            status = PVMFSuccess;
            break;

        // these commands all fail
        case CMD_CANCEL_ALL_COMMANDS:
        case CMD_CANCEL_COMMAND:
        default:
            break;
        }
        // do RequestCompleted unless command is still pending
        if (status != PVMFPending) {
            DoRequestCompleted(cmd, status);
        }
    }

    if (!iCmdQueue.empty()) {
        // Run again if there are more things to process
        RunIfNotReady();
    }
}

PVMFCommandId AndroidCameraInput::AddCmdToQueue(AndroidCameraInputCmdType aType,
        const OsclAny* aContext,
        OsclAny* aData)
{
    LOGV("AddCmdToQueue");
    if (aType == DATA_EVENT) {
        LOGE("Invalid argument");
        OSCL_LEAVE(OsclErrArgument);
        return -1;
    }

    AndroidCameraInputCmd cmd;
    cmd.iType = aType;
    cmd.iContext = OSCL_STATIC_CAST(OsclAny*, aContext);
    cmd.iData = aData;
    cmd.iId = iCmdIdCounter;
    ++iCmdIdCounter;

    // TODO:
    // Check against out of memory failure
    int err = 0;
    OSCL_TRY(err, iCmdQueue.push_back(cmd));
    OSCL_FIRST_CATCH_ANY(err, LOGE("Out of memory"); return -1;);
    RunIfNotReady();
    return cmd.iId;
}

void AndroidCameraInput::AddDataEventToQueue(uint32 aMicroSecondsToEvent)
{
    LOGV("AddDataEventToQueue");
    AndroidCameraInputCmd cmd;
    cmd.iType = DATA_EVENT;

    int err = 0;
    OSCL_TRY(err, iCmdQueue.push_back(cmd));
    OSCL_FIRST_CATCH_ANY(err, LOGE("Out of memory"); return;);
    RunIfNotReady(aMicroSecondsToEvent);
}

void AndroidCameraInput::DoRequestCompleted(const AndroidCameraInputCmd& aCmd, PVMFStatus aStatus, OsclAny* aEventData)
{
    LOGV("DoRequestCompleted");
    PVMFCmdResp response(aCmd.iId, aCmd.iContext, aStatus, aEventData);

    for (uint32 i = 0; i < iObservers.size(); i++) {
        iObservers[i]->RequestCompleted(response);
    }
}

PVMFStatus AndroidCameraInput::DoInit()
{
    LOGV("DoInit()");
    iState = STATE_INITIALIZED;
    iMilliSecondsPerDataEvent = (int32)(1000 / mFrameRate);
    iMicroSecondsPerDataEvent = (int32)(1000000 / mFrameRate);
    iDataEventCounter = 0;
    u4VosSaveCnt = 0;
    bVosOutputDone = 0;
    bVopNeedSkip = 0;        
    bVosSaved = 0;
#ifdef COMPRESS_INPUT
    bCompressVideoStart = false;    
    bCompressVideoReady = false;
    u4NotReadyCnt = 0;
#endif                
    // create a camera if the app didn't supply one
    if (mCamera == 0) {
        mCamera = Camera::connect(0);  //Donglei for Android2.3 opencore migration
    }

    // always call setPreviewDisplay() regardless whether mCamera is just created or not
    // return failure if no display surface is available
    if (mCamera != NULL && mSurface != NULL) {
        mCamera->setPreviewDisplay(mSurface);
    } else {
        if (mCamera == NULL) {
            LOGE("Camera is not available");
        } else if (mSurface == NULL) {
            LOGE("No surface is available for display");
        }
        return PVMFFailure;
    }

    LOGD("Intended mFrameWidth=%d, mFrameHeight=%d ",mFrameWidth, mFrameHeight);
    String8 s = mCamera->getParameters();
    if (s.length() == 0) {
        LOGE("Failed to get camera(%p) parameters", mCamera.get());
        return PVMFFailure;
    }
    CameraParameters p(s);
    p.setPreviewSize(mFrameWidth, mFrameHeight);
    p.setPreviewFrameRate(mFrameRate);
    s = p.flatten();
    if (mCamera->setParameters(s) != NO_ERROR) {
        LOGE("Failed to set camera(%p) parameters", mCamera.get());
        return PVMFFailure;
    }

    // Since we may not honor the preview size that app has requested
    // It is a good idea to get the actual preview size and used it
    // for video recording.
    CameraParameters newCameraParam(mCamera->getParameters());
    int32 width, height;
    newCameraParam.getPreviewSize(&width, &height);
    if (width < 0 || height < 0) {
        LOGE("Failed to get camera(%p) preview size", mCamera.get());
        return PVMFFailure;
    }
    if (width != mFrameWidth || height != mFrameHeight) {
        LOGE("Mismatch between the intended frame size (%dx%d) and the available frame size (%dx%d)", mFrameWidth, mFrameHeight, width, height);
        return PVMFFailure;
    }
    LOGD("Actual mFrameWidth=%d, mFrameHeight=%d ",mFrameWidth, mFrameHeight);
    if (mCamera->startPreview() != NO_ERROR) {
    LOGE("Failed to start camera(%p) preview", mCamera.get());
        return PVMFFailure;
    }
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::DoStart()
{
    LOGV("DoStart");
#ifdef SAVE_BUF_TIME
    out_fp = NULL;
    if(NULL == (out_fp = fopen("/sdcard/fps_out.txt", "wt")))
	  {
		    LOGE("error open out file");
	  }
#endif
    iAudioFirstFrameTs = 0;
    // Set the clock state observer
    if (iAuthorClock) {
        iAuthorClock->ConstructMediaClockNotificationsInterface(iClockNotificationsInf, *this);

        if (iClockNotificationsInf == NULL) {
             return PVMFErrNoMemory;
        }

        iClockNotificationsInf->SetClockStateObserver(*this);
    }

    PVMFStatus status = PVMFFailure;
    iWriteState = EWriteOK;
    if (mCamera == NULL) {
        status = PVMFFailure;
        LOGE("mCamera is not initialized yet");
    } else {
        mCamera->setListener(mListener);
#ifdef COMPRESS_INPUT      
		if(false == bCompressVideoHasAudio)
		{
        if (mCamera->startRecording() != NO_ERROR)
        {
            LOGE("mCamera start recording failed");
            status = PVMFFailure;
        } 
        else         
        {
            iState = STATE_STARTED;
		 i1stFramesend = false; //modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
		 iFrameCounter = 0;//modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
            status = PVMFSuccess;            
        }
    }
		else
		{
		    mCameraExtra = mCamera;
	        iState = STATE_STARTED;
		   i1stFramesend = false;//modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
		   iFrameCounter = 0;//modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
	        status = PVMFSuccess; 	        
	        bCompressVideoStart = true;
	        LOGD("wait for audio MIO trigger mCamera start recording...");    
		}
#else
        if (mCamera->startRecording() != NO_ERROR)
        {
            LOGE("mCamera start recording failed");
            status = PVMFFailure;
        } 
        else         
        {
            iState = STATE_STARTED;
		i1stFramesend = false;//modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
		iFrameCounter = 0;//modify by zhongmingliang @2010-09-14(ISSUE:alps00121109)
            status = PVMFSuccess;            
        }
#endif
    }
    
    AddDataEventToQueue(iMilliSecondsPerDataEvent);
    return status;
}

PVMFStatus AndroidCameraInput::DoPause()
{
    LOGV("DoPause");
    iState = STATE_PAUSED;
    return PVMFSuccess;
}

// Does this work for reset?
PVMFStatus AndroidCameraInput::DoReset()
{
    LOGD("DoReset: E");
    // Remove and destroy the clock state observer
    RemoveDestroyClockObs();
    iDataEventCounter = 0;
    iAudioLossDuration = 0;
    u4VosSaveCnt = 0;
    bVosOutputDone = 0;
    bVopNeedSkip = 0;
    bVosSaved = 0;
    mCameraExtra = 0;
    bCompressVideoStart = false;
    iWriteState = EWriteOK;
    if ( (iState == STATE_STARTED) || (iState == STATE_PAUSED) ) {
    if (mCamera != NULL) {
        mCamera->setListener(NULL);
        mCamera->stopRecording();
        ReleaseQueuedFrames();
    }
    }
    while(!iCmdQueue.empty())
    {
        AndroidCameraInputCmd cmd = iCmdQueue[0];
        iCmdQueue.erase(iCmdQueue.begin());
    }
    Cancel();
    iState = STATE_IDLE;
    LOGD("DoReset: X");
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::DoFlush(const AndroidCameraInputCmd& aCmd)
{
    LOGV("DoFlush");
    // This method should stop capturing media data but continue to send
    // captured media data that is already in buffer and then go to
    // stopped state.
    // However, in this case of file input we do not have such a buffer for
    // captured data, so this behaves the same way as stop.
    return DoStop(aCmd);
}

PVMFStatus AndroidCameraInput::DoStop(const AndroidCameraInputCmd& aCmd)
{
    LOGD("DoStop: E");

    // Remove and destroy the clock state observer
    RemoveDestroyClockObs();
		if(0 != iTimeStamp)
		{
			LOGD("DoStop, iDataEventCounter:%d, iTimeStamp:%u, fps:%f", iDataEventCounter-1, iTimeStamp, ((float) (iDataEventCounter-1))/((float) iTimeStamp/1000.0));
		}
    iDataEventCounter = 0;
    mCameraExtra = 0;
    bCompressVideoStart = false;
    iWriteState = EWriteOK;
    if (mCamera != NULL) {
		    mCamera->setListener(NULL);
		    mCamera->stopRecording();
		    ReleaseQueuedFrames();
    }
    iState = STATE_STOPPED;
#ifdef SAVE_BUF_TIME
    fclose(out_fp);
#endif
    LOGD("DoStop: X");
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::DoRead()
{
    LOGV("DoRead");
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::AllocateKvp(PvmiKvp*& aKvp,
        PvmiKeyType aKey,
        int32 aNumParams)
{
    LOGV("AllocateKvp");
    uint8* buf = NULL;
    uint32 keyLen = oscl_strlen(aKey) + 1;
    int32 err = 0;

    OSCL_TRY(err,
        buf = (uint8*)iAlloc.allocate(aNumParams * (sizeof(PvmiKvp) + keyLen));
        if (!buf) {
            LOGE("Failed to allocate memory to Kvp");
            OSCL_LEAVE(OsclErrNoMemory);
        }
    );
    OSCL_FIRST_CATCH_ANY(err,
        LOGE("kvp allocation failed");
        return PVMFErrNoMemory;
    );

    PvmiKvp* curKvp = aKvp = new (buf) PvmiKvp;
    buf += sizeof(PvmiKvp);
    for (int32 i = 1; i < aNumParams; ++i) {
        curKvp += i;
        curKvp = new (buf) PvmiKvp;
        buf += sizeof(PvmiKvp);
    }

    for (int32 i = 0; i < aNumParams; ++i) {
        aKvp[i].key = (char*)buf;
        oscl_strncpy(aKvp[i].key, aKey, keyLen);
        buf += keyLen;
    }

    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::VerifyAndSetParameter(PvmiKvp* aKvp,
        bool aSetParam)
{
    LOGV("VerifyAndSetParameter");

    if (!aKvp) {
        LOGE("Invalid key-value pair");
        return PVMFFailure;
    }

    if (!pv_mime_strcmp(aKvp->key, OUTPUT_FORMATS_VALTYPE)) {
#ifdef COMPRESS_INPUT
    if(pv_mime_strcmp(aKvp->value.pChar_value, ANDROID_VIDEO_FORMAT_H263) == 0
    	 || pv_mime_strcmp(aKvp->value.pChar_value, ANDROID_VIDEO_FORMAT_M4V) == 0
    	 ) {
            return PVMFSuccess;
        } else  {
            LOGE("Unsupported format %d", aKvp->value.uint32_value);
            return PVMFFailure;
        }
#else
    if(pv_mime_strcmp(aKvp->value.pChar_value, ANDROID_VIDEO_FORMAT) == 0) {
            return PVMFSuccess;
        } else  {
            LOGE("Unsupported format %d", aKvp->value.uint32_value);
            return PVMFFailure;
        }
#endif
    }
    else if (pv_mime_strcmp(aKvp->key, PVMF_AUTHORING_CLOCK_KEY) == 0)
    {
        LOGV("AndroidCameraInput::VerifyAndSetParameter() PVMF_AUTHORING_CLOCK_KEY value %p", aKvp->value.key_specific_value);
        if( (NULL == aKvp->value.key_specific_value) && ( iAuthorClock ) )
        {
            RemoveDestroyClockObs();
        }
        iAuthorClock = (PVMFMediaClock*)aKvp->value.key_specific_value;
        return PVMFSuccess;
    }

    LOGE("Unsupported parameter(%s)", aKvp->key);
    return PVMFFailure;
}

void AndroidCameraInput::SetPreviewSurface(const sp<android::ISurface>& surface)
{
    LOGV("SetPreviewSurface");
    mSurface = surface;

    if (mCamera != NULL) {
        mCamera->setPreviewDisplay(surface);
    }
}

PVMFStatus AndroidCameraInput::SetCamera(const sp<android::ICamera>& camera)
{
    LOGV("SetCamera");
    mFlags &= ~ FLAGS_SET_CAMERA | FLAGS_HOT_CAMERA;
    if (camera == NULL) {
        LOGV("camera is NULL");
        return PVMFErrArgument;
    }

    // Connect our client to the camera remote
    mCamera = Camera::create(camera);
    if (mCamera == NULL) {
        LOGE("Unable to connect to camera");
        return PVMFErrNoResources;
    }

    LOGV("Connected to camera");
    mFlags |= FLAGS_SET_CAMERA;
    if (mCamera->previewEnabled()) {
        mFlags |= FLAGS_HOT_CAMERA;
        LOGV("camera is hot");
    }
    return PVMFSuccess;
}

PVMFStatus AndroidCameraInput::postWriteAsyncSaveVos(const sp<IMemory>& frame)
{
	//VosBuf[u4VosSaveCnt] = NULL;
	VosBuf[u4VosSaveCnt] = frame;
u4VosSaveCnt ++;
LOGD("SAVE VOS:%d", u4VosSaveCnt);
	return PVMFSuccess;
}
PVMFStatus AndroidCameraInput::postWriteAsyncOutputVos()
{
	sp<IMemory>             frame = VosBuf[u4VosSaveCnt-1];
	    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;
   // if (!oscl_pmem_register(&rInfo))
    //{
      //  LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rInfo.base, rInfo.offset);
   // }

    // queue data to be sent to peer
    AndroidCameraInputMediaData data;
    data.iXferHeader.seq_num = iDataEventCounter++;
    data.iXferHeader.timestamp = 0; //iTimeStamp;
    data.iXferHeader.flags = 0;
    data.iXferHeader.duration = 0;
    data.iXferHeader.stream_id = 0;

    {//compose private data
        //could size be zero?
        if(NULL == pPmemInfo)
        {
            int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / size;
            LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), size, iCalculateNoOfCameraPreviewBuffer);
            pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
            if(NULL == pPmemInfo)
            {
                LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
                return PVMFFailure;
            }
        }

        int iIndex = offset / size;
        pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
        pPmemInfo[iIndex].offset = offset;
        data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
        LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
    }

    data.iFrameBuffer = frame;
    data.iFrameSize = size;
#ifdef COMPRESS_INPUT
    LOGD("hi, cur_vos offset:%d, header:%08x ", offset, *((uint32*)(((char*)rInfo.base)+offset)) );
    data.iFrameSize = *((uint32*)( (((char *)rInfo.base)+offset)+size-4));
    LOGD("DATA SIZE:%d, %08x", data.iFrameSize, *((uint32*)( (((char *)rInfo.base)+offset)+size-4)));
#endif
	
	if(u4VosSaveCnt > 1)
	{
		sp<IMemory>             Srcframe = VosBuf[0];
	  LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
	  // get memory offset for frame buffer
	  ssize_t src_offset = 0;
	  size_t src_size = 0;
	  sp<IMemoryHeap> heap = Srcframe->getMemory(&src_offset, &src_size);
	  LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), src_offset, src_size, Srcframe->pointer());
	
	  ExPmemInfo rSrcInfo;
	  rSrcInfo.base = heap->base();
	  rSrcInfo.size = src_size;
	  rSrcInfo.shared_fd = heap->heapID();
	  rSrcInfo.offset = src_offset;
	 // if (!oscl_pmem_register(&rSrcInfo))
	 // {
	    //  LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rSrcInfo.base, rSrcInfo.offset);
	 // }
	
	  // queue data to be sent to peer
	  AndroidCameraInputMediaData src_data;
	  src_data.iXferHeader.seq_num = 0;//iDataEventCounter++;
	  src_data.iXferHeader.timestamp = 0;//iTimeStamp;
	  src_data.iXferHeader.flags = 0;
	  src_data.iXferHeader.duration = 0;
	  src_data.iXferHeader.stream_id = 0;
	
	  {//compose private data
	      //could size be zero?
	      if(NULL == pPmemInfo)
	      {
	          int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / src_size;
	          LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), src_size, iCalculateNoOfCameraPreviewBuffer);
	          pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
	          if(NULL == pPmemInfo)
	          {
	              LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
	              return PVMFFailure;
	          }
	      }
	
	      int iIndex = src_offset / src_size;
	      pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
	      pPmemInfo[iIndex].offset = src_offset;
	      src_data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
	      LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
	  }
	
	  src_data.iFrameBuffer = Srcframe;
	  src_data.iFrameSize = src_size;
		#ifdef COMPRESS_INPUT
		/*
		  if (iDataEventCounter == 0) 
		  {
		  FILE *fp = fopen("/first.m4v", "wb");
		  fwrite(((char*)rInfo.base+offset), 1, 16*1024);
		  fclose(fp);
		  fp = fopen("/first.m4v", "wb");
		  fwrite(((char*)rInfo.base+offset), 1, 16*1024);
		  fclose(fp);
		  }
		*/
		  LOGD("hi, src offset:%d, header:%08x ", src_offset, *((uint32*)(((char*)rSrcInfo.base)+src_offset)) );
		  src_data.iFrameSize = *((uint32*)( (((char *)rSrcInfo.base)+src_offset)+size-4));
		  LOGD("DATA SIZE:%d, %08x", src_data.iFrameSize, *((uint32*)( (((char *)rSrcInfo.base)+src_offset)+size-4)));
		#endif
		memcpy((((char*)rInfo.base)+offset), (((char*)rSrcInfo.base)+src_offset), src_data.iFrameSize);
		*((uint32*)( (((char *)rInfo.base)+offset)+size-4)) = *((uint32*)( (((char *)rSrcInfo.base)+src_offset)+size-4));	
		data.iFrameSize = src_data.iFrameSize;
		bVopNeedSkip = 1;	
	}
//LOGD("postWriteAsync, SIZE:%d", size);
    // lock mutex and queue frame buffer
    iFrameQueueMutex.Lock();
    iFrameQueue.push_back(data);
    iFrameQueueMutex.Unlock();
    RunIfNotReady();    
	bVosOutputDone = 1;
	
	for(int i=0; i<u4VosSaveCnt-1; i++)
	{
		LOGD("VOS release:%d", i);
		mCamera->releaseRecordingFrame(VosBuf[i]);
	}
	return PVMFSuccess;
}
PVMFStatus AndroidCameraInput::postWriteAsyncSkipVop(const sp<IMemory>& frame)
{
	bool bSkip = 0;
	    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;

if(0 == *((char *)( (((char *)rInfo.base)+offset)+size-5)))
	{
		LOGD("VOP SKIP, offset:%d", offset);
		mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
	}
	else
		{
			LOGD("VOP SKIP, got key frame");
			bVopNeedSkip = 0;
			return PVMFFailure;
		}

	return PVMFSuccess;
}
PVMFStatus AndroidCameraInput::SaveVos(const sp<IMemory>& frame)
{
	
	    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;
    /*
    if (!oscl_pmem_register(&rInfo))
    {
        LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rInfo.base, rInfo.offset);
    }
    */

    // queue data to be sent to peer
    AndroidCameraInputMediaData data;
    data.iXferHeader.seq_num = 0; //iDataEventCounter++;
    data.iXferHeader.timestamp = 0; //iTimeStamp;
    data.iXferHeader.flags = 0;
    data.iXferHeader.duration = 0;
    data.iXferHeader.stream_id = 0;
    /*
    {//compose private data
        //could size be zero?
        if(NULL == pPmemInfo)
        {
            int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / size;
            LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), size, iCalculateNoOfCameraPreviewBuffer);
            pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
            if(NULL == pPmemInfo)
            {
                LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
                return PVMFFailure;
            }
        }

        int iIndex = offset / size;
        pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
        pPmemInfo[iIndex].offset = offset;
        data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
        LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
    }
    */
    data.iFrameBuffer = frame;
    data.iFrameSize = size;
#ifdef COMPRESS_INPUT
    LOGD("hi, cur_vos offset:%d, header:%08x ", offset, *((uint32*)(((char*)rInfo.base)+offset)) );
    data.iFrameSize = *((uint32*)( (((char *)rInfo.base)+offset)+size-4));
    LOGD("DATA SIZE:%d, %08x", data.iFrameSize, *((uint32*)( (((char *)rInfo.base)+offset)+size-4)));
#endif

	memcpy(cVosData, (((char*)rInfo.base)+offset), data.iFrameSize);
	u4VosLen = data.iFrameSize;
    bVosSaved = 1;
    bVopNeedSkip = 1;
    LOGD("Vos saved!");
	return PVMFSuccess;
}


#ifdef COMPRESS_INPUT
PVMFStatus AndroidCameraInput::postWriteAsync(nsecs_t timestamp, const sp<IMemory>& frame)
{
    LOGV("postWriteAsync");

    if (frame == NULL) {
        LOGE("frame is a NULL pointer");
        return PVMFFailure;
    }
    
    if (0 == bVosSaved)
    {
        SaveVos(frame);
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    }    

      
    if((!iPeer) || (!isRecorderStarting()) || (iWriteState == EWriteBusy) || (NULL == iAuthorClock) 
        || (iAuthorClock->GetState() != PVMFMediaClock::RUNNING)
            ) 
    {
        if( NULL == iAuthorClock )
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iAuthorClock NULL), frame dropped", iPeer, iState, iWriteState);
        }
        else
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iClockState %d), frame dropped", iPeer, iState, iWriteState, iAuthorClock->GetState());
        } 
        u4NotReadyCnt++;
        if(NULL != iAuthorClock && iAuthorClock->GetState() != PVMFMediaClock::RUNNING && u4NotReadyCnt > 120)
        {
        	dump_ccci_reg();
        	dump_md_image();
        	u4NotReadyCnt = 0;
        }
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    }

    if(1 == bVopNeedSkip)
    {
	
        if(PVMFSuccess == postWriteAsyncSkipVop(frame))
        {
        	return PVMFSuccess;
        }
    }

    bCompressVideoReady = true;
    
    u4NotReadyCnt = 0;
#if 1
    // if first event, set timestamp to zero
    if (iDataEventCounter == 0) {
        iStartTickCount = timestamp;
        iTimeStamp = 0;
        iAVTSOffset = 0;
        gettimeofday(&iVideoFirstFrameTs, NULL);
        LOGV("First event, set timestamp to zero, timestamp:%lld", timestamp);
    }
    
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp    
    uint32 ts = (uint32)((timestamp-iStartTickCount) / 1000000LL);
    ts += iAVTSOffset;
    //LOGD("ts:%d, iAudioFirstFrameTs:%d", ts, iAudioFirstFrameTs);

    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if ((iDataEventCounter == 0) && (iAudioFirstFrameTs == 0))
    {
        iAudioFirstFrameTs = ts;
        LOGD("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }   
    else if(iDataEventCounter == 0 && ts < iAudioFirstFrameTs)
    {
        iAVTSOffset = iAudioFirstFrameTs - ts;
        //iAVTSOffset = iAudioFirstFrameTs;
        ts += iAVTSOffset;
        LOGD("ts < iAudioFirstFrameTs, update iAVTSOffset:%d, systime:%lld", iAVTSOffset, systemTime()/1000000LL);
        
    }
#else
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp
    uint32 ts = (uint32)(timestamp / 1000000L);    


    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if (iAudioFirstFrameTs == 0)
    {
        iAudioFirstFrameTs = ts;
        LOGD("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }
#endif

    iAudioLossMutex.Lock();
    if (ts < iAudioFirstFrameTs + iAudioLossDuration) {
        LOGE("postWriteAsync,Drop the frame, ts:%d ,iAudioFirstFrameTs:%d ",ts , iAudioFirstFrameTs);
        // Drop the frame
        iAudioLossMutex.Unlock();
        bVopNeedSkip = 1;
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    } else {
         // calculate timestamp as offset from start time plus lost audio
         ts -= (iAudioFirstFrameTs + iAudioLossDuration);
    }
    iAudioLossMutex.Unlock();
    
    // Determine how much video to drop so when we resync to the audio
    // time we don't go into old video (we need increasing timestamps)
    if (0 != iTimeStamp && ts <= iTimeStamp) {
        LOGD("Dropping video frame to catch up for audio ts %lu, dropUntil %lu", ts, iTimeStamp);
        bVopNeedSkip = 1;
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    }
    
    // Make sure that no two samples have the same timestamp
    if (iDataEventCounter != 0) {
        if (iTimeStamp != ts) {
            iTimeStamp = ts;
        } else {
            ++iTimeStamp;
        }
    }
    
    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;
   // if (!oscl_pmem_register(&rInfo))
   // {
      //  LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rInfo.base, rInfo.offset);
   // }

    // queue data to be sent to peer
    AndroidCameraInputMediaData data;
    data.iXferHeader.seq_num = iDataEventCounter++;
    data.iXferHeader.timestamp = iTimeStamp;
    data.iXferHeader.flags = 0;
    data.iXferHeader.duration = 0;
    data.iXferHeader.stream_id = 0;
    data.iFrameSize = size;
    data.iFrameBuffer = frame;

    // lock muteddx and queue frame buffer
    iFrameQueueMutex.Lock();
    
    {//compose private data
        //could size be zero?
        if(NULL == pPmemInfo)
        {
            int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / size;
            LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), size, iCalculateNoOfCameraPreviewBuffer);
            pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
            if(NULL == pPmemInfo)
            {
                LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
                iFrameQueueMutex.Unlock();
                return PVMFFailure;
            }
        }

        int iIndex = offset / size;
        pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
        pPmemInfo[iIndex].offset = offset;
        data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
        LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
        if(0 == bVosOutputDone)
        {
            data.iXferHeader.vos_data_ptr = ((OsclAny*) cVosData);
            data.iXferHeader.vos_data_length = u4VosLen;
            bVosOutputDone = 1;
        }
        data.iXferHeader.iEndOfFrameFlagOut = 1;
        data.iXferHeader.iKeyFrameFlagOut = *((char *)( (((char *)rInfo.base)+offset)+size-5));
        
    }


#ifdef COMPRESS_INPUT
    //LOGD("hi, offset:%d, header:%08x ", offset, *((uint32*)(((char*)rInfo.base)+offset)) );
    data.iFrameSize = *((uint32*)( (((char *)rInfo.base)+offset)+size-4));
    LOGV("DATA SIZE:%d, %08x", data.iFrameSize, *((uint32*)( (((char *)rInfo.base)+offset)+size-4)));
#endif
//LOGD("postWriteAsync, SIZE:%d", size);    
   
    iFrameQueue.push_back(data);
    iFrameQueueMutex.Unlock();
    RunIfNotReady();

    return PVMFSuccess; 
}
#elif 0
PVMFStatus AndroidCameraInput::postWriteAsync(nsecs_t timestamp, const sp<IMemory>& frame)
{
    LOGV("postWriteAsync");

    if (frame == NULL) {
        LOGE("frame is a NULL pointer");
        return PVMFFailure;
    }
    

    if (0 == bVosOutputDone)
    {
        postWriteAsyncSaveVos(frame);
    }    
	
    if((!iPeer) || (!isRecorderStarting()) || (iWriteState == EWriteBusy) || (NULL == iAuthorClock) 
    	|| (iAuthorClock->GetState() != PVMFMediaClock::RUNNING)
    		) 
    {
        if( NULL == iAuthorClock )
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iAuthorClock NULL), frame dropped", iPeer, iState, iWriteState);
        }
        else
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iClockState %d), frame dropped", iPeer, iState, iWriteState, iAuthorClock->GetState());
        } 

        if (0 == bVosOutputDone)
        {        	  
            return PVMFSuccess;
        } 
        //bVopNeedSkip = 1; 
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    }

    if (0 == bVosOutputDone)
    {
        postWriteAsyncOutputVos();
        return PVMFSuccess;
    }

    if(1 == bVopNeedSkip)
    {
        if(PVMFSuccess == postWriteAsyncSkipVop(frame))
        {
        	return PVMFSuccess;
        }
    }
		

#if 1
    // if first event, set timestamp to zero
    if (iDataEventCounter == 1) {
        iStartTickCount = timestamp;
        iTimeStamp = 0;
        iAVTSOffset = 0;
        LOGV("First event, set timestamp to zero, timestamp:%lld", timestamp);
    }
    
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp    
    uint32 ts = (uint32)((timestamp-iStartTickCount) / 1000000LL);
    ts += iAVTSOffset;
    //LOGD("ts:%d, iAudioFirstFrameTs:%d", ts, iAudioFirstFrameTs);

    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if (iAudioFirstFrameTs == 0)
    {
        iAudioFirstFrameTs = ts;
        LOGV("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }   
    else if(iDataEventCounter == 1 && ts < iAudioFirstFrameTs)
    {
        iAVTSOffset = iAudioFirstFrameTs - ts;
        //iAVTSOffset = iAudioFirstFrameTs;
        ts += iAVTSOffset;
        LOGV("ts < iAudioFirstFrameTs, update iAVTSOffset:%d", iAVTSOffset);
    }
#else
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp
    uint32 ts = (uint32)(timestamp / 1000000L);    


    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if (iAudioFirstFrameTs == 0)
    {
        iAudioFirstFrameTs = ts;
        LOGD("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }
#endif
    if (ts < iAudioFirstFrameTs) {
        LOGE("postWriteAsync,Drop the frame, ts:%d ,iAudioFirstFrameTs:%d ",ts , iAudioFirstFrameTs);
        // Drop the frame
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    } else {
         // calculate timestamp as offset from start time
         ts -= iAudioFirstFrameTs;
    }

    // Make sure that no two samples have the same timestamp
    if (iDataEventCounter > 1) {
        if (iTimeStamp != ts) {
            iTimeStamp = ts;
        } else {
            ++iTimeStamp;
        }
    }
    
    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;
    if (!oscl_pmem_register(&rInfo))
    {
        LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rInfo.base, rInfo.offset);
    }

    // queue data to be sent to peer
    AndroidCameraInputMediaData data;
    data.iXferHeader.seq_num = iDataEventCounter++;
    data.iXferHeader.timestamp = iTimeStamp;
    data.iXferHeader.flags = 0;
    data.iXferHeader.duration = 0;
    data.iXferHeader.stream_id = 0;

    {//compose private data
        //could size be zero?
        if(NULL == pPmemInfo)
        {
            int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / size;
            LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), size, iCalculateNoOfCameraPreviewBuffer);
            pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
            if(NULL == pPmemInfo)
            {
                LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
                return PVMFFailure;
            }
        }

        int iIndex = offset / size;
        pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
        pPmemInfo[iIndex].offset = offset;
        data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
        LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
    }

    data.iFrameBuffer = frame;
    data.iFrameSize = size;
#ifdef COMPRESS_INPUT
    LOGD("hi, offset:%d, header:%08x ", offset, *((uint32*)(((char*)rInfo.base)+offset)) );
    data.iFrameSize = *((uint32*)( (((char *)rInfo.base)+offset)+size-4));
    LOGD("DATA SIZE:%d, %08x", data.iFrameSize, *((uint32*)( (((char *)rInfo.base)+offset)+size-4)));
#endif
//LOGD("postWriteAsync, SIZE:%d", size);
    // lock mutex and queue frame buffer
    iFrameQueueMutex.Lock();
    iFrameQueue.push_back(data);
    iFrameQueueMutex.Unlock();
    RunIfNotReady();

    return PVMFSuccess; 
}

#else //COMPRESS_INPUT
PVMFStatus AndroidCameraInput::postWriteAsync(nsecs_t timestamp, const sp<IMemory>& frame)
{
    LOGV("postWriteAsync");

    if (frame == NULL) {
        LOGE("frame is a NULL pointer");
        return PVMFFailure;
    }
    

				
    if((!iPeer) || (!isRecorderStarting()) || (iWriteState == EWriteBusy) || (NULL == iAuthorClock) 
    	|| (iAuthorClock->GetState() != PVMFMediaClock::RUNNING)
    		) {
        if( NULL == iAuthorClock )
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iAuthorClock NULL), frame dropped", iPeer, iState, iWriteState);
        }
        else
        {
            LOGE("Recording is not ready (iPeer %p iState %d iWriteState %d iClockState %d), frame dropped", iPeer, iState, iWriteState, iAuthorClock->GetState());
        } 

        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    }
    

				
#if 1
    // if first event, set timestamp to zero
    if (iDataEventCounter == 0) {
        iStartTickCount = timestamp;
        iTimeStamp = 0;
        iAVTSOffset = 0;
        LOGV("First event, set timestamp to zero, timestamp:%lld", timestamp);
    }
    
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp    
    uint32 ts = (uint32)((timestamp-iStartTickCount) / 1000000LL);
    ts += iAVTSOffset;
    //LOGD("ts:%d, iAudioFirstFrameTs:%d", ts, iAudioFirstFrameTs);

    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if (iAudioFirstFrameTs == 0)
    {
        iAudioFirstFrameTs = ts;
        LOGV("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }   
    else if(iDataEventCounter == 0 && ts < iAudioFirstFrameTs)
    {
        iAVTSOffset = iAudioFirstFrameTs - ts;
        //iAVTSOffset = iAudioFirstFrameTs;
        ts += iAVTSOffset;
        LOGV("ts < iAudioFirstFrameTs, update iAVTSOffset:%d", iAVTSOffset);
    }
#else
    // Now compare the video timestamp with the AudioFirstTimestamp
    // if video timestamp is earlier to audio drop it
    // or else send it downstream with correct timestamp
    uint32 ts = (uint32)(timestamp / 1000000L);    


    // In cases of Video Only recording iAudioFirstFrameTs will always be zero,
    // so for such cases assign iAudioFirstFrameTs to Video's first sample TS
    // which will make Video samples to start with Timestamp zero.
    if (iAudioFirstFrameTs == 0)
    {
        iAudioFirstFrameTs = ts;
        LOGD("iAudioFirstFrameTs == 0, timestamp:%lld, ts:%u", timestamp, ts);
    }
#endif
    if (ts < iAudioFirstFrameTs) {
        LOGE("postWriteAsync,Drop the frame, ts:%d ,iAudioFirstFrameTs:%d ",ts , iAudioFirstFrameTs);
        // Drop the frame
        mCamera->releaseRecordingFrame(frame);
        return PVMFSuccess;
    } else {
         // calculate timestamp as offset from start time
         ts -= iAudioFirstFrameTs;
    }

    // Make sure that no two samples have the same timestamp
    if (iDataEventCounter != 0) {
        if (iTimeStamp != ts) {
            iTimeStamp = ts;
        } else {
            ++iTimeStamp;
        }
    }
    
    LOGV("postWriteAsync, timestamp:%lld, iTimeStamp:%d, ts:%u, iAudioFirstFrameTs:%u, iDataEventCounter:%d", timestamp, iTimeStamp, ts, iAudioFirstFrameTs, iDataEventCounter);
    // get memory offset for frame buffer
    ssize_t offset = 0;
    size_t size = 0;
    sp<IMemoryHeap> heap = frame->getMemory(&offset, &size);
    LOGV("postWriteAsync: ID = %d, base = %p, offset = %ld, size = %d pointer %p", heap->getHeapID(), heap->base(), offset, size, frame->pointer());

    ExPmemInfo rInfo;
    rInfo.base = heap->base();
    rInfo.size = size;
    rInfo.shared_fd = heap->heapID();
    rInfo.offset = offset;
    if (!oscl_pmem_register(&rInfo))
    {
        LOGE("oscl pmem register err: 0x%X, offset: 0x%X !", (uint32)rInfo.base, rInfo.offset);
    }

    // queue data to be sent to peer
    AndroidCameraInputMediaData data;
    data.iXferHeader.seq_num = iDataEventCounter++;
    data.iXferHeader.timestamp = iTimeStamp;
    data.iXferHeader.flags = 0;
    data.iXferHeader.duration = 0;
    data.iXferHeader.stream_id = 0;

    {//compose private data
        //could size be zero?
        if(NULL == pPmemInfo)
        {
            int iCalculateNoOfCameraPreviewBuffer = heap->getSize() / size;
            LOGV("heap->getSize() = %d, size of each frame= %d, iCalculateNoOfCameraPreviewBuffer = %d", heap->getSize(), size, iCalculateNoOfCameraPreviewBuffer);
            pPmemInfo = new CAMERA_PMEM_INFO[iCalculateNoOfCameraPreviewBuffer];
            if(NULL == pPmemInfo)
            {
                LOGE("Failed to allocate the camera pmem info buffer array. iCalculateNoOfCameraPreviewBuffer %d",iCalculateNoOfCameraPreviewBuffer);
                return PVMFFailure;
            }
        }

        int iIndex = offset / size;
        pPmemInfo[iIndex].pmem_fd = heap->getHeapID();
        pPmemInfo[iIndex].offset = offset;
        data.iXferHeader.private_data_ptr = ((OsclAny*)(&pPmemInfo[iIndex]));
        LOGV("struct size %d, pmem_info - %x, &pmem_info[iIndex] - %x, iIndex =%d, pmem_info.pmem_fd = %d, pmem_info.offset = %d", sizeof(CAMERA_PMEM_INFO), pPmemInfo, &pPmemInfo[iIndex], iIndex, pPmemInfo[iIndex].pmem_fd, pPmemInfo[iIndex].offset );
    }

    data.iFrameBuffer = frame;
    data.iFrameSize = size;
#ifdef COMPRESS_INPUT
    LOGD("hi, offset:%d, header:%08x ", offset, *((uint32*)(((char*)rInfo.base)+offset)) );
    data.iFrameSize = *((uint32*)( (((char *)rInfo.base)+offset)+size-4));
    LOGD("DATA SIZE:%d, %08x", data.iFrameSize, *((uint32*)( (((char *)rInfo.base)+offset)+size-4)));
#endif
//LOGD("postWriteAsync, SIZE:%d", size);
    // lock mutex and queue frame buffer
    iFrameQueueMutex.Lock();
    iFrameQueue.push_back(data);
    iFrameQueueMutex.Unlock();
    RunIfNotReady();

    return PVMFSuccess; 
}
#endif

// Value is expected to be in ms
void AndroidCameraInput::setAudioLossDuration(uint32 duration)
{
    iAudioLossMutex.Lock();
    LOGD("Update for lost audio for %lu for existing duration %lu", duration, iAudioLossDuration);
    iAudioLossDuration += duration;
    iAudioLossMutex.Unlock();
}

// camera callback interface
void AndroidCameraInputListener::postData(int32_t msgType, const sp<IMemory>& dataPtr)
{
}

void AndroidCameraInputListener::postDataTimestamp(nsecs_t timestamp, int32_t msgType, const sp<IMemory>& dataPtr)
{
    if ((mCameraInput != NULL) && (msgType == CAMERA_MSG_VIDEO_FRAME)) {
        mCameraInput->postWriteAsync(timestamp, dataPtr);
    }
}
void AndroidCameraInputListener::notify(int32_t msgType, int32_t ext1, int32_t ext2)
{
	if(CAMERA_MSG_ZOOM == msgType)
	{
		LOGD("CAMERA_MSG_ZOOM Notify!\n" );
		if(mCameraInput)
		  mCameraInput->OnCameraNotify(0x100,ext1,ext2);
	}
}

void AndroidCameraInput::NotificationsInterfaceDestroyed()
{
    iClockNotificationsInf = NULL;
}

void AndroidCameraInput::ClockStateUpdated()
{
    PVMFMediaClock::PVMFMediaClockState iClockState = iAuthorClock->GetState();
    if ((iClockState == PVMFMediaClock::RUNNING) && (iAudioFirstFrameTs == 0)) {
        // Get the clock time here
        // this will be the time of first audio frame capture
        bool tmpbool = false;
        iAuthorClock->GetCurrentTime32(iAudioFirstFrameTs, tmpbool, PVMF_MEDIA_CLOCK_MSEC);
        LOGV("Audio first ts %d", iAudioFirstFrameTs);
    }
}

void AndroidCameraInput::RemoveDestroyClockObs()
{
    if (iAuthorClock != NULL) {
        if (iClockNotificationsInf != NULL) {
            iClockNotificationsInf->RemoveClockStateObserver(*this);
            iAuthorClock->DestroyMediaClockNotificationsInterface(iClockNotificationsInf);
            iClockNotificationsInf = NULL;
        }
    }
}



