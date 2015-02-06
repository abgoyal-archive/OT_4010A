

//#define LOG_NDEBUG 0
#define MIO_TAG "VideoMIO"
#define LOG_TAG MIO_TAG
#include <utils/Log.h>
#include <surfaceflinger/ISurface.h>
#include <ui/egl/android_natives.h>

#include "android_surface_output.h"
#include <media/PVPlayer.h>

#include "pvlogger.h"
#include "pv_mime_string_utils.h"
#include "oscl_snprintf.h"

#include "oscl_dll.h"
#include "pvmf_video.h"

// Ryan Lin !! for get tick count
#include "oscl_tickcount.h"
#include "oscl_profile.h"
#include "oscl_file_cfg.h"

#ifdef MT6516_OMX_USE_PMEM
#include "oscl_pmem.h"
#include <binder/MemoryHeapPmem.h>
#endif

// add by Ryan Lin for use mm queue
#include "oscl_mm_queue.h"

#include <sys/types.h>
#include <signal.h>

//The factory functions.
#include "oscl_mem.h"

#include "oscl_trace.h"

// [Honda] use Keene's RR setting
#include <linux/rtpm_prio.h>
#undef LOG_TAG
#define LOG_TAG "VideoMIO"

#define LOG_PROFILE(...)\
{\
    if (bSave_Profile)\
    {\
        if (m_pProfileFile != NULL) \
        {\
            fprintf(m_pProfileFile, __VA_ARGS__);\
        }\
    }\
    if (bLog_Profile)\
    {\
        LOGD(__VA_ARGS__);\
    }\
}

static FILE* m_pProfileFile = NULL;
static bool bShowLog = false;
static uint32 iDecTheshold = 0;
static uint32 iDspThreshold = 0;
static uint32 iSeqNumThreshold = 0;
static bool bEnableFTrace = false;
static bool bSave_Profile = false;
static bool bLog_Profile = false;
static bool bLog_PostInfo = false;
static bool bDisableOutputThread = false;
    
#define PROFILING_PATH "/vp.txt"

#define LOGP(...) \
{\
    if (bShowLog)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#define LOG_POSTINFO(...) \
{\
    if (bLog_PostInfo)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE) JJLOGE MESSAGE
#define JJLOGE(id, ...) LOGP(__VA_ARGS__)

#define PRESERVE_CURRENT_BUFFER 1
#define PROFILE_TAG "PROFILE"

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW) 
#define USE_BUF_QUEUE 1
#define CONVERT_YUV_TO_RGB 0
#endif

OSCL_DLL_ENTRY_POINT_DEFAULT()

// Define entry point for this DLL

using namespace android;

OSCL_EXPORT_REF AndroidSurfaceOutput::AndroidSurfaceOutput() :
    OsclTimerObject(OsclActiveObject::EPriorityNominal, "androidsurfaceoutput")
    , mAVSyncCtrl(NULL)
    , m_iPostBufferTime(0)
    , m_iGetNotifyTime(0)
    , iExitSurfaceThread(false)
    , iReturnBuffers(false)
    , iSurfaceThreadSem(NULL)
    , iSurfaceThreadTermSem(NULL)
    , m_bCreateThread(false)
    , mFrameBuffers(NULL)
    , kBufferCount(8)
    , iNumberOfBuffers(8)
{
    initData();

    iColorConverter = NULL;
    mInitialized = false;
    mPvPlayer = NULL;
    mEmulation = false;
    iEosReceived = false; 

    // for return buf
    m_DataAddr = NULL;

    iThreadSafeSurfaceOutputAO = NULL;

    mSeqNum = 0;
    mPBISeqNum = 0;
    mDropFrameCount = 0;
    returnAllBuffers();

    OsclProfile::ClearMap();
    
    if (bSave_Profile)
    {
        m_pProfileFile = fopen(PROFILING_PATH, "w+");
    }

    mNumberOfFramesToHold = 1;
}

status_t AndroidSurfaceOutput::set(PVPlayer* pvPlayer, const sp<ISurface>& surface, bool emulation)
{
    mPvPlayer = pvPlayer;
    mEmulation = emulation;
    setVideoSurface(surface);
    
    return NO_ERROR;
}

status_t AndroidSurfaceOutput::setVideoSurface(const sp<ISurface>& surface)
{
    LOGP("setVideoSurface(%p)", surface.get());
    // unregister buffers for the old surface
    if (mSurface != NULL) {
        LOGP("unregisterBuffers from old surface");
        mSurface->unregisterBuffers();
    }
    mSurface = surface;
    // register buffers for the new surface
    if ((mSurface != NULL) && (mBufferHeap.heap != NULL)) {
        LOGP("registerBuffers from old surface");
        mSurface->registerBuffers(mBufferHeap);       
#if defined(MTK_TVOUT_SUPPORT)
        mSurface->setLayerType(LAYER_TYPE_VIDEO);
#endif
    }

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
#if PRESERVE_CURRENT_BUFFER
    if (iThreadSafeSurfaceOutputAO == NULL)
    {
        iThreadSafeSurfaceOutputAO = OSCL_NEW(ThreadSafeSurfaceOutputAO, (this, 10, ThreadSafeSurfaceOutputAOName, Priority()+1));
    }
    if (iThreadSafeSurfaceOutputAO != NULL)
    {
        LOGP("!!!!! AndroidSurfaceOutput::setVideoSurface - set Listener !!!!!");
#if 1        
        //mSurface->setListener(iThreadSafeSurfaceOutputAO);
#else
        mSurface->setListener(this);
#endif
    }
#else
    LOGP("!!!!! AndroidSurfaceOutput::setVideoSurface - no set Listener !!!!!");
#endif
#endif

    return NO_ERROR;
}

void AndroidSurfaceOutput::initData()
{
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_surfaceoutput");
    bSave_Profile = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "save_profile");
    bLog_Profile = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_profile");
    bLog_PostInfo = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_posttime");

    bEnableFTrace = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "enable_ftrace");    
    iDecTheshold = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "dec_threshold");
    iDspThreshold = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "dsp_threshold");
    iSeqNumThreshold = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "seq_threshold");

    bDisableOutputThread = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "disable_surf_output_thread");
        
    iVideoHeight = iVideoWidth = iVideoDisplayHeight = iVideoDisplayWidth = 0;
    iVideoFormat=PVMF_MIME_FORMAT_UNKNOWN;
    resetVideoParameterFlags();

    iCommandCounter=0;
    iLogger=NULL;
    iCommandResponseQueue.reserve(8);
    iWriteResponseQueue.reserve(8);
    iObserver=NULL;
    iLogger=NULL;
    iPeer=NULL;
    iState=STATE_IDLE;
    iIsMIOConfigured = false;

    // semaphore used to communicate between this  mio and the audio output thread
    iSurfaceThreadSem = new OsclSemaphore();
    iSurfaceThreadSem->Create(0);
    iSurfaceThreadTermSem = new OsclSemaphore();
    iSurfaceThreadTermSem->Create(0);

    iDispRequestQueueLock.Create();
}

void AndroidSurfaceOutput::ResetData()
    //reset all data from this session.
{
    Cleanup();

    //reset all the received media parameters.
    iVideoFormatString="";
    iVideoFormat=PVMF_MIME_FORMAT_UNKNOWN;
    resetVideoParameterFlags();
    iIsMIOConfigured = false;

    returnAllBuffers();
}

void AndroidSurfaceOutput::resetVideoParameterFlags()
{
    iVideoParameterFlags = VIDEO_PARAMETERS_INVALID;
}

bool AndroidSurfaceOutput::checkVideoParameterFlags()
{
    return (iVideoParameterFlags & VIDEO_PARAMETERS_MASK) == VIDEO_PARAMETERS_VALID;
}

void AndroidSurfaceOutput::processWriteResponseQueue(int numFramesToHold)
{
    LOGP("processWriteResponseQueue: queued = %d, numFramesToHold = %d",
            iWriteResponseQueue.size(), numFramesToHold);
    while (iWriteResponseQueue.size() > numFramesToHold) {
        if (iPeer) {
            iPeer->writeComplete(iWriteResponseQueue[0].iStatus,
                    iWriteResponseQueue[0].iCmdId,
                    (OsclAny*)iWriteResponseQueue[0].iContext);
        }
        iWriteResponseQueue.erase(&iWriteResponseQueue[0]);
    }
}

void AndroidSurfaceOutput::Cleanup()
//cleanup all allocated memory and release resources.
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Cleanup() In"));
    while (!iCommandResponseQueue.empty())
    {
        if (iObserver)
            iObserver->RequestCompleted(PVMFCmdResp(iCommandResponseQueue[0].iCmdId, iCommandResponseQueue[0].iContext, iCommandResponseQueue[0].iStatus));
        iCommandResponseQueue.erase(&iCommandResponseQueue[0]);
    }

    processWriteResponseQueue(0);

    // We'll close frame buf and delete here for now.
    closeFrameBuf();    

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Cleanup() Out"));
 }

OSCL_EXPORT_REF AndroidSurfaceOutput::~AndroidSurfaceOutput()
{
    // make sure output thread has exited
    RequestAndWaitForThreadExit();        

    // clean up some thread interface objects
    iSurfaceThreadSem->Close();
    delete iSurfaceThreadSem;
    iSurfaceThreadTermSem->Close();
    delete iSurfaceThreadTermSem;

    iDispRequestQueueLock.Close();
    
    mAVSyncCtrl = NULL;
    Cleanup(); 

	if (mFrameBuffers)
	{
		delete [] mFrameBuffers;
		mFrameBuffers = NULL;
	}

    if (iThreadSafeSurfaceOutputAO != NULL)
    {
        OSCL_DELETE(iThreadSafeSurfaceOutputAO);
        iThreadSafeSurfaceOutputAO = NULL;
    }   

    if (m_pProfileFile != NULL)
    {
        fclose(m_pProfileFile);
    }
    m_pProfileFile = NULL;
}


PVMFStatus AndroidSurfaceOutput::connect(PvmiMIOSession& aSession, PvmiMIOObserver* aObserver)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::connect() called"));
    // Each Session could have its own set of Configuration parameters
    //in an array of structures and the session ID could be an index to that array.

    //currently supports only one session
    if (iObserver)
        return PVMFFailure;

    iObserver=aObserver;
    return PVMFSuccess;
}


PVMFStatus AndroidSurfaceOutput::disconnect(PvmiMIOSession aSession)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::disconnect() called"));
    //currently supports only one session
    iObserver=NULL;
    return PVMFSuccess;
}


PvmiMediaTransfer* AndroidSurfaceOutput::createMediaTransfer(PvmiMIOSession& aSession, 
                                                        PvmiKvp* read_formats, int32 read_flags,
                                                        PvmiKvp* write_formats, int32 write_flags)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::createMediaTransfer() called"));
    return (PvmiMediaTransfer*)this;
}

void AndroidSurfaceOutput::QueueCommandResponse(CommandResponse& aResp)
{
    //queue a command response and schedule processing.

    iCommandResponseQueue.push_back(aResp);

    //cancel any timer delay so the command response will happen ASAP.
    if (IsBusy())
        Cancel();

    RunIfNotReady();
}

PVMFCommandId AndroidSurfaceOutput::QueryUUID(const PvmfMimeString& aMimeType, 
                                        Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                        bool aExactUuidsOnly, const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::QueryUUID() called"));

    OSCL_UNUSED_ARG(aMimeType);
    OSCL_UNUSED_ARG(aExactUuidsOnly);

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;
    int32 err ;
    OSCL_TRY(err, aUuids.push_back(PVMI_CAPABILITY_AND_CONFIG_PVUUID););
    if (err==OsclErrNone)
        status= PVMFSuccess;

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}


PVMFCommandId AndroidSurfaceOutput::QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr, const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::QueryInterface() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;
    if(aUuid == PVMI_CAPABILITY_AND_CONFIG_PVUUID)
    {
        PvmiCapabilityAndConfig* myInterface = OSCL_STATIC_CAST(PvmiCapabilityAndConfig*,this);
        aInterfacePtr = OSCL_STATIC_CAST(PVInterface*, myInterface);
        status= PVMFSuccess;
    }
    else
    {
        status=PVMFFailure;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}


void AndroidSurfaceOutput::deleteMediaTransfer(PvmiMIOSession& aSession, PvmiMediaTransfer* media_transfer)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::deleteMediaTransfer() called"));
    // This class is implementing the media transfer, so no cleanup is needed
}


PVMFCommandId AndroidSurfaceOutput:: Init(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Init() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;

    switch(iState)
    {
    case STATE_LOGGED_ON:
        status=PVMFSuccess;
        iState=STATE_INITIALIZED;
        break;

    default:
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Invalid State"));
        status=PVMFErrInvalidState;
        break;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::Reset(const OsclAny* aContext)
{
    RequestAndWaitForThreadExit();
    ResetData();
    PVMFCommandId cmdid=iCommandCounter++;
    CommandResponse resp(PVMFSuccess,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}


PVMFCommandId AndroidSurfaceOutput::Start(const OsclAny* aContext)
{
    iEosReceived = false;
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Start() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;

    switch(iState)
    {
    case STATE_INITIALIZED:
    case STATE_PAUSED:

        iState=STATE_STARTED;
        processWriteResponseQueue(0);
        status=PVMFSuccess;
        break;

    default:
        status=PVMFErrInvalidState;
        break;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

// post the last video frame to refresh screen after pause
void AndroidSurfaceOutput::postLastFrame()
{   
    LOGP("AndroidSurfaceOutput::postLastFrame in");
    
#if USE_PMEM

#if PRESERVE_CURRENT_BUFFER
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    //Mutex::Autolock _l(mLock);
    if (m_BufRetMap.size() != 0)
    {
        LOGP("AndroidSurfaceOutput::postLastFrame - return buffer not empty");
        //iState = STATE_POST_LAST_FRAME;
        //RunIfNotReady(3000000);
        return;
    }
#endif    
#endif 
    LOGP("AndroidSurfaceOutput::postLastFrame - current index: 0x%x", mCurBufferIndex);
    mSurface->postBuffer(mCurBufferIndex);
    
#else

    int CurBufferIndex = kBufferCount - 1;
    if (mFrameBufferIndex != 0)
    {
        CurBufferIndex = mFrameBufferIndex - 1;
    }
    LOGP("!!!!! AndroidSurfaceOutput::postLastFrame - %d !!!!!", CurBufferIndex);
    mSurface->postBuffer(mFrameBuffers[CurBufferIndex]);
    
#endif    
    LOGP("AndroidSurfaceOutput::postLastFrame out");
}

PVMFCommandId AndroidSurfaceOutput::Pause(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Pause() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;

    switch(iState)
    {
    case STATE_STARTED:

        iState=STATE_PAUSED;
        status=PVMFSuccess;

        // post last buffer to prevent stale data
        // if not configured, PVMFMIOConfigurationComplete is not sent
        // there should not be any media data.
        if(iIsMIOConfigured) 
        { 
            postLastFrame();
        }
        break;

    default:
        status=PVMFErrInvalidState;
        break;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}


PVMFCommandId AndroidSurfaceOutput::Flush(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Flush() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;

    switch(iState)
    {
    case STATE_STARTED:

        iState=STATE_INITIALIZED;
        status=PVMFSuccess;
        break;

    default:
        status=PVMFErrInvalidState;
        break;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::DiscardData(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::DiscardData(const OsclAny* aContext)() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    //this component doesn't buffer data, so there's nothing
    //needed here.

    PVMFStatus status=PVMFSuccess;
    processWriteResponseQueue(0);

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext)() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    aTimestamp = 0;

    //this component doesn't buffer data, so there's nothing
    //needed here.

    PVMFStatus status=PVMFSuccess;
    processWriteResponseQueue(0);

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::Stop(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::Stop() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    PVMFStatus status=PVMFFailure;

    switch(iState)
    {
    case STATE_STARTED:
    case STATE_PAUSED:

#ifdef PERFORMANCE_MEASUREMENTS_ENABLED
        // FIXME: This should be moved to OMAP library
    PVOmapVideoProfile.MarkEndTime();
    PVOmapVideoProfile.PrintStats();
    PVOmapVideoProfile.Reset();
#endif

        iState=STATE_INITIALIZED;
        status=PVMFSuccess;
        break;

    default:
        status=PVMFErrInvalidState;
        break;
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::CancelAllCommands(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::CancelAllCommands() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    //commands are executed immediately upon being received, so
    //it isn't really possible to cancel them.

    PVMFStatus status=PVMFSuccess;

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

PVMFCommandId AndroidSurfaceOutput::CancelCommand(PVMFCommandId aCmdId, const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::CancelCommand() called"));

    PVMFCommandId cmdid=iCommandCounter++;

    //commands are executed immediately upon being received, so
    //it isn't really possible to cancel them.

    //see if the response is still queued.
    PVMFStatus status=PVMFFailure;
    for (uint32 i=0;i<iCommandResponseQueue.size();i++)
    {
        if (iCommandResponseQueue[i].iCmdId==aCmdId)
        {
            status=PVMFSuccess;
            break;
        }
    }

    CommandResponse resp(status,cmdid,aContext);
    QueueCommandResponse(resp);
    return cmdid;
}

void AndroidSurfaceOutput::ThreadLogon()
{
    if(iState==STATE_IDLE)
    {
        iLogger = PVLogger::GetLoggerObject("PVOmapVideo");
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::ThreadLogon() called"));
        AddToScheduler();
        iState=STATE_LOGGED_ON;
    }
}


void AndroidSurfaceOutput::ThreadLogoff()
{   
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::ThreadLogoff() called"));
    
    if(iState!=STATE_IDLE)
    {
        RemoveFromScheduler();
        iLogger=NULL;
        iState=STATE_IDLE;
        //Reset all data from this session
        ResetData();
    }
}

void AndroidSurfaceOutput::setPeer(PvmiMediaTransfer* aPeer)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::setPeer() called"));
    // Set the observer 
    iPeer = aPeer;
}


void AndroidSurfaceOutput::useMemoryAllocators(OsclMemAllocator* write_alloc)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::useMemoryAllocators() called"));
    //not supported.
}

// This routine will determine whether data can be accepted in a writeAsync
// call and if not, will return true;
bool AndroidSurfaceOutput::CheckWriteBusy(uint32 aSeqNum)
{
    // for all other cases, accept data now.
    return false;
}

PVMFCommandId AndroidSurfaceOutput::writeAsync(uint8 aFormatType, int32 aFormatIndex, uint8* aData, uint32 aDataLen,
                                        const PvmiMediaXferHeader& data_header_info, OsclAny* aContext)
{
    LOGP("AndroidSurfaceOutput::writeAsync in");
    // Do a leave if MIO is not configured except when it is an EOS
    if (!iIsMIOConfigured
            &&
            !((PVMI_MEDIAXFER_FMT_TYPE_NOTIFICATION == aFormatType)
              && (PVMI_MEDIAXFER_FMT_INDEX_END_OF_STREAM == aFormatIndex)))
    {
        LOGE("data is pumped in before MIO is configured");
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    uint32 aSeqNum=data_header_info.seq_num;
    PVMFTimestamp aTimestamp=data_header_info.timestamp;
    uint32 flags=data_header_info.flags;
    
    if (aSeqNum < 6)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidSurfaceOutput::writeAsync() seqnum %d ts %d context %d",aSeqNum,aTimestamp,aContext));

        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidSurfaceOutput::writeAsync() Format Type %d Format Index %d length %d",aFormatType,aFormatIndex,aDataLen));
    }

    PVMFStatus status=PVMFFailure;

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW) 
    //Mutex::Autolock _l(mLock);
#endif

    bool bWriteResponseQueue = true;
    switch(aFormatType)
    {
    case PVMI_MEDIAXFER_FMT_TYPE_COMMAND :
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidSurfaceOutput::writeAsync() called with Command info."));
        //ignore
        status= PVMFSuccess;
        break;

    case PVMI_MEDIAXFER_FMT_TYPE_NOTIFICATION :
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidSurfaceOutput::writeAsync() called with Notification info."));
        switch(aFormatIndex)
        {
        case PVMI_MEDIAXFER_FMT_INDEX_END_OF_STREAM:
            iEosReceived = true;            
            break;
        default:
            break;
        }
        //ignore
        status= PVMFSuccess;
        break;

    case PVMI_MEDIAXFER_FMT_TYPE_DATA :
        switch(aFormatIndex)
        {
        case PVMI_MEDIAXFER_FMT_INDEX_FMT_SPECIFIC_INFO:
            //format-specific info contains codec headers.
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::writeAsync() called with format-specific info."));

            if (iState<STATE_INITIALIZED)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                    (0,"AndroidSurfaceOutput::writeAsync: Error - Invalid state"));
                status=PVMFErrInvalidState;
            }
            else
            {
                status= PVMFSuccess;
            }

            break;

        case PVMI_MEDIAXFER_FMT_INDEX_DATA:
            //data contains the media bitstream.

            //Verify the state
            if (iState!=STATE_STARTED)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                    (0,"AndroidSurfaceOutput::writeAsync: Error - Invalid state"));
                status=PVMFErrInvalidState;
            }
            else
            {
            // <--- Morris Yang ALPS00006444 : seek after EOS (it's possible to happen when the audio is longer than video)
            // In this case, we still have to keep the last frame (the one posted to Surface Flinger) otherwise the decoder has chance to 
            // write to the pmem address used by SF (last frame) which results in broken image if direct-link was enabled.
                if (iEosReceived)
                {
                    iEosReceived = false; // Don't return all buffers to dec node, keep the latest one to make sure decoder and SF will not access the same pmem address
                    LOGE ("[Warning] Seek after EOS !!");
                }
            // --->

#if PRESERVE_CURRENT_BUFFER 
#if (!defined(MT6516_MP4_HW_DECODER_V2) && !defined(MT6573_MFV_HW))
                if (m_DataAddr != NULL)
                {
                    HandleRetBuf(m_DataAddr);
                    m_DataAddr = NULL;
                }
#else                
#endif                
                bWriteResponseQueue = false;
#endif                
                //printf("V WriteAsync { seq=%d, ts=%d }\n", data_header_info.seq_num, data_header_info.timestamp);

                // Call playback to send data to IVA for Color Convert
                LOGP("writeAsync - pushBufToRequestQueue(%x)", aData);                                                                                                       
#if 0                
                status = writeFrameBuf(aData, aDataLen, data_header_info);
#else
                status = pushBufToRequestQueue(aData, aDataLen, data_header_info, aContext);                                                                                 
#if !USE_BUF_QUEUE
                LOGP("writeAsync - handleDispQueue");
                status = handleDispQueue();                
#endif

#endif

#if 0
                PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                   (0,"AndroidSurfaceOutput::writeAsync: Playback Progress - frame %d",iFrameNumber++));
#endif
            }
            break;

        default:
            PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                (0,"AndroidSurfaceOutput::writeAsync: Error - unrecognized format index"));
            status= PVMFFailure;
            break;
        }
        break;

    default:
        PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
            (0,"AndroidSurfaceOutput::writeAsync: Error - unrecognized format type"));
        status= PVMFFailure;
        break;
    }    

    //Schedule asynchronous response
    PVMFCommandId cmdid=iCommandCounter++;
    if (bWriteResponseQueue || status == PVMFInfoBufNotReturn)
    {
        status = PVMFSuccess;
        WriteResponse resp(status,cmdid,aContext,aTimestamp);
        iWriteResponseQueue.push_back(resp);
    }
#if 0    
    else
    {
        pushBufToRetQueue(aData, status, cmdid, aContext, aTimestamp);
    }
#endif    
    RunIfNotReady();

    return cmdid;
}

void AndroidSurfaceOutput::writeComplete(PVMFStatus aStatus, PVMFCommandId  write_cmd_id, OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::writeComplete() called"));
    //won't be called since this component is a sink.
}


PVMFCommandId  AndroidSurfaceOutput::readAsync(uint8* data, uint32 max_data_len, OsclAny* aContext,
                                            int32* formats, uint16 num_formats)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::readAsync() called"));
    //read not supported.
    OsclError::Leave(OsclErrNotSupported);
    return -1;
}


void AndroidSurfaceOutput::readComplete(PVMFStatus aStatus, PVMFCommandId  read_cmd_id, int32 format_index,
                                    const PvmiMediaXferHeader& data_header_info, OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::readComplete() called"));
    //won't be called since this component is a sink.
}


void AndroidSurfaceOutput::statusUpdate(uint32 status_flags)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::statusUpdate() called"));
    //won't be called since this component is a sink.
}


void AndroidSurfaceOutput::cancelCommand(PVMFCommandId  command_id)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::cancelCommand() called"));

    //the purpose of this API is to cancel a writeAsync command and report
    //completion ASAP.

    //in this implementation, the write commands are executed immediately
    //when received so it isn't really possible to cancel.
    //just report completion immediately.
    processWriteResponseQueue(0);
}

void AndroidSurfaceOutput::cancelAllCommands()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidSurfaceOutput::cancelAllCommands() called"));

    //the purpose of this API is to cancel all writeAsync commands and report
    //completion ASAP.

    //in this implementaiton, the write commands are executed immediately 
    //when received so it isn't really possible to cancel.
    //just report completion immediately.

    for (uint32 i=0;i<iWriteResponseQueue.size();i++)
    {
        //report completion
        if (iPeer)
            iPeer->writeComplete(iWriteResponseQueue[i].iStatus,iWriteResponseQueue[i].iCmdId,(OsclAny*)iWriteResponseQueue[i].iContext);
        iWriteResponseQueue.erase(&iWriteResponseQueue[i]);
    }
}

void AndroidSurfaceOutput::setObserver (PvmiConfigAndCapabilityCmdObserver* aObserver)
{
    OSCL_UNUSED_ARG(aObserver);
    //not needed since this component only supports synchronous capability & config
    //APIs.
}

PVMFStatus AndroidSurfaceOutput::getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier,
                                              PvmiKvp*& aParameters, int& num_parameter_elements,
                                              PvmiCapabilityContext aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    aParameters=NULL;

    // This is a query for the list of supported formats.
    if(pv_mime_strcmp(aIdentifier, INPUT_FORMATS_CAP_QUERY) == 0)
    {
        aParameters=(PvmiKvp*)oscl_malloc(sizeof(PvmiKvp));
        if (aParameters == NULL) return PVMFErrNoMemory;
        aParameters[num_parameter_elements++].value.pChar_value=(char*) PVMF_MIME_YUV420;

        return PVMFSuccess;
    }

    //unrecognized key.
    return PVMFFailure;
}

PVMFStatus AndroidSurfaceOutput::releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements)
{
    //release parameters that were allocated by this component.
    if (aParameters)
    {
        oscl_free(aParameters);
        return PVMFSuccess;
    }
    return PVMFFailure;
}

void AndroidSurfaceOutput ::createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext)
{
    OsclError::Leave(OsclErrNotSupported);
}

void AndroidSurfaceOutput::setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext,
                                           PvmiKvp* aParameters, int num_parameter_elements)
{
    OsclError::Leave(OsclErrNotSupported);
}

void AndroidSurfaceOutput::DeleteContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext)
{
    OsclError::Leave(OsclErrNotSupported);
}


void AndroidSurfaceOutput::setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                        int num_elements, PvmiKvp * & aRet_kvp)
{
    OSCL_UNUSED_ARG(aSession);

    aRet_kvp = NULL;

    LOGP("setParametersSync");
    for (int32 i=0;i<num_elements;i++)
    {
        //Check against known video parameter keys...
        if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_FORMAT_KEY) == 0)
        {
            iVideoFormatString=aParameters[i].value.pChar_value;
            iVideoFormat=iVideoFormatString.get_str();
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Video Format Key, Value %s",iVideoFormatString.get_str()));
        }
        else if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_WIDTH_KEY) == 0)
        {
            iVideoWidth=(int32)aParameters[i].value.uint32_value;
            iVideoParameterFlags |= VIDEO_WIDTH_VALID;
            LOGP("iVideoWidth=%d", iVideoWidth);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Video Width Key, Value %d",iVideoWidth));
        }
        else if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_HEIGHT_KEY) == 0)
        {
            iVideoHeight=(int32)aParameters[i].value.uint32_value;
            iVideoParameterFlags |= VIDEO_HEIGHT_VALID;
            LOGP("iVideoHeight=%d", iVideoHeight);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Video Height Key, Value %d",iVideoHeight));
        }
        else if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_DISPLAY_HEIGHT_KEY) == 0)
        {
            iVideoDisplayHeight=(int32)aParameters[i].value.uint32_value;
            iVideoParameterFlags |= DISPLAY_HEIGHT_VALID;
            LOGP("iVideoDisplayHeight=%d", iVideoDisplayHeight);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Video Display Height Key, Value %d",iVideoDisplayHeight));
        }
        else if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_DISPLAY_WIDTH_KEY) == 0)
        {
            iVideoDisplayWidth=(int32)aParameters[i].value.uint32_value;
            iVideoParameterFlags |= DISPLAY_WIDTH_VALID;
            LOGP("iVideoDisplayWidth=%d", iVideoDisplayWidth);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Video Display Width Key, Value %d",iVideoDisplayWidth));
        }
        else if (pv_mime_strcmp(aParameters[i].key, MOUT_VIDEO_SUBFORMAT_KEY) == 0)
        {
            iVideoSubFormat=aParameters[i].value.pChar_value;
            iVideoParameterFlags |= VIDEO_SUBFORMAT_VALID;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                    (0,"AndroidSurfaceOutput::setParametersSync() Video SubFormat Key, Value %s",iVideoSubFormat.getMIMEStrPtr()));

            LOGP("VIDEO SUBFORMAT SET TO %s\n",iVideoSubFormat.getMIMEStrPtr());
        }
        // Ryan Lin !!!        
        else if (pv_mime_strcmp(aParameters[i].key, PVMF_FORMAT_SPECIFIC_INFO_KEY_YUV) == 0)
        {
            uint8* data = (uint8*)aParameters->value.key_specific_value;            
            PVMFYuvFormatSpecificInfo0* yuvInfo = (PVMFYuvFormatSpecificInfo0*)data;            

            iVideoWidth = (int32)yuvInfo->width;            
            iVideoParameterFlags |= VIDEO_WIDTH_VALID;           
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Video Width, Value %d", iVideoWidth));            

            iVideoHeight = (int32)yuvInfo->height;            
            iVideoParameterFlags |= VIDEO_HEIGHT_VALID;           
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Video Height, Value %d", iVideoHeight));            

            iVideoDisplayHeight = (int32)yuvInfo->display_height;            
            iVideoParameterFlags |= DISPLAY_HEIGHT_VALID;         
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Video Display Height, Value %d", iVideoDisplayHeight));            

            iVideoDisplayWidth = (int32)yuvInfo->display_width;            
            iVideoParameterFlags |= DISPLAY_WIDTH_VALID;        
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Video Display Width, Value %d", iVideoDisplayWidth));            

            iNumberOfBuffers = (int32)yuvInfo->num_buffers;            
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Number of Buffer, Value %d", iNumberOfBuffers));            

            iBufferSize = (int32)yuvInfo->buffer_size;            
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,                            
                (0, "AndroidSurfaceOutput::setParametersSync() Buffer Size, Value %d", iBufferSize));

            iVideoSubFormat = yuvInfo->video_format;
            iVideoParameterFlags |= VIDEO_SUBFORMAT_VALID;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                    (0,"AndroidSurfaceOutput::setParametersSync() Video SubFormat Key, Value %s",iVideoSubFormat.getMIMEStrPtr()));

            LOGP("!!!!! VIDEO SUBFORMAT SET TO %s !!!!!",iVideoSubFormat.getMIMEStrPtr());
            
        }
        else
        {
            //if we get here the key is unrecognized.

            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidSurfaceOutput::setParametersSync() Error, unrecognized key = %s", aParameters[i].key));

            //set the return value to indicate the unrecognized key
            //and return.
            aRet_kvp = &aParameters[i];
            return;
        }
    }
    uint32 mycache = iVideoParameterFlags ;
    // if initialization is complete, update the app display info
    if( checkVideoParameterFlags() )
    {
    	initCheck();
    }
    iVideoParameterFlags = mycache;

    // when all necessary parameters are received, send 
    // PVMFMIOConfigurationComplete event to observer
    if(!iIsMIOConfigured && checkVideoParameterFlags() )
    {
        iIsMIOConfigured = true;
        if(iObserver)
        {
            iObserver->ReportInfoEvent(PVMFMIOConfigurationComplete);
        }
    }
}

PVMFCommandId AndroidSurfaceOutput::setParametersAsync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                                  int num_elements, PvmiKvp*& aRet_kvp, OsclAny* context)
{
    OsclError::Leave(OsclErrNotSupported);
    return -1;
}

uint32 AndroidSurfaceOutput::getCapabilityMetric (PvmiMIOSession aSession)
{
    return 0;
}

PVMFStatus AndroidSurfaceOutput::verifyParametersSync (PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements)
{
    OSCL_UNUSED_ARG(aSession);

    // Go through each parameter
    for (int32 i=0; i<num_elements; i++) {
        char* compstr = NULL;
        pv_mime_string_extract_type(0, aParameters[i].key, compstr);
        if (pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/media/format-type")) == 0) {
            if (pv_mime_strcmp(aParameters[i].value.pChar_value, PVMF_MIME_YUV420) == 0) {
                return PVMFSuccess;
            }
            else {
                return PVMFErrNotSupported;
            }
        }
    }
    return PVMFSuccess;
}

//
// Private section
//

void AndroidSurfaceOutput::Run()
{
#if USE_BUF_QUEUE
    iDispRequestQueueLock.Lock();                                                                                                                                            
    if (bDisableOutputThread)
    {
        if (!iDispRequestQueue.empty() && (m_BufRetMap.size() == 0))
        {
            LOGP("run handleDispQueue");
            handleDispQueue();
        }
    }
#if 1                                                                                                                                                                        
    else                                                                                                                                                                     
    {                                                                                                                                                                        
        uint32 _RequestSize = iDispRequestQueue.size();                                                                                                                      
        if (_RequestSize == m_BufRetMap.size() && _RequestSize != 0)                                                                                                         
        {                                                                                                                                                                    
            LOGP("Run: signal display");                                                                                                                                     
            //LOGD("debug(%d, %d)", iDispRequestQueue.size(), m_BufRetMap.size());                                                                                           
            triggerOutputThread();                                                                                                                                           
        }                                                                                                                                                                    
        else                                                                                                                                                                 
        {                                                                                                                                                                    
            LOGP("debug(%d, %d)", iDispRequestQueue.size(), m_BufRetMap.size());                                                                                             
        }                                                                                                                                                                    
    }                                                                                                                                                                        
#endif                                                                                                                                                                       
    iDispRequestQueueLock.Unlock();                                                                                                                                          
#endif
    //send async command responses
    while (!iCommandResponseQueue.empty())
    {
        if (iObserver)
        {
            iObserver->RequestCompleted(PVMFCmdResp(iCommandResponseQueue[0].iCmdId, iCommandResponseQueue[0].iContext, iCommandResponseQueue[0].iStatus));
        }
        if (!iCommandResponseQueue.empty())
        {
            iCommandResponseQueue.erase(&iCommandResponseQueue[0]);
        }
    }

    //send async write completion
    if (iEosReceived) {
        LOGP("Flushing buffers after EOS"); 
        processWriteResponseQueue(0);
    } else {
        processWriteResponseQueue(mNumberOfFramesToHold);
    }
}

// create a frame buffer for software codecs
OSCL_EXPORT_REF bool AndroidSurfaceOutput::initCheck()
{

    // initialize only when we have all the required parameters
    if (!checkVideoParameterFlags())
        return mInitialized;

    // release resources if previously initialized
    closeFrameBuf();

    // reset flags in case display format changes in the middle of a stream
    resetVideoParameterFlags();

    // copy parameters in case we need to adjust them
    int displayWidth = iVideoDisplayWidth;
    int displayHeight = iVideoDisplayHeight;
    int frameWidth = iVideoWidth;
    int frameHeight = iVideoHeight;
    int frameSize;

    // RGB-565 frames are 2 bytes/pixel
    displayWidth = (displayWidth + 1) & -2;
    displayHeight = (displayHeight + 1) & -2;
    frameWidth = (frameWidth + 1) & -2;
    frameHeight = (frameHeight + 1) & -2;
    frameSize = frameWidth * frameHeight * 2;

    LOGD("!!!!!**************************************************!!!!!");
    LOGD("!!!!!           video resolution = %d x %d           ", displayWidth, displayHeight);
    LOGD("!!!!!           frame resolution = %d x %d           ", frameWidth, frameHeight);
    LOGD("!!!!!           frame size = %d bytes               ", frameSize);
    LOGD("!!!!!**************************************************!!!!!");

    // Honda [
    kBufferCount = iNumberOfBuffers;
	if (mFrameBuffers)
	{
	    delete [] mFrameBuffers;
	    mFrameBuffers = NULL;
	}
	mFrameBuffers = new size_t[kBufferCount];
    for (int i = 0; i < kBufferCount; i++) 
    {
        mFrameBuffers[i] = 0;
    }
    // ]
    
    // Ryan Lin !!
#if USE_PMEM

    LOGP("AndroidSurfaceOutput::initCheck - Use Pmem, ignore the initialization of MemoryHeapBase");
    iIsFirstWriteFrameBuf = true;
    
#else

    LOGP("AndroidSurfaceOutput::initCheck - Create anonymous shared memory");
    // create frame buffer heap and register with surfaceflinger
    sp<MemoryHeapBase> heap = new MemoryHeapBase(frameSize * kBufferCount);
    if (heap->heapID() < 0) {
        LOGE("Error creating frame buffer heap");
        return false;
    }
    
    mBufferHeap = ISurface::BufferHeap(displayWidth, displayHeight,
            frameWidth, frameHeight, PIXEL_FORMAT_RGB_565, heap);
    mSurface->registerBuffers(mBufferHeap);
#if defined(MTK_TVOUT_SUPPORT)
    mSurface->setLayerType(LAYER_TYPE_VIDEO);
#endif

    // create frame buffers
    for (int i = 0; i < kBufferCount; i++) {
        mFrameBuffers[i] = i * frameSize;
    }

#endif // USE_PMEM

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    LOGP("AndroidSurfaceOutput::initCheck - output format: MTKYUV/YUV420");
#else
    // Ryan Lin !!
#ifndef MT6516_VIDEO_DECODER_OUTPUT_FORMAT
    // initialize software color converter
    iColorConverter = ColorConvert16::NewL();
    iColorConverter->Init(displayWidth, displayHeight, frameWidth, displayWidth, displayHeight, frameWidth, CCROTATE_NONE);
    //iColorConverter->Init(displayWidth, displayHeight, frameWidth, displayWidth, displayHeight, displayWidth, CCROTATE_NONE);
    iColorConverter->SetMemHeight(frameHeight);
    iColorConverter->SetMode(1);
#else
    LOGP("AndroidSurfaceOutput::initCheck - output format: RGB");
#endif // MT6516_VIDEO_DECODER_OUTPUT_FORMAT
#endif

    // add by Ryan as creating new thread for output
    OsclThread SurfaceOutput_Thread;
    iExitSurfaceThread = false;
    iReturnBuffers = false;

    if (!bDisableOutputThread)
    {
        if (m_bCreateThread == false)
        {
            OsclProcStatus::eOsclProcError ret = SurfaceOutput_Thread.Create((TOsclThreadFuncPtr)start_surfout_thread_func,
                                                        0, (TOsclThreadFuncArg)this, Start_on_creation);
            if(OsclProcStatus::SUCCESS_ERROR != ret)
            {
                LOGE("create output thread fail");
                mInitialized = false;
                return mInitialized;
            }
            m_bCreateThread = true;
			// Honda add
            if(iObserver){
				iObserver->ReportInfoEvent(PVMFInfoSinkThreadCreate);
           	}
        }
    }
    
    // register frame buffers with SurfaceFlinger
    mFrameBufferIndex = 0;
    mInitialized = true;
    mPvPlayer->sendEvent(MEDIA_SET_VIDEO_SIZE, iVideoDisplayWidth, iVideoDisplayHeight);

    return mInitialized;
}


//<-- Honda add
#define _DISPLAY_MARGIN_  (50)
#define _SLEEP_MARGIN_    (20)
//-->
OSCL_EXPORT_REF PVMFStatus AndroidSurfaceOutput::writeFrameBuf(uint8* aData, uint32 aDataLen, const PvmiMediaXferHeader& data_header_info)
{
    LOGP("writeFrameBuf(%d) in", gettid());
    // post to SurfaceFlinger
    if (mSurface == 0) return PVMFFailure;     

    // Ryan Lin !!
#ifndef MT6516_VIDEO_DECODER_OUTPUT_FORMAT
#if (!defined(MT6516_MP4_HW_DECODER_V2) && !defined(MT6573_MFV_HW))
    if (iColorConverter)
    {
        uint32 start_tickcount, end_tickcount;
        start_tickcount = OsclTickCount::TickCount();        
        iColorConverter->Convert(aData, static_cast<uint8*>(mBufferHeap.heap->base()) + mFrameBuffers[mFrameBufferIndex]);
        end_tickcount = OsclTickCount::TickCount();
    }
#endif    
#endif

#if USE_PMEM

    PmemInfo pmemInfo;
    oscl_pmem_get_info(aData, &pmemInfo);    
    
    PVMFStatus status = CheckFirstWriteFrameBuffer(&pmemInfo);
    if (status != PVMFSuccess)
    {
        LOGE("CheckFirstWriteFrameBuffer error(d)", status);
    }

// for copy buffer
#if 0
    uint32 start_tickcount, end_tickcount;
    start_tickcount = OsclTickCount::TickCount();
    // memory copy
    memcpy(static_cast<uint8*>(mBufferHeap.heap->base()) + mFrameBuffers[mFrameBufferIndex], aData, aDataLen);
    end_tickcount = OsclTickCount::TickCount();

    LOGP("memcpy output buffer %d size to surface flinger - spend %d ms", aDataLen, end_tickcount - start_tickcount);
#endif    
// !!!!!!!!!!!!

#if CONVERT_YUV_TO_RGB
    if (iColorConverter)
    {
        iColorConverter->Convert(aData, static_cast<uint8*>(mBufferHeap.heap->base()) + mFrameBuffers[mFrameBufferIndex]);
    }
#endif // 

#else // USE_PMEM

    // memory copy
    memcpy(static_cast<uint8*>(mBufferHeap.heap->base()) + mFrameBuffers[mFrameBufferIndex], aData, aDataLen);
    
#endif // USE_PMEM


    // post to SurfaceFlinger
#if USE_PMEM    

#if 1
    if (mAVSyncCtrl != NULL)
    {
        mAVSyncCtrl->UnlockSyncMutex();
    }
#endif  

    //LOGD("PBI(%d)", mPBISeqNum);
    OsclProfile::Push(OsclProfile::iPostBufferInTime, mPBISeqNum);

    mPBISeqNum++;
    
#if (!defined(MT6516_MP4_HW_DECODER_V2) && !defined(MT6573_MFV_HW))
    LogProfile(_seqNum);    
#endif    

    m_iPostBufferTime = OsclTickCount::getTimeMs();

#if !CONVERT_YUV_TO_RGB

    //<-- Honda add
    // Don't sleep here since clock32 may change up to 100ms during sleep here
    // Let Pv_media_output_node_input.cpp to handle the Early Frame
	//uint32 delta = data_header_info.aDelat;
    //if ((delta > _DISPLAY_MARGIN_) && (!iExitSurfaceThread))
    //{
    //    usleep((delta - _SLEEP_MARGIN_) * 1000);
    //}
    //-->
    LOGP("post buffer(%d) in: %x", gettid(), pmemInfo.offset);
    mSurface->postBuffer(pmemInfo.offset);
    LOGP("post buffer(%d) out: %x", gettid(), pmemInfo.offset);
    mCurBufferIndex = pmemInfo.offset;

    iThreadSafeSurfaceOutputAO->notify(SURFACE_END, pmemInfo.offset);
#else
    mSurface->postBuffer(mFrameBuffers[mFrameBufferIndex]);
    mCurBufferIndex = mFrameBuffers[mFrameBufferIndex];
#endif

#else // USE_PMEM
    mSurface->postBuffer(mFrameBuffers[mFrameBufferIndex]);
#endif

    if (++mFrameBufferIndex == kBufferCount) mFrameBufferIndex = 0;

    LOGP("writeFrameBuf out");
    
    return PVMFSuccess;
}

OSCL_EXPORT_REF void AndroidSurfaceOutput::closeFrameBuf()
{
    LOGP("closeFrameBuf");
    if (!mInitialized) return;

    mInitialized = false;
    if (mSurface.get()) 
    {
        LOGP("unregisterBuffers");
        mSurface->unregisterBuffers();
    }

    // free frame buffers
    LOGP("free frame buffers");
    // Honda [
	if (mFrameBuffers)
	{
	    delete [] mFrameBuffers;
	    mFrameBuffers = NULL;
	}
    //for (int i = 0; i < kBufferCount; i++) 
    //{
    //    mFrameBuffers[i] = 0;
    //}
    // ]

    // free heaps
    LOGP("free frame heap");
    mBufferHeap.heap.clear();
#if USE_PMEM  
    //Honda add    
    if (mHeap.get())    
    {        
        mHeap = NULL;    
    }
#endif    
    // free color converter
    if (iColorConverter != 0)
    {
        LOGP("free color converter");
        delete iColorConverter;
        iColorConverter = 0;
    }
}

OSCL_EXPORT_REF bool AndroidSurfaceOutput::GetVideoSize(int *w, int *h) 
{

    *w = iVideoDisplayWidth;
    *h = iVideoDisplayHeight;
    return iVideoDisplayWidth != 0 && iVideoDisplayHeight != 0;
}

void AndroidSurfaceOutput::ResetMIO()
{
    LOGP("***** ResetMIO *****");
 
    mSeqNum = 0;
    mPBISeqNum = 0;
    mDropFrameCount = 0;

    //ClearBuffer();
}

void AndroidSurfaceOutput::ClearBuffer()
{
    LOGP("ClearBuffer in");
    returnAllBuffers();
    LOGP("ClearBuffer out");
}

void AndroidSurfaceOutput::SetAVSyncCtrl(PVMFAVSyncbase* aAVSyncCtrl)
{
    LOGP("SetAVSyncCtrl");
    mAVSyncCtrl = aAVSyncCtrl;
}

void AndroidSurfaceOutput::notify(int msg, int ext1/*, int ext2*/)
{
    LOGP("!!!!! notify message received msg=%d, ext1=0x%x !!!!!", msg, ext1);
    
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    //Mutex::Autolock _l(mLock);
#endif    

    switch(msg)
    {
    case SURFACE_END:
        HandleRetBuf((uint8*)ext1);
        break;

    default:
        break;
    }

    RunIfNotReady();
}

void AndroidSurfaceOutput::HandleRetBuf(uint8* addr)
{
    LOGP("HandleRetBuf in");
    int err;

#if 0                                                                                                                                                                        
    iDispRequestQueueLock.Lock();
    if (iDispRequestQueue.size() != 0)
    {
        LOGP("HRB: signal display");
        triggerOutputThread();
    }
    iDispRequestQueueLock.Unlock();
#endif

    BufRetMap::iterator iterator = m_BufRetMap.find(addr);

    if (m_BufRetMap.size() == 0)
    {
        LOGP("HandleRetBuf - m_BufRetMap.size() == 0");
        
        return;
    }
    
    if (m_BufRetMap.end() == iterator)
    {
        LOGE("AndroidSurfaceOutput::HandleRetBuf - Invalid address: 0x%x !", (unsigned)addr);
        return;
    }

    m_iGetNotifyTime = OsclTickCount::getTimeMs();    
        
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    if (mAVSyncCtrl != NULL)
    {
        uint32 _AudioDelayTime = (uint32)(m_iGetNotifyTime - m_iPostBufferTime);
        mAVSyncCtrl->SetAudioDelayTime(_AudioDelayTime);
    }
    OsclProfile::Push(OsclProfile::iReturnBufferInTime, mSeqNum);

    LOG_POSTINFO("post frame use time %d", (uint32)(m_iGetNotifyTime - m_iPostBufferTime));
    
    LogProfile(mSeqNum);
    mSeqNum++;
#endif 

    LOGP("AndroidSurfaceOutput::HandleRetBuf - addr 0x%x", (unsigned)addr);
    BufRetEntry entry = (*iterator).second;
    m_BufRetMap.erase(iterator);

    WriteResponse resp(entry.m_status, entry.m_cmdid, entry.m_Context, entry.m_Timestamp);
    iWriteResponseQueue.push_back(resp);

#ifdef MT6516_MP4_HW_DECODER_V2
    //[20100421] Jackal
    //Mark this because the mechanism of mm_queue is changed!!
    //Oscl_MM_Queue::PopFrameFromTop();
#endif
}

void AndroidSurfaceOutput::RequestAndWaitForThreadExit()
{
    if (bDisableOutputThread)
    {
        return;
    }
    
    LOGP("RequestAndWaitForThreadExit In");
    if (mInitialized && !iExitSurfaceThread) {
        LOGP("signal thread for exit");
        iExitSurfaceThread = true;
        triggerOutputThread();
        while (iSurfaceThreadTermSem->Wait() != OsclProcStatus::SUCCESS_ERROR)
            ;
        LOGP("thread term signal received");
        //[ALPS00008196]To solve gallery 3D frame corrupt by honda
        //mInitialized = false;
        //~
    }
}

OSCL_EXPORT_REF PVMFStatus AndroidSurfaceOutput::checkRetBuf(uint8* aData)
{
#if PRESERVE_CURRENT_BUFFER
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    if (m_BufRetMap.size() != 0)
    {
        OsclProfile::Push(OsclProfile::iPostBufferInTime, mSeqNum);
        LogProfile(mSeqNum);
        mSeqNum++;
        mDropFrameCount ++;
        
#undef LOG_TAG
#define LOG_TAG "STATISTICS"         
        LOGE("!!!!! Warning: drop frame %d !!!!!", mDropFrameCount);
#undef LOG_TAG    
#define LOG_TAG MIO_TAG

        return PVMFInfoBufNotReturn;
    }
#endif    
#endif   

    return PVMFSuccess;
}

OSCL_EXPORT_REF PVMFStatus AndroidSurfaceOutput::pushBufToRequestQueue(                                                                                                      
    uint8* aData, uint32 aDataLen, const PvmiMediaXferHeader& data_header_info, OsclAny* aContext)
{
    LOGP("AndroidSurfaceOutput::pushBufToRequestQueue in");                                                                                                                  
    PVMFStatus status = PVMFSuccess;

#if 0
    iDispRequestQueueLock.Lock();
    if (iDispRequestQueue.size() >= 3)
    {
        status = PVMFInfoBufNotReturn;
    }
    iDispRequestQueueLock.Unlock();

    if (status != PVMFSuccess)
    {
        return status;
    }
#endif

    // push buf to queue
    BufQueueEntry* _entry = OSCL_NEW(BufQueueEntry, ());
    _entry->mData = aData;
    _entry->mDataLen = aDataLen;
    memcpy(&_entry->m_data_header_info, &data_header_info, sizeof(PvmiMediaXferHeader));
    _entry->mContext = aContext;
    _entry->mCmdid = iCommandCounter;
    
    iDispRequestQueueLock.Lock();
    iDispRequestQueue.push(_entry);
#if 0                                                                                                                                                                        
    if (iDispRequestQueue.size() == 1 && m_BufRetMap.size() == 0)
    {
        LOGP("PTQ: signal display");
        triggerOutputThread();
    }
#endif                                                                                                                                                                       
    iDispRequestQueueLock.Unlock();

    if (!bDisableOutputThread)
    {
        LOGP("!!!!! pushBufToRetQueue %x, %d, %d, %x, %d", aData, status, iCommandCounter, aContext, data_header_info.timestamp);
        pushBufToRetQueue(aData, status, iCommandCounter, aContext, data_header_info.timestamp);
    }
    
    LOGP("AndroidSurfaceOutput::pushBufToRequestQueue out");                                                                                                                 
    return status;
}

OSCL_EXPORT_REF PVMFStatus AndroidSurfaceOutput::handleDispQueue()
{
    LOGP("handleDispQueue(%d) in", gettid());
    PVMFStatus status = PVMFSuccess;

    // pop buf from queue
    iDispRequestQueueLock.Lock();
    bool empty = iDispRequestQueue.empty();

    BufQueueEntry* _entry = NULL;
    if (!empty)
    {
        _entry = iDispRequestQueue.front();
        iDispRequestQueue.pop();
    }
    iDispRequestQueueLock.Unlock();

    if (_entry == NULL)
    {
        return PVMFErrNoResources;
    }

    uint8* _Data = _entry->mData;
    uint32 _DataLen = _entry->mDataLen;
    PvmiMediaXferHeader _data_header_info;
    memcpy(&_data_header_info, &_entry->m_data_header_info, sizeof(PvmiMediaXferHeader));
    OsclAny* _Context = _entry->mContext;
    PVMFCommandId cmdid = _entry->mCmdid;
    
    uint32 aSeqNum=_data_header_info.seq_num;
    PVMFTimestamp aTimestamp=_data_header_info.timestamp;
    uint32 flags=_data_header_info.flags;

    //Schedule asynchronous response

#if !USE_BUF_QUEUE
    status = checkRetBuf(_Data);
#endif

    if (status == PVMFSuccess)
    {
        status = writeFrameBuf(_Data, _DataLen, _data_header_info);
    }

#if 0    
    if (bDisableOutputThread && status != PVMFInfoBufNotReturn)
    {
        LOGP("!!!!! pushBufToRetQueue %x, %d, %d, %x, %d", _Data, status, cmdid, _Context, aTimestamp);
        pushBufToRetQueue(_Data, status, cmdid, _Context, aTimestamp);
    }
#else
    
#endif

    if (_entry != NULL)
    {
        OSCL_DELETE(_entry);
    }
    LOGP("AndroidSurfaceOutput::handleDispQueue out");
    return status;
}

OSCL_EXPORT_REF void AndroidSurfaceOutput::returnAllBuffers()
{
    LOGP("returnAllBuffers in");
    iDispRequestQueueLock.Lock();

    while (!iDispRequestQueue.empty())
    {
        BufQueueEntry* _entry = iDispRequestQueue.front();
        uint8* _Data = _entry->mData;
        //HandleRetBuf(_Data);
        iDispRequestQueue.pop();

        OSCL_DELETE(_entry);
    }

    iDispRequestQueue.clear();
    iDispRequestQueueLock.Unlock();

    processWriteResponseQueue(0);

    LOGP("returnAllBuffers out");
}

void AndroidSurfaceOutput::pushBufToRetQueue(
    uint8* aData, PVMFStatus astatus, PVMFCommandId acmdid, OsclAny* aContext, PVMFTimestamp aTimestamp)
{
#if PRESERVE_CURRENT_BUFFER

#if USE_PMEM 
        PmemInfo pmemInfo;
        oscl_pmem_get_info(aData, &pmemInfo);
        m_DataAddr = (uint8 *)pmemInfo.offset;
#else
        m_DataAddr = aData;
#endif        
        BufRetEntry entry;
//        entry.m_pDataAddr = m_DataAddr;
        entry.m_status = astatus;
        entry.m_cmdid = acmdid;
        entry.m_Context = aContext;
        entry.m_Timestamp = aTimestamp;

        LOGP("!!!!! pmemInfo.offset: 0x%x !!!!!", m_DataAddr);
        BufRetMap::value_type value(m_DataAddr, entry);
        if (m_BufRetMap.insert(value).second == false)
        {
            LOGE("AndroidSurfaceOutput::writeAsync: Error - map insert failed");
            assert(false);
        }
#endif    
}

void AndroidSurfaceOutput::triggerOutputThread()
{
    if (!bDisableOutputThread)
    {
        if (iSurfaceThreadSem)
        {
            LOGP("triggerOutputThread(%d)", gettid());
            iSurfaceThreadSem->Signal();
        }        
    }
}

PVMFStatus AndroidSurfaceOutput::CheckFirstWriteFrameBuffer(void* aPmemInfo)
{
#if USE_PMEM      
    if (iIsFirstWriteFrameBuf)
    {      
        PmemInfo* _pmemInfo = (PmemInfo*)aPmemInfo;
        ColorConvertBase::PixelFormat _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
        PixelFormat pixelFormat = PIXEL_FORMAT_RGB_565;  
        
        iIsFirstWriteFrameBuf = false;
        
        LOGP("CheckFirstWriteFrameBuffer - first write frame buf, initialize memory heap base");
        // copy parameters in case we need to adjust them
        int displayWidth = iVideoDisplayWidth;
        int displayHeight = iVideoDisplayHeight;
        int frameWidth = iVideoWidth;
        int frameHeight = iVideoHeight;
        int frameSize;

        // RGB-565 frames are 2 bytes/pixel
        displayWidth = (displayWidth + 1) & -2;
        displayHeight = (displayHeight + 1) & -2;
        frameWidth = (frameWidth + 1) & -2;
        frameHeight = (frameHeight + 1) & -2;
        frameSize = frameWidth * frameHeight * 2;

        // create frame buffer heap and register with surfaceflinger
        LOGP("CheckFirstWriteFrameBuffer - fd: %d, base: 0x0%x, size: %d", 
            _pmemInfo->fd, _pmemInfo->base, _pmemInfo->size);              

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
        LOGD("!!!!!**************************************************!!!!!");
        if (iVideoSubFormat == PVMF_MIME_YUV420_PLANAR_MTKYUV)
        {
            LOGD("!!!!!          Output Color Format: MTK-YUV            !!!!!");
            pixelFormat = PIXEL_FORMAT_YUV_420_PLANER_MTK;
            _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER_MTK;
        }
        else if (iVideoSubFormat == PVMF_MIME_YUV420_PLANAR)
        {
            LOGD("!!!!!          Output Color Format: YUV-420            !!!!!");
            pixelFormat = PIXEL_FORMAT_YUV_420_PLANER;
            _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
        }
        // for default value
        else
        {
            LOGD("!!!!!          Output Color Format: YUV-420            !!!!!");
            pixelFormat = PIXEL_FORMAT_YUV_420_PLANER;
            _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
        }
        LOGD("!!!!!**************************************************!!!!!");
#if CONVERT_YUV_TO_RGB
        pixelFormat = PIXEL_FORMAT_RGB_565;
#endif
#endif

#if !CONVERT_YUV_TO_RGB        
        // Use MemoryHeapBaseMTK
        const char *pmemDev = "/dev/pmem_multimedia";
    #if(0)  
        sp<MemoryHeapBaseMTK> heap = new MemoryHeapBaseMTK(
            _pmemInfo->shared_fd, _pmemInfo->base, _pmemInfo->size, MemoryHeapBase::DONT_MAP_LOCALLY, pmemDev);
        if (heap->heapID() < 0) {
            LOGE("Error creating frame buffer heap");
            return PVMFFailure;
        }
        heap->incStrong(heap.get());
    #else//Honda Add
        mHeap = new MemoryHeapBaseMTK(
                    _pmemInfo->shared_fd, _pmemInfo->base, _pmemInfo->size, MemoryHeapBase::DONT_MAP_LOCALLY, pmemDev);
        if (mHeap->heapID() < 0) {
            LOGE("Error creating frame buffer heap");
            return PVMFFailure;
        }
    #endif
#else
        // create frame buffer heap and register with surfaceflinger
        sp<MemoryHeapBase> heap = new MemoryHeapBase(frameSize * kBufferCount);
        if (heap->heapID() < 0) {
            LOGE("Error creating frame buffer heap");
            return PVMFFailure;
        }
        
        // initialize software color converter
        iColorConverter = ColorConvert16::NewL();
        iColorConverter->Init(displayWidth, displayHeight, frameWidth, displayWidth, displayHeight, frameWidth, CCROTATE_NONE);
        //iColorConverter->Init(displayWidth, displayHeight, frameWidth, displayWidth, displayHeight, displayWidth, CCROTATE_NONE);
        iColorConverter->SetMemHeight(frameHeight);
        iColorConverter->SetMode(1);
        LOGP("SetInputBufFormat, %d", _pixelFormat);
        iColorConverter->SetInputBufFormat(_pixelFormat);
#endif 
      #if(0)
        mBufferHeap = ISurface::BufferHeap(displayWidth, displayHeight,
            frameWidth, frameHeight, pixelFormat, heap);
      #else//Honda add
      mBufferHeap = ISurface::BufferHeap(displayWidth, displayHeight,
                                         frameWidth, frameHeight, pixelFormat, mHeap/*heap*/);
      #endif
        mSurface->registerBuffers(mBufferHeap);
      #if defined(MTK_TVOUT_SUPPORT)
        mSurface->setLayerType(LAYER_TYPE_VIDEO);
      #endif

        // create frame buffers
        for (int i = 0; i < kBufferCount; i++) {
            mFrameBuffers[i] = i * frameSize;
        }        
    }
#endif

    return PVMFSuccess;
}

void AndroidSurfaceOutput::LogProfile(uint32 aSeqNum)
{
    if (!bSave_Profile && !bLog_Profile)
    {
        return;
    }
    
#undef LOG_TAG
#define LOG_TAG "playerprofile"

    long long _RetrieveTrackDataInTime              = OsclProfile::Pop(OsclProfile::iRetrieveTrackDataInTime, aSeqNum);
    long long _RetrieveTrackDataOutTime             = OsclProfile::Pop(OsclProfile::iRetrieveTrackDataOutTime, aSeqNum);
    long long _SendTrackDataOutTime                 = OsclProfile::Pop(OsclProfile::iSendTrackDataOutTime, aSeqNum);
    long long _SendInBufToOMXInTime                 = OsclProfile::Pop(OsclProfile::iSendInBufToOMXInTime, aSeqNum);
    long long _SendOutBufToOMXInTime                = OsclProfile::Pop(OsclProfile::iSendOutBufToOMXInTime, aSeqNum);
    long long _Mp4DecodeVideoInTime                 = OsclProfile::Pop(OsclProfile::iMp4DecodeVideoInTime, aSeqNum);
    long long _Mp4DecodeVideoOutTime                = OsclProfile::Pop(OsclProfile::iMp4DecodeVideoOutTime, aSeqNum);
    long long _CallbackFillBufferDoneInTime         = OsclProfile::Pop(OsclProfile::iCallbackFillBufferDoneInTime, aSeqNum);
    long long _ProcessCallbackFillBufferDoneInTime  = OsclProfile::Pop(OsclProfile::iProcessCallbackFillBufferDoneInTime, aSeqNum);
    long long _FillBufferDoneProcessingInTime       = OsclProfile::Pop(OsclProfile::iFillBufferDoneProcessingInTime, aSeqNum);
    long long _QueueOutputBufferInTime              = OsclProfile::Pop(OsclProfile::iQueueOutputBufferInTime, aSeqNum);
    long long _ReceiveYUVDataInTime                 = OsclProfile::Pop(OsclProfile::iReceiveYUVDataInTime, aSeqNum);
    long long _SendMediaDataInTime                  = OsclProfile::Pop(OsclProfile::iSendMediaDataInTime, aSeqNum);
    long long _PostBufferInTime                     = OsclProfile::Pop(OsclProfile::iPostBufferInTime, aSeqNum);
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    long long _SurfaceFlingerNotifyInTime           = OsclProfile::Pop(OsclProfile::iSurfaceFlingerNotifyInTime, aSeqNum);
    long long _ReturnBufferInTime                   = OsclProfile::Pop(OsclProfile::iReturnBufferInTime, aSeqNum);

    LOG_PROFILE(", %d, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld\n", 
        mSeqNum,
        _RetrieveTrackDataInTime, 
        _RetrieveTrackDataOutTime, 
        _SendTrackDataOutTime, 
        _SendInBufToOMXInTime, 
        _SendOutBufToOMXInTime, 
        _Mp4DecodeVideoInTime, 
        _Mp4DecodeVideoOutTime, 
        _CallbackFillBufferDoneInTime, 
        _ProcessCallbackFillBufferDoneInTime, 
        _FillBufferDoneProcessingInTime, 
        _QueueOutputBufferInTime, 
        _ReceiveYUVDataInTime, 
        _SendMediaDataInTime, 
        _PostBufferInTime, 
        _SurfaceFlingerNotifyInTime, 
        _ReturnBufferInTime);
#else
    LOG_PROFILE(", %d, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld, %lld\n", 
        mSeqNum,
        _RetrieveTrackDataInTime, 
        _RetrieveTrackDataOutTime, 
        _SendTrackDataOutTime, 
        _SendInBufToOMXInTime, 
        _SendOutBufToOMXInTime, 
        _Mp4DecodeVideoInTime, 
        _Mp4DecodeVideoOutTime, 
        _CallbackFillBufferDoneInTime, 
        _ProcessCallbackFillBufferDoneInTime, 
        _FillBufferDoneProcessingInTime, 
        _QueueOutputBufferInTime, 
        _ReceiveYUVDataInTime, 
        _SendMediaDataInTime, 
        _PostBufferInTime);
#endif


    uint32 _decdelta = 0;
    uint32 _dspdelta = 0;
    if (iDecTheshold != 0)
    {
        _decdelta = _ProcessCallbackFillBufferDoneInTime - _SendOutBufToOMXInTime;
    }

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    if (iDspThreshold != 0)
    {
        _dspdelta = _ReturnBufferInTime - _PostBufferInTime;
    }
#endif

    if (bEnableFTrace && iSeqNumThreshold > 0)
    {
        if (((_decdelta > iDecTheshold)||(_dspdelta > iDspThreshold)) && (aSeqNum > iSeqNumThreshold))
        {
            if (_decdelta > iDecTheshold)
            {
                LOGD("seq %d - delta: %d, %d", aSeqNum, _decdelta, iDecTheshold);
            }
            else if (_dspdelta > iDspThreshold)
            {
                LOGD("seq %d - delta: %d, %d", aSeqNum, _dspdelta, iDspThreshold);
            }
            bEnableFTrace = false;
            Oscl_Trace::end();
        }
    }
#undef LOG_TAG    
#define LOG_TAG MIO_TAG
}

////////////////////////////////////////////////////////////////////////////////////////////////
#undef LOG_TAG
#define LOG_TAG "ThreadSafeSurfaceOutputAO"

ThreadSafeSurfaceOutputAO::ThreadSafeSurfaceOutputAO(void* aObserver,
        uint32 aDepth,
        const char* aAOname,
        int32 aPriority)
        : ThreadSafeCallbackAO(aObserver, aDepth, aAOname, aPriority)
        , iSeqNum(0)                                                                                                                                                         
{
    LOGP("constructor");
}

ThreadSafeSurfaceOutputAO::~ThreadSafeSurfaceOutputAO()
{
    LOGP("destructor");
}

OsclReturnCode ThreadSafeSurfaceOutputAO::ProcessEvent(OsclAny* EventData)
{
    LOGP("!!!!! ProcessEvent in ");
    // In this case, ProcessEvent calls the method of the primary test AO to process the Event
    if (iObserver != NULL)
    {
        AndroidSurfaceOutput* ptr = (AndroidSurfaceOutput*) iObserver;

        if (ptr->IsAdded())
        {
            SurfaceOutputReturnData* pData = (SurfaceOutputReturnData*)EventData;
            if (pData != NULL)
            {
                ptr->notify(pData->mMsg, pData->mExt1);
                delete pData;
            }
        }
    }
    else
    {
        LOGE("iObserver is NULL");
    }
    return OsclSuccess;
}

void ThreadSafeSurfaceOutputAO::notify(int msg, int ext1/*, int ext2*/)
{
#if SHOW_PROFILE_ABOUT_SYNC    
#undef LOG_TAG
#define LOG_TAG PROFILE_TAG
    LOGD("NTF: %lld", OsclTickCount::getTimeMs());
#undef LOG_TAG    
#define LOG_TAG MIO_TAG
#endif

    if (iObserver != NULL)
    {
        AndroidSurfaceOutput* ptr = (AndroidSurfaceOutput*) iObserver;
        if (ptr->IsAdded())
        {
            OsclProfile::Push(OsclProfile::iSurfaceFlingerNotifyInTime, iSeqNum);                                                                                            
            iSeqNum++;                                                                                                                                                       
            
            ptr->triggerOutputThread();                                                                                                                                      
        }
    }

    LOGP("!!!!! notify message received msg=%d, ext1=0x%x !!!!!", msg, ext1);
    
    SurfaceOutputReturnData* _Data = new SurfaceOutputReturnData;
    _Data->mMsg = msg;
    _Data->mExt1 = ext1;

    OsclReturnCode err = ReceiveEvent(_Data);
    if (err != OsclSuccess)
    {
        LOGE("ReceiveEvent error: %d", err);
    }
}

//------------------------------------------------------------------------
// surface thread
//

#undef LOG_TAG
#define LOG_TAG "surfacethread"

// this is the surface output thread
// used to send data to the linux surface output device
// communicates with the surface MIO via a semaphore, a request queue and a response queue
/*static*/ int AndroidSurfaceOutput::start_surfout_thread_func(TOsclThreadFuncArg arg)
{
    LOGP("start_surfout_thread_func in");
    AndroidSurfaceOutput *obj = (AndroidSurfaceOutput *)arg;
    prctl(PR_SET_NAME, (unsigned long) "surface out", 0, 0, 0);
    int err = obj->surfout_thread_func();
    LOGP("start_surfout_thread_func out return code %d",err);
    return err;
}

int AndroidSurfaceOutput::surfout_thread_func()
{
    LOGP("surfout_thread_func");
    
    // add by Ryan
    // adjust Audio out  priority
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    // [Honda] use Keene's RR setting
    //sched_p.sched_priority = 80;
    sched_p.sched_priority = RTPM_PRIO_SURFACE_OUT;

    if(0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } else {
        sched_p.sched_priority = 0;
        sched_getparam(0, &sched_p);
        LOGP("sched_setscheduler ok, priority: %d", sched_p.sched_priority);
    }

    while (1)
    {
        if (iSurfaceThreadSem)
        {
            LOGP("iSurfaceThreadSem->Wait() in");
            iSurfaceThreadSem->Wait();
            PVMFStatus _status = handleDispQueue();                                                                                                                          
            if (_status != PVMFSuccess)                                                                                                                                      
            {                                                                                                                                                                
                LOGP("output warning(%d)", _status);                                                                                                                         
            }                                                                                                                                                                
            LOGP("iSurfaceThreadSem->Wait() out");
        }

        // check for exit signal
        if (iExitSurfaceThread) {
            LOGP("exit received");
            //if (len) sendResponse(cmdid, context, timestamp);
            break;
        }
    }

    iExitSurfaceThread = false;
    
    LOGP("surfout_thread_func exit");
    iSurfaceThreadTermSem->Signal();
    
    return 0;
}
