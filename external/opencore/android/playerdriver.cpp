

//#define LOG_NDEBUG 0
#define LOG_TAG "PlayerDriver"
#include <utils/Log.h>

#include <sys/prctl.h>
#include <sys/resource.h>
// TODO: I don't think the include below is needed.
#include <media/mediaplayer.h>
#include <media/thread_init.h>
#include <utils/threads.h>
#include <utils/List.h>
#include <utils/SortedVector.h>
#include <cutils/properties.h>
#include <binder/Parcel.h>
#include <utils/Timers.h>

#include <media/MediaPlayerInterface.h>
#include <media/Metadata.h>

#include "playerdriver.h"
#include <media/PVPlayer.h>

#include "oscl_base.h"
#include "oscl_exception.h"
#include "oscl_scheduler.h"
#include "oscl_scheduler_ao.h"
#include "oscl_exception.h"
#include "oscl_mem_basic_functions.h"  // oscl_memcpy
#include "oscl_mem.h"
#include "oscl_mem_audit.h"
#include "oscl_error.h"
#include "oscl_utf8conv.h"
#include "oscl_string_utils.h"

#include "media_clock_converter.h"

#include "pv_player_factory.h"
#include "pv_player_interface.h"
#include "pv_engine_observer.h"
#include "pvmi_media_io_fileoutput.h"
#include "pv_player_datasourceurl.h"
#include "pv_player_datasinkfilename.h"
#include "pv_player_datasinkpvmfnode.h"
#include "pvmf_errorinfomessage_extension.h"
#include "pvmf_duration_infomessage.h"
#include "android_surface_output.h"
#include "android_audio_output.h"
#include "android_audio_stream.h"
#include "pv_media_output_node_factory.h"
#include "pvmf_format_type.h"  // for PVMFFormatType
#include "pvmf_node_interface.h"
#include "pvmf_source_context_data.h"
#include "pvmf_download_data_source.h"
#include "OMX_Core.h"
#include "pv_omxcore.h"
// <--- Demon Deng
#include "pvmf_sm_node_events.h"
#include "pvmf_socket_node_events.h"
// --->

// color converter
#include "cczoomrotation16.h"

//for KMJ DRM Plugin
#include "pvmf_local_data_source.h"
#include "pvmf_recognizer_registry.h"

#include <sched.h>

#include <sys/time.h>
#include "oscl_file_cfg.h"
#include "threadsafe_callback_ao.h"

#ifndef OSCL_PMEM_H_INCLUDED
#include "oscl_pmem.h"
#endif

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
// [Honda] use Keene's RR setting
#include <linux/rtpm_prio.h>
#endif
#undef LOG_TAG
#define LOG_TAG "PlayerDriver"



using namespace android;

#define LOG_PROFILE(...) LOGV(__VA_ARGS__)

static bool bShowLog = false;
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

extern bool oscl_pmem_show_addr_map(void);

#define USE_THREAD_SAFE_ENQUEUE 1
#define PROXY_NAME "name"
#define PROXY_PORT "port"
#define PROXY_ID "id"
#define PROXY_PASSWORD "password"
#define PROXY_CFG_PATH "/data/http-proxy-cfg"
#define MAX_CFG_STRING_LENGTH 50

static uint32 iDebugTest = 0;
#define debugger_test(x) \
{\
    if (x == iDebugTest)\
    {\
        LOGD("iDebugTest = %d", iDebugTest);\
        _OSCL_Abort();\
    }\
}

# ifndef PAGESIZE
#  define PAGESIZE              4096
# endif

// library and function name to retrieve device-specific MIOs
static const char* MIO_LIBRARY_NAME = "libopencorehw.so";
static const char* VIDEO_MIO_FACTORY_NAME = "createVideoMio";
typedef AndroidSurfaceOutput* (*VideoMioFactory)();

static const nsecs_t kBufferingUpdatePeriod = seconds(10);

namespace {


// For the event's buffer format is:
//                     1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        buffering percent                      |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// @param buffer Pointer to the start of the buffering status data.
// @param size Of the buffer
// @param[out] percentage On return contains the amout of buffering.
//                        The value is in [0,100]
// @return true if a valid buffering update was found. false otherwise.
bool GetBufferingPercentage(const void *buffer,
                            const size_t size,
                            int *percentage)
{
    if (buffer == NULL) {
        LOGE("Invalid buffer: NULL");
        return false;
    }
    if (sizeof(int) != size)
    {
        LOGE("Invalid percentage buffer size %d (expected %d)", size, sizeof(int));
        return false;
    }
    // TODO: The PVEvent class should expose a memcopy method
    // that does bound checking instead of having clients reaching
    // for its internal buffer.
    oscl_memcpy(percentage, buffer, sizeof(int));

    // Clamp the value and complain loudly.
    if (*percentage < 0 || *percentage > 100)
    {
        LOGE("Invalid percentage value %d", *percentage);
        return false;
    }
    return true;
}

// Macro used in a switch statement to convert a PlayerCommand code into its
// string representation.
#ifdef CONSIDER_CODE
#error "CONSIDER_CODE already defined!!"
#endif
#define CONSIDER_CODE(val) case ::PlayerCommand::val: return #val

// Convert a command code into a string for logging purposes.
// @param code Of the command.
// @return a string representation of the command type.
const char *PlayerCommandCodeToString(PlayerCommand::Code code) {
    switch (code) {
        CONSIDER_CODE(PLAYER_QUIT);
        CONSIDER_CODE(PLAYER_SETUP);
        CONSIDER_CODE(PLAYER_SET_DATA_SOURCE);
        CONSIDER_CODE(PLAYER_SET_VIDEO_SURFACE);
        CONSIDER_CODE(PLAYER_SET_AUDIO_SINK);
        CONSIDER_CODE(PLAYER_INIT);
        CONSIDER_CODE(PLAYER_PREPARE);
        CONSIDER_CODE(PLAYER_START);
        CONSIDER_CODE(PLAYER_STOP);
        CONSIDER_CODE(PLAYER_PAUSE);
        CONSIDER_CODE(PLAYER_RESET);
        CONSIDER_CODE(PLAYER_SET_LOOP);
        CONSIDER_CODE(PLAYER_SEEK);
        CONSIDER_CODE(PLAYER_GET_POSITION);
        CONSIDER_CODE(PLAYER_GET_DURATION);
        CONSIDER_CODE(PLAYER_GET_STATUS);
        CONSIDER_CODE(PLAYER_REMOVE_DATA_SOURCE);
        CONSIDER_CODE(PLAYER_CANCEL_ALL_COMMANDS);
        // <--- Demon Deng
        CONSIDER_CODE(PLAYER_GET_METADATA);
        // --->
        default: return "UNKNOWN PlayerCommand code";
    }
}
#undef CONSIDER_CODE

// Map a PV status code to a message type (error/info/nop)
// @param status PacketVideo status code as defined in pvmf_return_codes.h
// @return the corresponding android message type. MEDIA_NOP is used as a default.
::android::media_event_type MapStatusToEventType(const PVMFStatus status) {
    if (status <= PVMFErrFirst) {
        return ::android::MEDIA_ERROR;
    } else if (status >= PVMFInfoFirst) {
        return ::android::MEDIA_INFO;
    } else {
        return ::android::MEDIA_NOP;
    }
}

// Map a PV status to an error/info code.
// @param status PacketVideo status code as defined in pvmf_return_codes.h
// @return the corresponding android error/info code.
int MapStatusToEventCode(const PVMFStatus status) {
    switch (status) {
// <--- Demon Deng
        case PVMFErrNetwork:
            return ::android::MEDIA_ERROR_CANNOT_CONNECT_TO_SERVER;
        case PVMFErrResource:
            return ::android::MEDIA_ERROR_BAD_FILE;
        case PVMFErrResourceConfiguration:
            return ::android::MEDIA_ERROR_TYPE_NOT_SUPPORTED;
// --->
        case PVMFErrContentInvalidForProgressivePlayback:
            return ::android::MEDIA_ERROR_NOT_VALID_FOR_PROGRESSIVE_PLAYBACK;

        default:
            // Takes advantage that both error and info codes are mapped to the
            // same value.
            assert(::android::MEDIA_ERROR_UNKNOWN == ::android::MEDIA_INFO_UNKNOWN);
            return ::android::MEDIA_ERROR_UNKNOWN;
    }
}

}  // anonymous namespace

////////////////////////////////////////////////////////////////////////////////////////////////

const char ThreadSafeEnqueueAOName[] = "ThreadSafeEnqueueAO";
class ThreadSafeEnqueueAO : public ThreadSafeCallbackAO
{
    public:
        // Constructor
        ThreadSafeEnqueueAO(
            void* aObserver = NULL,
            uint32 aDepth = DEFAULT_QUEUE_DEPTH,
            const char* aAOname = ThreadSafeEnqueueAOName,
            int32 aPriority = OsclActiveObject::EPriorityNominal);


        // OVERLOADED ProcessEvent
        OsclReturnCode ProcessEvent(OsclAny* EventData);
        virtual ~ThreadSafeEnqueueAO();
};

////////////////////////////////////////////////////////////////////////////////////////////////

const char PlayerDriverTimerAOName[] = "PlayerDriverTimerAO";
class PlayerDriverTimerAO : public OsclTimerObject
{
public:
    
    PlayerDriverTimerAO(void* aObserver = NULL,
            const char* aAOname = PlayerDriverTimerAOName,
            int32 aPriority = OsclActiveObject::EPriorityNominal);
    ~PlayerDriverTimerAO();    

    static void syncCompletion(status_t s, void *cookie, bool cancelled);
    
protected:
    virtual void Run();
    
    void ThreadLogon();
    void ThreadLogoff();

    void *iObserver;
};

#define _PLAYER_DRIVER_SHOW_PMEM_INFO_

class PlayerDriver :
        public OsclActiveObject,
        public PVCommandStatusObserver,
        public PVInformationalEventObserver,
        public PVErrorEventObserver
{
  public:
    PlayerDriver(PVPlayer* pvPlayer);
    ~PlayerDriver();

    PlayerCommand* dequeueCommand();
    status_t enqueueCommand(PlayerCommand* code);

    // Dequeues a code from MediaPlayer and gives it to PVPlayer.
    void Run();

    // Handlers for the various commands we can accept.
    void commandFailed(PlayerCommand* command);
    void handleSetup(PlayerSetup* command);
    void handleSetDataSource(PlayerSetDataSource* command);
    void handleSetVideoSurface(PlayerSetVideoSurface* command);
    void handleSetAudioSink(PlayerSetAudioSink* command);
    void handleInit(PlayerInit* command);
    void handlePrepare(PlayerPrepare* command);
    void handleStart(PlayerStart* command);
    void handleStop(PlayerStop* command);
    void handlePause(PlayerPause* command);
    void handleSeek(PlayerSeek* command);
    void handleCancelAllCommands(PlayerCancelAllCommands* command);
    void handleRemoveDataSource(PlayerRemoveDataSource* command);
    void handleReset(PlayerReset* command);
    void handleQuit(PlayerQuit* command);
    void handleGetPosition(PlayerGetPosition* command);
    void handleGetDuration(PlayerGetDuration* command);
    void handleGetStatus(PlayerGetStatus* command);
    void handleCheckLiveStreaming(PlayerCheckLiveStreaming* cmd);
    // <--- Demon Deng 
    void handleGetMetadata(PlayerGetMetadata* cmd);
    // --->

    //void endOfData();
    PVMFFormatType getFormatType();
    void CommandCompleted(const PVCmdResponse& aResponse);
    void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
    void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

    bool prepareDone() const { return mPrepareDone; }
    bool isLiveStreaming() const { return mIsLiveStreaming; }

  private:
    // Finish up a non-async code in such a way that
    // the event loop will keep running.
    void FinishSyncCommand(PlayerCommand* command);

    void handleGetDurationComplete(PlayerGetDuration* cmd);
    void handleCheckLiveStreamingComplete(PlayerCheckLiveStreaming* cmd);
// <--- Demon Deng
    void handleGetMetadataComplete(PlayerGetMetadata* cmd);
// --->

// <--- Morris Yang 20101004 for OMA DM
    bool getRtspStreamingParam(const char* key, char* value);
    bool checkRtspStreamingConfig();
    int setupRtspStreaming();
// --->
    int setupHttpStreamPre();
    int setupHttpStreamPost();

    // Starts the PV scheduler thread.
    static int startPlayerThread(void *cookie);
    static int startMountMonitorThread(void *pData);
    static int stopMountMonitorThread(void *pData);
    int playerThread();

    // Callback for synchronous commands.
    static void syncCompletion(status_t s, void *cookie, bool cancelled);

    PVPlayer                *mPvPlayer;
    PVPlayerInterface       *mPlayer;
    PVPlayerDataSourceURL   *mDataSource;

    PVPlayerDataSink        *mAudioSink;
    PVMFNodeInterface       *mAudioNode;
    AndroidAudioMIO         *mAudioOutputMIO;

    PVPlayerDataSink        *mVideoSink;
    PVMFNodeInterface       *mVideoNode;
    AndroidSurfaceOutput    *mVideoOutputMIO;

    PvmiCapabilityAndConfig *mPlayerCapConfig;

    OSCL_wHeapString<OsclMemAllocator> mDownloadFilename;
    OSCL_HeapString<OsclMemAllocator> mDownloadProxy;
    OSCL_wHeapString<OsclMemAllocator> mDownloadConfigFilename;
    PVMFSourceContextData   *mDownloadContextData;

    PVPMetadataList mMetaKeyList;
    Oscl_Vector<PvmiKvp,OsclMemAllocator> mMetaValueList;
    int mNumMetaValues;
    PVPMetadataList mCheckLiveKey;
    Oscl_Vector<PvmiKvp,OsclMemAllocator> mCheckLiveValue;
    int mCheckLiveMetaValues;

    // Semaphore used for synchronous commands.
    OsclSemaphore           *mSyncSem;
    // Status cached by syncCompletion for synchronous commands.
    status_t                mSyncStatus;

    // Command queue and its lock.
    List<PlayerCommand*>    mCommandQueue;
    Mutex                   mQueueLock;

    bool                    mIsLooping;
    bool                    mDoLoop;
    bool                    mDataReadyReceived;
    bool                    mPrepareDone;
    bool                    mEndOfData;
    int                     mRecentSeek;
    bool                    mSeekComp;
    bool                    mSeekPending;
    bool                    mIsLiveStreaming;
    bool                    mEmulation;
    bool                    mContentLengthKnown;
    void*                   mLibHandle;
    nsecs_t                 mLastBufferingLog;

    // video display surface
    android::sp<android::ISurface> mSurface;

    // add by Ryan
public:
    status_t enqueueCommandDirectly(PlayerCommand* code);
    status_t enqueueCommandThreaadSafe(PlayerCommand* code);

private:
    // add by Ryan
    ThreadSafeEnqueueAO*     mThreadSafeEnqueueAO;
    PlayerDriverTimerAO*     mPlayerDriverTimerAO;

    Mutex                    mRunLock;

    // Streaming
    int mDownloadProxyPort;
    OSCL_HeapString<OsclMemAllocator> mDownloadProxyId;
    OSCL_HeapString<OsclMemAllocator> mDownloadProxyPassword;
    
    PVMFSourceContextData   mAsyncContextData;

    bool loadProxyConfig();
    int getProxySetting(char* szString, char* szName, char* szValue);    

    uint32 iUserData;
    bool bIsRtspStreaming;
};

PlayerDriver::PlayerDriver(PVPlayer* pvPlayer) :
        OsclActiveObject(OsclActiveObject::EPriorityNominal, "PVPlayerPlayer"),
        mPvPlayer(pvPlayer),
        mIsLooping(false),
        mDoLoop(false),
        mDataReadyReceived(false),
        mPrepareDone(false),
        mEndOfData(false),
        mRecentSeek(0),
        mSeekComp(true),
        mSeekPending(false),
        mIsLiveStreaming(false),
        mEmulation(false),
        mContentLengthKnown(false),
        mLastBufferingLog(0)
{
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_playerdriver");
    iDebugTest = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "debug_test");
    LOG_PROFILE("constructor, %lld", OsclTickCount::getTimeMs());
    
    LOGP("constructor");
#ifdef _PLAYER_DRIVER_SHOW_PMEM_INFO_
    LOGD("===== PlayerDriver constructor show P-MEM info =====");
    oscl_pmem_show_addr_map();
    LOGD("====================================================");
#endif

    mSyncSem = new OsclSemaphore();
    mSyncSem->Create();

    mDataSource = NULL;
    mAudioSink = NULL;
    mAudioNode = NULL;
    mAudioOutputMIO = NULL;
    mVideoSink = NULL;
    mVideoNode = NULL;
    mVideoOutputMIO = NULL;
    mSurface = NULL;

    mPlayerCapConfig = NULL;
    mDownloadContextData = NULL;

    mThreadSafeEnqueueAO = NULL;
    mPlayerDriverTimerAO = NULL;
    
    iUserData = 0;
    bIsRtspStreaming = false;
    
    // running in emulation?
    mLibHandle = NULL;
    char value[PROPERTY_VALUE_MAX];
    if (property_get("ro.kernel.qemu", value, 0)) {
        mEmulation = true;
        LOGP("Emulation mode - using software codecs");
    } else {
        // attempt to open h/w specific library
        mLibHandle = ::dlopen(MIO_LIBRARY_NAME, RTLD_NOW);
        if (mLibHandle != NULL) {
            LOGP("OpenCore hardware module loaded");
        } else {
            LOGP("OpenCore hardware module not found");
        }
    }

    // start player thread
    LOGP("start player thread");
    LOG_PROFILE("start player thread, %lld", OsclTickCount::getTimeMs());
    createThreadEtc(PlayerDriver::startPlayerThread, this, "PV player");

    createThreadEtc(PlayerDriver::startMountMonitorThread, mPvPlayer, "PV MountMonitor");
    // mSyncSem will be signaled when the scheduler has started
    mSyncSem->Wait();
}

PlayerDriver::~PlayerDriver()
{
    LOGP("destructor");
    if (mLibHandle != NULL) {
        ::dlclose(mLibHandle);
    }
    if (mThreadSafeEnqueueAO)
    {
        OSCL_DELETE(mThreadSafeEnqueueAO);
        mThreadSafeEnqueueAO = NULL;
    }
    if (mPlayerDriverTimerAO)
    {
        OSCL_DELETE(mPlayerDriverTimerAO);
        mPlayerDriverTimerAO = NULL;
    }
    
#ifdef _PLAYER_DRIVER_SHOW_PMEM_INFO_
    LOGD("===== PlayerDriver destructor show P-MEM info =====");
    oscl_pmem_show_addr_map();
    LOGD("===================================================");
#endif
}

PlayerCommand* PlayerDriver::dequeueCommand()
{
    PlayerCommand* command;

    mQueueLock.lock();

    // XXX should we assert here?
    if (mCommandQueue.empty()) {
#if !USE_THREAD_SAFE_ENQUEUE          
        PendForExec();
#endif   
        mQueueLock.unlock();
        return NULL;
    }

    command = *(--mCommandQueue.end());
    mCommandQueue.erase(--mCommandQueue.end());
    if (mCommandQueue.size() > 0 )
    {
        RunIfNotReady();
    }
    else
    {
#if !USE_THREAD_SAFE_ENQUEUE          
        PendForExec();
#endif
    }
    mQueueLock.unlock();

    return command;
}

status_t PlayerDriver::enqueueCommand(PlayerCommand* command)
{
#if USE_THREAD_SAFE_ENQUEUE    
    return enqueueCommandThreaadSafe(command);
#else
    return enqueueCommandDirectly(command);
#endif
}

status_t PlayerDriver::enqueueCommandDirectly(PlayerCommand* command)
{
    if (mPlayer == NULL) {
        // Only commands which can come in this use-case is PLAYER_SETUP and PLAYER_QUIT
        // The calling function should take responsibility to delete the command and cleanup
        return NO_INIT;
    }

    OsclSemaphore *syncsemcopy = NULL;

    // If the user didn't specify a completion callback, we
    // are running in synchronous mode.
#if !USE_THREAD_SAFE_ENQUEUE    
    if (!command->hasCallback()) {
        command->set(PlayerDriver::syncCompletion, this);
        // make a copy of this semaphore for special handling of the PLAYER_QUIT code
        syncsemcopy = mSyncSem;
    }
#endif

    // Add the code to the queue.
    mQueueLock.lock();
    mCommandQueue.push_front(command);

    // save code, since command will be deleted by the standard completion function
    int code = command->code();

    // AO needs to be scheduled only if this is the first cmd after queue was empty
#if !USE_THREAD_SAFE_ENQUEUE    
    if (mCommandQueue.size() == 1)
    {
        PendComplete(OSCL_REQUEST_ERR_NONE);
    }
#else
    RunIfNotReady();
#endif
    mQueueLock.unlock();

#if !USE_THREAD_SAFE_ENQUEUE
    // If we are in synchronous mode, wait for completion.
    if (syncsemcopy) {
        syncsemcopy->Wait();
        if (code == PlayerCommand::PLAYER_QUIT) {
            syncsemcopy->Close();
            delete syncsemcopy;
            return 0;
        }
        return mSyncStatus;
    }
#endif

    return OK;
}

status_t PlayerDriver::enqueueCommandThreaadSafe(PlayerCommand* command)
{
    if (mPlayer == NULL) {
        // Only commands which can come in this use-case is PLAYER_SETUP and PLAYER_QUIT
        // The calling function should take responsibility to delete the command and cleanup
        return NO_INIT;
    }    
    
    OsclSemaphore *syncsemcopy = NULL;

    // If the user didn't specify a completion callback, we
    // are running in synchronous mode.
    if (!command->hasCallback()) {
        command->set(PlayerDriver::syncCompletion, this);
        // make a copy of this semaphore for special handling of the PLAYER_QUIT code
        syncsemcopy = mSyncSem;
    }

    // save code, since command will be deleted by the standard completion function
    int code = command->code();
    // Add the code to the queue.    
    if (mThreadSafeEnqueueAO)
    {
        OsclReturnCode err = mThreadSafeEnqueueAO->ReceiveEvent(command);
        if (err != OsclSuccess)
        {
            LOGE("mThreadSafeEnqueueAO->ReceiveEvent error: %d", err);
        }
    }

    // If we are in synchronous mode, wait for completion.
    if (syncsemcopy) {
        syncsemcopy->Wait();
        if (code == PlayerCommand::PLAYER_QUIT) {
            syncsemcopy->Close();
            delete syncsemcopy;
            return 0;
        }
        return mSyncStatus;
    }

    return OK;
}

void PlayerDriver::FinishSyncCommand(PlayerCommand* command)
{
    command->complete(NO_ERROR, false);
    delete command;
}

// The OSCL scheduler calls this when we get to run (this should happen only
// when a code has been enqueued for us).
void PlayerDriver::Run()
{
    if (mDoLoop) {
        LOGP("!!!!! Loop PlayerDriver !!!!!");

        mEndOfData = false;
        mContentLengthKnown = false;
        PVPPlaybackPosition begin, end;
        begin.iIndeterminate = false;
        begin.iPosUnit = PVPPBPOSUNIT_SEC;
        begin.iPosValue.sec_value = 0;
        begin.iMode = PVPPBPOS_MODE_NOW;
        end.iIndeterminate = true;
        mPlayer->ResetEngineForLooping();
        mPlayer->SetPlaybackRange(begin, end, false, NULL);
        mPlayer->Resume();
        return;
    }

    PVPlayerState state = PVP_STATE_ERROR;
    if ((mPlayer->GetPVPlayerStateSync(state) == PVMFSuccess))
    {
        if (state == PVP_STATE_ERROR)
        {
            return;
        }
    }


    PlayerCommand* command;

    command = dequeueCommand();
    if (command) {
        LOGP("Send player code: %d", command->code());

        switch (command->code()) {
            case PlayerCommand::PLAYER_SETUP:
                handleSetup(static_cast<PlayerSetup*>(command));
                break;

            case PlayerCommand::PLAYER_SET_DATA_SOURCE:
                handleSetDataSource(static_cast<PlayerSetDataSource*>(command));
                break;

            case PlayerCommand::PLAYER_SET_VIDEO_SURFACE:
                handleSetVideoSurface(static_cast<PlayerSetVideoSurface*>(command));
                break;

            case PlayerCommand::PLAYER_SET_AUDIO_SINK:
                handleSetAudioSink(static_cast<PlayerSetAudioSink*>(command));
                break;

            case PlayerCommand::PLAYER_INIT:
                handleInit(static_cast<PlayerInit*>(command));
                break;

            case PlayerCommand::PLAYER_PREPARE:
                handlePrepare(static_cast<PlayerPrepare*>(command));
                break;

            case PlayerCommand::PLAYER_START:
                handleStart(static_cast<PlayerStart*>(command));
                break;

            case PlayerCommand::PLAYER_STOP:
                handleStop(static_cast<PlayerStop*>(command));
                break;

            case PlayerCommand::PLAYER_PAUSE:
                {
                    if(mIsLiveStreaming) {
                        LOGW("Pause denied");
                        FinishSyncCommand(command);
                        return;
                    }
                    handlePause(static_cast<PlayerPause*>(command));
                }
                break;

            case PlayerCommand::PLAYER_SEEK:
                {
                    if(mIsLiveStreaming) {
                        LOGW("Seek denied");
                        mPvPlayer->sendEvent(MEDIA_SEEK_COMPLETE);
                        FinishSyncCommand(command);
                        return;
                    }
                    handleSeek(static_cast<PlayerSeek*>(command));
                }
                break;

            case PlayerCommand::PLAYER_GET_POSITION:
                handleGetPosition(static_cast<PlayerGetPosition*>(command));
                FinishSyncCommand(command);
                return;

            case PlayerCommand::PLAYER_GET_STATUS:
                handleGetStatus(static_cast<PlayerGetStatus*>(command));
                FinishSyncCommand(command);
                return;

            case PlayerCommand::PLAYER_CHECK_LIVE_STREAMING:
                handleCheckLiveStreaming(static_cast<PlayerCheckLiveStreaming*>(command));
                break;

            case PlayerCommand::PLAYER_GET_DURATION:
                handleGetDuration(static_cast<PlayerGetDuration*>(command));
                break;

            case PlayerCommand::PLAYER_REMOVE_DATA_SOURCE:
                handleRemoveDataSource(static_cast<PlayerRemoveDataSource*>(command));
                break;

            case PlayerCommand::PLAYER_CANCEL_ALL_COMMANDS:
                handleCancelAllCommands(static_cast<PlayerCancelAllCommands*>(command));
                break;

            case PlayerCommand::PLAYER_RESET:
                handleReset(static_cast<PlayerReset*>(command));
                break;

            case PlayerCommand::PLAYER_QUIT:
                handleQuit(static_cast<PlayerQuit*>(command));
                return;

            case PlayerCommand::PLAYER_SET_LOOP:
                mIsLooping = static_cast<PlayerSetLoop*>(command)->loop();
                FinishSyncCommand(command);
                return;

                // <--- Demon Deng
            case PlayerCommand::PLAYER_GET_METADATA:
                handleGetMetadata(static_cast<PlayerGetMetadata*>(command));
                break;
                // --->

            default:
                LOGE("Unexpected code %d", command->code());
                break;
        }
    }

}

void PlayerDriver::commandFailed(PlayerCommand* command)
{
    if (command == NULL) {
        LOGP("async code failed");
        return;
    }

    LOGP("Command failed: %d", command->code());
    command->complete(UNKNOWN_ERROR, false);
    delete command;
}

void PlayerDriver::handleSetup(PlayerSetup* command)
{
    int error = 0;

    // Make sure we have the capabilities and config interface first.
    OSCL_TRY(error, mPlayer->QueryInterface(PVMI_CAPABILITY_AND_CONFIG_PVUUID,
                                            (PVInterface *&)mPlayerCapConfig, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

bool PlayerDriver::loadProxyConfig()
{
    FILE *pFile = fopen(PROXY_CFG_PATH, "r");			
    if (pFile != NULL)			
    {               
        char szString[MAX_CFG_STRING_LENGTH];              
        char* szName = NULL;               
        char* szValue = NULL;              
        while (fgets(szString, MAX_CFG_STRING_LENGTH, pFile) != NULL)              
        {                   
            int nIndex = getProxySetting(szString, szName, szValue);
        }
        LOGP("load http setting: proxy ip: %s, port: %d", mDownloadProxy.get_cstr(), mDownloadProxyPort);
#if 1
        LOGP("proxy account: %s", mDownloadProxyId.get_cstr());
#endif        
        fclose(pFile);          
    }
    else
    {
        return false;
    }

    return true;
}

int PlayerDriver::getProxySetting(char* szString, char* szName, char* szValue)
{		
    szName = strtok(szString, "="); 
    szValue = strtok(NULL, "\r\n");
    
    LOGP("getProxySetting: %s", szName);
    if (!strcmp(szName, PROXY_NAME))    
    {
        mDownloadProxy = _STRLIT_CHAR(szValue);     
        return 0;   
    }   
    else if (!strcmp(szName, PROXY_PORT))  
    {
        mDownloadProxyPort = atoi(szValue);
        return 1;   
    }   
    else if (!strcmp(szName, PROXY_ID))    
    {
        mDownloadProxyId = _STRLIT_CHAR(szValue);
        return 2;   
    }
    else if (!strcmp(szName, PROXY_PASSWORD))  
    {
        mDownloadProxyPassword = _STRLIT_CHAR(szValue);
        return 3;   
    }       

    return -1;
}

// <--- Morris Yang 20101004 for OMA DM
#define MAX_PARAM_LEN 128
#define EQUALSIGN "="
#define DELIMITERS "=\r\n"
#define STREAMING_CFG_FILE "/etc/Streaming.cfg"

bool PlayerDriver::checkRtspStreamingConfig()
{
    // check the file existence:
    struct stat stFileInfo; 
    if (stat(STREAMING_CFG_FILE, &stFileInfo) == 0)
    {
        return true;
    }

    return false;
}

bool PlayerDriver::getRtspStreamingParam(const char* key, char* value)
{
    FILE *fp;
    char str[MAX_PARAM_LEN];
    if((fp = fopen(STREAMING_CFG_FILE, "r")) == NULL)
    {
        LOGE ("Can't open %s", STREAMING_CFG_FILE);
        return false;
    }

    value[0] = '\0';

    while(!feof(fp))
    {
        if(fgets(str, MAX_PARAM_LEN, fp) == NULL)
        {
           fclose(fp);
           return false;
        }

        char* pch;
        char* pchtok;
        pch = strstr (str, key);
        if (pch == &str[0])
        {
            pch = strstr (str, EQUALSIGN);
            //LOGE ("found %s, value=%s\n", key, pch+1);
            if (strlen(pch) <= 1)
            {
                break;
            }

            pchtok = strtok (pch, DELIMITERS);
            while (pchtok != NULL)
            {
                //LOGE ("%s\n",pchtok);
                strcpy (value, pchtok);
                break;
                pchtok = strtok (NULL, DELIMITERS);
            }

            break;
        }
    }
    fclose(fp);
    return true;
}


static bool checkRTSPParam(char *value) {
    return value && strlen(value) && strcmp(value, "null") && strcmp(value, "NULL");
}

int PlayerDriver::setupRtspStreaming() 
{
    OSCL_wHeapString<OsclMemAllocator> _ProxyName = _STRLIT_WCHAR("");
    oscl_wchar _temp_wstr[50];
    int32 _MinUDPPort = 0x2000;
    int32 _MaxUDPPort = 0x4000;
    
    // <--- Demon Deng
    int32 _ProxyPort = 554;
#if 0
    if (false == checkRtspStreamingConfig())
    {
        LOGD ("%s doesn't exist, use default parameters", STREAMING_CFG_FILE);
    }
    else
    {
        LOGD ("Found %s", STREAMING_CFG_FILE);

        char value[50];

        // get the proxy
        getRtspStreamingParam("Streaming-Proxy", value);
        LOGD ("Streaming-Proxy=%s", value);
        int32 wtemplen = oscl_UTF8ToUnicode(value, strlen(value), _temp_wstr, 50);
         if (wtemplen > 0) _ProxyName.set(_temp_wstr, wtemplen);

        // get the min UDP port number
        getRtspStreamingParam("Streaming-MIN-UDP-PORT", value);
        int32 _tmpNum = atoi(value);

        // validate the min UDP port number:
        if (_tmpNum >= 1024 && _tmpNum <= 65534)
        {
            _MinUDPPort = _tmpNum;
        }
        LOGD ("Streaming-MIN-UDP-PORT=%d", _MinUDPPort);

        // get the max UDP port number
        getRtspStreamingParam("Streaming-MAX-UDP-PORT", value);
        _tmpNum = atoi(value);
        
        // validate the min UDP port number:
        if (_tmpNum >= 1024 && _tmpNum <= 65535)
        {
            _MaxUDPPort = _tmpNum;
        }
        LOGD ("Streaming-MAX-UDP-PORT=%d", _MaxUDPPort);

        // The max UDP port number should greater than min UDP port number +5 by CMCC requirement
        if (_MaxUDPPort <= (_MinUDPPort+5))
        {
            LOGE ("Illegal UDP port number setting, reset port number to default value !!!");
            _MinUDPPort = 0x2000;
            _MaxUDPPort = 0x4000;
        }
    }

#else
    char* value;

    // get the proxy ip
    value = mPvPlayer->getRTSPProxyName();
    if (checkRTSPParam(value)) {
        LOGD ("Streaming-ProxyIP=%s", value);
        int32 wtemplen = oscl_UTF8ToUnicode(value, strlen(value), _temp_wstr, 50);
        if (wtemplen > 0) _ProxyName.set(_temp_wstr, wtemplen);
    }

    // get the proxy port
    value = mPvPlayer->getRTSPProxyPort();
    if (checkRTSPParam(value)) {
        int32 _tmpNum = atoi(value);
        if (_tmpNum >= 0 && _tmpNum <= 65535)
        {
            _ProxyPort = _tmpNum;
            LOGD ("Streaming-ProxyPort=%d", _ProxyPort);
        }
    }

    // get the min UDP port number
    value = mPvPlayer->getRTSPUDPMinPort();
    if (checkRTSPParam(value)) {
        int32 _tmpNum = atoi(value);

        // validate the min UDP port number:
        if (_tmpNum >= 1024 && _tmpNum <= 65534)
        {
            _MinUDPPort = _tmpNum;
            LOGD ("Streaming-MIN-UDP-PORT=%d", _MinUDPPort);
        }
    }

    // get the max UDP port number
    value = mPvPlayer->getRTSPUDPMaxPort();
    if (checkRTSPParam(value)) {
        int32 _tmpNum = atoi(value);
        
        // validate the min UDP port number:
        if (_tmpNum >= 1024 && _tmpNum <= 65535)
        {
            _MaxUDPPort = _tmpNum;
            LOGD ("Streaming-MAX-UDP-PORT=%d", _MaxUDPPort);
        }
    }

    // The max UDP port number should greater than min UDP port number +5 by CMCC requirement
    if (_MaxUDPPort <= (_MinUDPPort+5))
    {
        LOGE ("Illegal UDP port number setting, reset port number to default value !!!");
        _MinUDPPort = 0x2000;
        _MaxUDPPort = 0x4000;
    }
#endif

    if (mDownloadContextData) delete mDownloadContextData;

    mDownloadContextData = new PVMFSourceContextData();
    mDownloadContextData->EnableCommonSourceContext();
    mDownloadContextData->EnableStreamingSourceContext();

    // get the streaming parameters from config file: /etc/Streaming.cfg

    // set the proxy address and port number
    mDownloadContextData->StreamingData()->iProxyName = _ProxyName;
    // <--- Demon Deng
    mDownloadContextData->StreamingData()->iProxyPort = _ProxyPort;
    // --->
    mDownloadContextData->StreamingData()->iMinUDPPort = _MinUDPPort;
    mDownloadContextData->StreamingData()->iMaxUDPPort = _MaxUDPPort;
    mDataSource->SetDataSourceContextData((OsclAny*)mDownloadContextData);

    bIsRtspStreaming = true;

    return 0;
}
// --->

int PlayerDriver::setupHttpStreamPre()
{
    mDataSource->SetDataSourceFormatType((char*)PVMF_MIME_DATA_SOURCE_HTTP_URL);

    delete mDownloadContextData;
    mDownloadContextData = NULL;

    mDownloadContextData = new PVMFSourceContextData();
    mDownloadContextData->EnableCommonSourceContext();
    mDownloadContextData->EnableDownloadHTTPSourceContext();

    // FIXME:
    // This mDownloadConfigFilename at /tmp/http-stream-cfg
    // should not exist. We need to clean it up later.
    mDownloadConfigFilename = _STRLIT_WCHAR("/tmp/http-stream-cfg");
    mDownloadFilename = NULL;
    mDownloadProxy = _STRLIT_CHAR("");
    // Ryan Lin !!
    mDownloadProxyPort = 0;
    mDownloadProxyId = _STRLIT_CHAR("");
    mDownloadProxyPassword = _STRLIT_CHAR("");

    mDownloadContextData->DownloadHTTPData()->iMaxFileSize = 0xFFFFFFFF;
    mDownloadContextData->DownloadHTTPData()->iPlaybackControl = PVMFSourceContextDataDownloadHTTP::ENoSaveToFile;
    mDownloadContextData->DownloadHTTPData()->iConfigFileName = mDownloadConfigFilename;
    mDownloadContextData->DownloadHTTPData()->iDownloadFileName = mDownloadFilename;
    mDownloadContextData->DownloadHTTPData()->iProxyName = mDownloadProxy;
    mDownloadContextData->DownloadHTTPData()->iProxyPort = 0;
    mDownloadContextData->DownloadHTTPData()->bIsNewSession = true;

    // Ryan Lin !! 
    // for proxy setting

#if 0
    mDownloadContextData->DownloadHTTPData()->iProxyUserID = mDownloadProxyId;
    mDownloadContextData->DownloadHTTPData()->iProxyUserPasswd = mDownloadProxyPassword;
#endif

    // Load cfg file
    if (!loadProxyConfig())
    {
        LOGP("load proxy cfg %s failed", PROXY_CFG_PATH);
    }
    else
    {
#if 0
        mDownloadContextData->DownloadHTTPData()->iProxyName = mDownloadProxy;
        mDownloadContextData->DownloadHTTPData()->iProxyPort = mDownloadProxyPort;
        mDownloadContextData->DownloadHTTPData()->iProxyUserID = mDownloadProxyId;
        mDownloadContextData->DownloadHTTPData()->iProxyUserPasswd = mDownloadProxyPassword;
#endif
    }    
    mDataSource->SetDataSourceContextData(mDownloadContextData);

    return 0;
}

int PlayerDriver::setupHttpStreamPost()
{
    PvmiKvp iKVPSetAsync;
    OSCL_StackString<64> iKeyStringSetAsync;
    PvmiKvp *iErrorKVP = NULL;

    int error = 0;

    iKeyStringSetAsync=_STRLIT_CHAR("x-pvmf/net/http-timeout;valtype=uint32");
    iKVPSetAsync.key=iKeyStringSetAsync.get_str();
    iKVPSetAsync.value.uint32_value=20;
    iErrorKVP=NULL;
    OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
    OSCL_FIRST_CATCH_ANY(error, return -1);

    iKeyStringSetAsync=_STRLIT_CHAR("x-pvmf/net/num-redirect-attempts;valtype=uint32");
    iKVPSetAsync.key=iKeyStringSetAsync.get_str();
    iKVPSetAsync.value.uint32_value=4;
    iErrorKVP=NULL;
    OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
    OSCL_FIRST_CATCH_ANY(error, return -1);

    iKeyStringSetAsync=_STRLIT_CHAR("x-pvmf/net/max-tcp-recv-buffer-size-download;valtype=uint32");
    iKVPSetAsync.key=iKeyStringSetAsync.get_str();
    iKVPSetAsync.value.uint32_value=64000;
    iErrorKVP=NULL;
    OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
    OSCL_FIRST_CATCH_ANY(error, return -1);

    return 0;
}

void PlayerDriver::handleSetDataSource(PlayerSetDataSource* command)
{
    LOGP("handleSetDataSource");
    int error = 0;
    const char* url = command->url();
    int lengthofurl = strlen(url);
    oscl_wchar output[lengthofurl + 1];
    OSCL_wHeapString<OsclMemAllocator> wFileName;

    if (mDataSource) {
        delete mDataSource;
        mDataSource = NULL;
    }

    // Create a URL datasource to feed PVPlayer
    mDataSource = new PVPlayerDataSourceURL();
    oscl_UTF8ToUnicode(url, strlen(url), output, lengthofurl+1);
    wFileName.set(output, oscl_strlen(output));
    LOGP("mDataSource url(%s)", url);
    mDataSource->SetDataSourceURL(wFileName);
    LOGP("handleSetDataSource- scanning for extension");
    if (strncmp(url, "rtsp:", strlen("rtsp:")) == 0) {
        mDataSource->SetDataSourceFormatType((const char*)PVMF_MIME_DATA_SOURCE_RTSP_URL);
    // <--- Morris Yang 20101004 for OMA DM
        setupRtspStreaming();
    // --->
    } else if (strncmp(url, "http:", strlen("http:")) == 0) {
        if (0!=setupHttpStreamPre())
        {
            commandFailed(command);
            return;
        }
    } else {
        const char* ext = strrchr(url, '.');
        if (ext && ( strcasecmp(ext, ".sdp") == 0) ) {
            // For SDP files, currently there is no recognizer. So, to play from such files,
            // there is a need to set the format type.
            mDataSource->SetDataSourceFormatType((const char*)PVMF_MIME_DATA_SOURCE_SDP_FILE);
        } else {
            mDataSource->SetDataSourceFormatType((const char*)PVMF_MIME_FORMAT_UNKNOWN); // Let PV figure it out
        }
    }

    OSCL_TRY(error, mPlayer->AddDataSource(*mDataSource, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleInit(PlayerInit* command)
{
    int error = 0;

    if (mDownloadContextData) {
        setupHttpStreamPost();
    }

    {
        PvmiKvp iKVPSetAsync;
        PvmiKvp *iErrorKVP = NULL;

        int error = 0;
        iKVPSetAsync.key = _STRLIT_CHAR("x-pvmf/net/user-agent;valtype=wchar*");
        OSCL_wHeapString<OsclMemAllocator> userAgent = _STRLIT_WCHAR("CORE/6.506.4.1 OpenCORE/2.02 (Linux;Android ");

#if (PROPERTY_VALUE_MAX < 8)
#error "PROPERTY_VALUE_MAX must be at least 8"
#endif
        char value[PROPERTY_VALUE_MAX];
        int len = property_get("ro.build.version.release", value, "Unknown");
        if (len) {
            LOGP("release string is %s len %d", value, len);
            oscl_wchar output[len+ 1];
            oscl_UTF8ToUnicode(value, len, output, len+1);
            userAgent += output;
        }
        userAgent += _STRLIT_WCHAR(")");
        iKVPSetAsync.value.pWChar_value=userAgent.get_str();
        iErrorKVP=NULL;
        OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
        OSCL_FIRST_CATCH_ANY(error,
                LOGE("handleInit- setParametersSync ERROR setting useragent");
        );

// <--- Morris Yang ALPS00221177 add for m.youtube.com
        char* _httpCookie = mPvPlayer->getHttpCookie();
        if ((NULL != _httpCookie) && (strlen(_httpCookie) > 0))
        {
            iKVPSetAsync.key = _STRLIT_CHAR("x-pvmf/net/http-cookie;valtype=wchar*");
            OSCL_HeapString<OsclMemAllocator> httpCookie = _STRLIT_CHAR(_httpCookie);
            iKVPSetAsync.value.pChar_value = httpCookie.get_str();
            iErrorKVP=NULL;
            OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
            OSCL_FIRST_CATCH_ANY(error,
                    LOGE("handleInit- setParametersSync ERROR setting http-cookie");
            );
        }
// --->
    }

    OSCL_TRY(error, mPlayer->Init(command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleSetVideoSurface(PlayerSetVideoSurface* command)
{

    // create video MIO if needed
    if (mVideoOutputMIO == NULL) {
        int error = 0;
        AndroidSurfaceOutput* mio = NULL;

        // attempt to load device-specific video MIO
        if (mLibHandle != NULL) {
            VideoMioFactory f = (VideoMioFactory) ::dlsym(mLibHandle, VIDEO_MIO_FACTORY_NAME);
            if (f != NULL) {
                mio = f();
            }
        }

        // if no device-specific MIO was created, use the generic one
        if (mio == NULL) {
            LOGW("Using generic video MIO");
            mio = new AndroidSurfaceOutput();
        }

        // initialize the MIO parameters
        status_t ret = mio->set(mPvPlayer, command->surface(), mEmulation);
        if (ret != NO_ERROR) {
            LOGE("Video MIO set failed");
            commandFailed(command);
            delete mio;
            return;
        }
        mVideoOutputMIO = mio;

        mVideoNode = PVMediaOutputNodeFactory::CreateMediaOutputNode(mVideoOutputMIO);
        mVideoSink = new PVPlayerDataSinkPVMFNode;

        ((PVPlayerDataSinkPVMFNode *)mVideoSink)->SetDataSinkNode(mVideoNode);
        ((PVPlayerDataSinkPVMFNode *)mVideoSink)->SetDataSinkFormatType((char*)PVMF_MIME_YUV420);

        OSCL_TRY(error, mPlayer->AddDataSink(*mVideoSink, command));
        OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
    } else {
        // change display surface
        if (mVideoOutputMIO->setVideoSurface(command->surface()) == NO_ERROR) {
            FinishSyncCommand(command);
        } else {
            LOGE("Video MIO set failed");
            commandFailed(command);
        }
    }
}

void PlayerDriver::handleSetAudioSink(PlayerSetAudioSink* command)
{
    int error = 0;
    if (command->audioSink()->realtime()) {
        LOGP("Create realtime output");
        mAudioOutputMIO = new AndroidAudioOutput();
    } else {
        LOGP("Create stream output");
        mAudioOutputMIO = new AndroidAudioStream();
    }
    mAudioOutputMIO->setAudioSink(command->audioSink());

    mAudioNode = PVMediaOutputNodeFactory::CreateMediaOutputNode(mAudioOutputMIO);
    mAudioSink = new PVPlayerDataSinkPVMFNode;

    ((PVPlayerDataSinkPVMFNode *)mAudioSink)->SetDataSinkNode(mAudioNode);
    ((PVPlayerDataSinkPVMFNode *)mAudioSink)->SetDataSinkFormatType((char*)PVMF_MIME_PCM16);

    OSCL_TRY(error, mPlayer->AddDataSink(*mAudioSink, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handlePrepare(PlayerPrepare* command)
{
    //Keep alive is sent during the play to prevent the firewall from closing ports while
    //streaming long clip
    PvmiKvp iKVPSetAsync;
    OSCL_StackString<64> iKeyStringSetAsync;
    PvmiKvp *iErrorKVP = NULL;
    int error=0;
    iKeyStringSetAsync=_STRLIT_CHAR("x-pvmf/net/keep-alive-during-play;valtype=bool");
    iKVPSetAsync.key=iKeyStringSetAsync.get_str();
    iKVPSetAsync.value.bool_value=true;
    iErrorKVP=NULL;
    OSCL_TRY(error, mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
    OSCL_TRY(error, mPlayer->Prepare(command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));

    char value[PROPERTY_VALUE_MAX] = {"0"};
    property_get("ro.com.android.disable_rtsp_nat", value, "0");
    LOGP("disable natpkt - %s",value);
    if (1 == atoi(value))
    {
        //disable firewall packet
        iKeyStringSetAsync=_STRLIT_CHAR("x-pvmf/net/disable-firewall-packets;valtype=bool");
        iKVPSetAsync.key=iKeyStringSetAsync.get_str();
        iKVPSetAsync.value.bool_value = 1; //1 - disable
        iErrorKVP=NULL;
        OSCL_TRY(error,mPlayerCapConfig->setParametersSync(NULL, &iKVPSetAsync, 1, iErrorKVP));
    }
}

void PlayerDriver::handleStart(PlayerStart* command)
{
    int error = 0;

    // reset logging
    mLastBufferingLog = 0;

    // for video, set thread priority so we don't hog CPU
    if (mVideoOutputMIO) {
    	// Honda [
        //int ret = setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_DISPLAY);
        int ret = setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_URGENT_DISPLAY);
    	// ]
    }
    // for audio, set thread priority so audio isn't choppy
    else {
        int ret = setpriority(PRIO_PROCESS, 0, ANDROID_PRIORITY_AUDIO);
    }

    // Signalling seek complete to continue obtaining the current position
    // from PVPlayer Engine
    mSeekComp = true;
    // if we are paused, just resume
    PVPlayerState state;
    if (mPlayer->GetPVPlayerStateSync(state) == PVMFSuccess
        && (state == PVP_STATE_PAUSED)) {
        if (mEndOfData) {
            // if we are at the end, seek to the beginning first
            mEndOfData = false;
            PVPPlaybackPosition begin, end;
            begin.iIndeterminate = false;
            begin.iPosUnit = PVPPBPOSUNIT_SEC;
            begin.iPosValue.sec_value = 0;
            begin.iMode = PVPPBPOS_MODE_NOW;
            end.iIndeterminate = true;
            mPlayer->SetPlaybackRange(begin, end, false, NULL);
        }
        // Ryan Lin !!!
        LOGP("!!!!! PlayerDriver::handleStart - Resume !!!!!");
        OSCL_TRY(error, mPlayer->Resume(command));
        OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
    } else {
        // Ryan Lin !!!
        LOGP("!!!!! PlayerDriver::handleStart - Start !!!!!");
        OSCL_TRY(error, mPlayer->Start(command));
        OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
    }
}

void PlayerDriver::handleSeek(PlayerSeek* command)
{
    int error = 0;

    LOGP("handleSeek");
    // Cache the most recent seek request
    mRecentSeek = command->msec();
    // Seeking in the pause state
    PVPlayerState state;
    if (mPlayer->GetPVPlayerStateSync(state) == PVMFSuccess
        && (state == PVP_STATE_PAUSED)) {
        mSeekComp = false;
    }
    PVPPlaybackPosition begin, end;
    begin.iIndeterminate = false;
    begin.iPosUnit = PVPPBPOSUNIT_MILLISEC;
    begin.iPosValue.millisec_value = command->msec();
    begin.iMode = PVPPBPOS_MODE_NOW;
    end.iIndeterminate = true;
    mSeekPending = true;

    LOGP("handleSeek begin=%d", command->msec());

    OSCL_TRY(error, mPlayer->SetPlaybackRange(begin, end, false, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));

    mEndOfData = false;
}

void PlayerDriver::handleGetPosition(PlayerGetPosition* command)
{
    PVPPlaybackPosition pos;
    pos.iPosUnit = PVPPBPOSUNIT_MILLISEC;
    PVPlayerState state;
    //  In the pause state, get the progress bar position from the recent seek value
    // instead of GetCurrentPosition() from PVPlayer Engine.
    if (mPlayer->GetPVPlayerStateSync(state) == PVMFSuccess
        && (state == PVP_STATE_PAUSED)
        && (mSeekComp == false)) {
        command->set(mRecentSeek);
    }
    else {
        if (mPlayer->GetCurrentPositionSync(pos) != PVMFSuccess) {
            command->set(-1);
        } else {
            LOGP("handleGetPosition position=%d", pos.iPosValue.millisec_value);
            command->set((int)pos.iPosValue.millisec_value);
        }
    }
}

void PlayerDriver::handleGetStatus(PlayerGetStatus* command)
{
    PVPlayerState state;
    if (mPlayer->GetPVPlayerStateSync(state) != PVMFSuccess) {
        command->set(0);
    } else {
        command->set(state);
        LOGP("status=%d", state);
    }
}

void PlayerDriver::handleCheckLiveStreaming(PlayerCheckLiveStreaming* command)
{
    LOGP("handleCheckLiveStreaming ...");
    mCheckLiveKey.clear();
    mCheckLiveKey.push_back(OSCL_HeapString<OsclMemAllocator>("pause-denied"));
    mCheckLiveValue.clear();
    int error = 0;
    OSCL_TRY(error, mPlayer->GetMetadataValues(mCheckLiveKey, 0, 1, mCheckLiveMetaValues, mCheckLiveValue, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleGetDuration(PlayerGetDuration* command)
{
    command->set(-1);
    mMetaKeyList.clear();
    mMetaKeyList.push_back(OSCL_HeapString<OsclMemAllocator>("duration"));
    mMetaValueList.clear();
    mNumMetaValues=0;
    int error = 0;
    OSCL_TRY(error, mPlayer->GetMetadataValues(mMetaKeyList,0,-1,mNumMetaValues,mMetaValueList, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleStop(PlayerStop* command)
{
    if (bIsRtspStreaming)   {
        iUserData |= PLAYER_DRIVER_STOPPED_RECEIVED;
    }

    int error = 0;
    // setting the looping boolean to false. MediaPlayer app takes care of setting the loop again before the start.
    mIsLooping = false;
    mDoLoop = false;
    PVPlayerState state;
    if ((mPlayer->GetPVPlayerStateSync(state) == PVMFSuccess)
        && ( (state == PVP_STATE_PAUSED) ||
             (state == PVP_STATE_PREPARED) ||
             (state == PVP_STATE_STARTED) ))
    {
        OSCL_TRY(error, mPlayer->Stop(command));
        OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
    }
    else
    {
        LOGP("handleStop - Player State = %d - Sending Reset instead of Stop\n",state);
        // TODO: Previously this called CancelAllCommands and RemoveDataSource
        handleReset(new PlayerReset(command->callback(), command->cookie()));
        delete command;
    }
}

void PlayerDriver::handlePause(PlayerPause* command)
{
    LOGP("call pause");
    int error = 0;
    OSCL_TRY(error, mPlayer->Pause(command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleRemoveDataSource(PlayerRemoveDataSource* command)
{
    LOGP("handleRemoveDataSource");
    int error = 0;
    OSCL_TRY(error, mPlayer->RemoveDataSource(*mDataSource, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleCancelAllCommands(PlayerCancelAllCommands* command)
{
    LOGP("handleCancelAllCommands");
    int error = 0;
    OSCL_TRY(error, mPlayer->CancelAllCommands(command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleReset(PlayerReset* command)
{
    LOGP("handleReset");
    int error = 0;

    // setting the looping boolean to false. MediaPlayer app takes care of setting the loop again before the start.
    mIsLooping = false;
    mDoLoop = false;
    mEndOfData = false;
    mContentLengthKnown = false;

    OSCL_TRY(error, mPlayer->Reset(command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}

void PlayerDriver::handleQuit(PlayerQuit* command)
{
    OsclExecScheduler *sched = OsclExecScheduler::Current();
    sched->StopScheduler();
}

// <--- Demon Deng
void PlayerDriver::handleGetMetadata(PlayerGetMetadata* command)
{
    // filter is not implemented yet
    const media::Metadata::Filter& ids = command->ids();

    mMetaKeyList.clear();
    // duration is always available, here to avoid PVMFErrArgument error
    mMetaKeyList.push_back(OSCL_HeapString<OsclMemAllocator>("duration"));
    mMetaKeyList.push_back(OSCL_HeapString<OsclMemAllocator>("codec-info/video/width"));
    mMetaKeyList.push_back(OSCL_HeapString<OsclMemAllocator>("codec-info/video/height"));
    mMetaValueList.clear();
    mNumMetaValues=0;
    int error = 0;
    OSCL_TRY(error, mPlayer->GetMetadataValues(mMetaKeyList,0,-1,mNumMetaValues,mMetaValueList, command));
    OSCL_FIRST_CATCH_ANY(error, commandFailed(command));
}
// --->

PVMFFormatType PlayerDriver::getFormatType()
{
    return mDataSource->GetDataSourceFormatType();
}

/*static*/ int PlayerDriver::startPlayerThread(void *cookie)
{
    LOGP("startPlayerThread");
    PlayerDriver *ed = (PlayerDriver *)cookie;
    return ed->playerThread();
}

#define PVPIPE "/data/mpvpipe"
#define BAD_REMOVAL_SIGNAL_CODE        0x03147
#define TERMINATE_MOUNT_MONITOR_THREAD 0x07413
int PlayerDriver::stopMountMonitorThread(void *pData)
{
    int fd = open (PVPIPE, O_RDWR);
    if (fd < 0)
    {
        LOGE ("vold PVPIPE failed in stopMountMonitorThread, errno=%d\n", errno);
        return 0;
    }
    
    int code = TERMINATE_MOUNT_MONITOR_THREAD;
    LOGE ("send exit code to MountMonitorThread\n");
    write(fd, &code, sizeof(int));
    close(fd);
    return 0;
}

int PlayerDriver::startMountMonitorThread(void *pData)
{
    LOGE ("+startMountMonitorThread\n");

     int status;
    status = mkfifo (PVPIPE, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    if (status == -1)
    {
        if (errno == EEXIST)
        {
            LOGE ("PVPIPE already exists !!\n");
        }
        else
        {
            LOGE ("create PVPIPE failed!!  errno=%d\n", errno);
            return 0;
        }
    }

    PVPlayer* player = (PVPlayer *)pData;

    int fd = open (PVPIPE, O_RDWR);

    if (fd < 0)
    {
        LOGE ("open PVPIPE failed in startMountMonitorThread, errno=%d\n", errno);
        return 0;
    }

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    // [Honda] use Keene's RR setting
    //sched_p.sched_priority = 2;
    sched_p.sched_priority = RTPM_PRIO_PV_MNTMON;

    if(0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } else {
        sched_p.sched_priority = 0;
        sched_getparam(0, &sched_p);
        LOGD("!!!!!        sched_setscheduler ok, priority: %d", sched_p.sched_priority);
    }
#endif

    while (1)
    {
        int code = 0;
        read (fd, &code, sizeof(int));
        LOGE ("PVPIPE got msg code(0x%x)", code);

        if (code == BAD_REMOVAL_SIGNAL_CODE)  // bad removal
        {
           PlayerDriver* _PlayerDriver = player->getPlayerDriver();
           if (_PlayerDriver)
           {
                PVMFFormatType _fmtType = _PlayerDriver->getFormatType();
                if (_fmtType.isNetworkSource())
                    continue;
           }
            
            LOGE ("MountMonitorThread is going to stop player...\n");
            player->sendEvent(MEDIA_ERROR_SD_CARD_BAD_REMOVAL);
            player->stop();
            player->reset();
            LOGE ("-startMountMonitorThread\n");
            close (fd);
            return 0;
        }
        else if (code == TERMINATE_MOUNT_MONITOR_THREAD)
        {
            LOGE ("-startMountMonitorThread exits\n");
            close (fd);
            return 0;
        }
        else
        {}

        usleep(100*1000); // 100ms      
    }

    close (fd);
    LOGE ("-startMountMonitorThread\n");
}

int PlayerDriver::playerThread()
{
// Honda [
#if (0)
    LOGD("!!!!!**************************************************!!!!!");
#if 1

#ifdef MT6516_MP4_HW_DECODER_V2
    // add by Ryan
    // adjust player priority
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    sched_p.sched_priority = 2;

    if(0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } else {
        sched_p.sched_priority = 0;
        sched_getparam(0, &sched_p);
        LOGD("!!!!!        sched_setscheduler ok, priority: %d", sched_p.sched_priority);
    }
#endif

#else
    setpriority(PRIO_PROCESS, gettid(), ANDROID_PRIORITY_HIGHEST);
    LOGD("!!!!!        Player Thread priority: %d\n", getpriority(PRIO_PROCESS, 0));
#endif        
    LOGD("!!!!!**************************************************!!!!!");
#endif
    int error;

    LOGP("InitializeForThread");
    LOG_PROFILE("InitializeForThread, %lld", OsclTickCount::getTimeMs());
    if (!InitializeForThread())
    {
        LOGP("InitializeForThread fail");
        mPlayer = NULL;
        mSyncSem->Signal();
        return -1;
    }

    LOGP("OMX_MasterInit");
    LOG_PROFILE("OMX_MasterInit, %lld", OsclTickCount::getTimeMs());    
    OMX_MasterInit();

    LOGP("OsclScheduler::Init");
    LOG_PROFILE("OsclScheduler_Init, %lld", OsclTickCount::getTimeMs());
    OsclScheduler::Init("AndroidPVWrapper");

    LOGP("CreatePlayer");
    LOG_PROFILE("CreatePlayer, %lld", OsclTickCount::getTimeMs());
    OSCL_TRY(error, mPlayer = PVPlayerFactory::CreatePlayer(this, this, this, &iUserData));
    if (error) {
        // Just crash the first time someone tries to use it for now?
        mPlayer = NULL;
        mSyncSem->Signal();
        return -1;
    }

    // substitute default parameters: observer(this node),queuedepth(3),nameAO for logging
    // Get the priority of the dec node, and set the threadsafe callback AO priority to 1 higher
    LOGP("Create ThreadSafeEnqueueAO");
    if (mThreadSafeEnqueueAO == NULL)
    {
        mThreadSafeEnqueueAO = OSCL_NEW(ThreadSafeEnqueueAO, (this, 10, ThreadSafeEnqueueAOName, Priority()+1));
    }
    
#if 0 // only for internal test
    if (mPlayerDriverTimerAO == NULL)
    {
        mPlayerDriverTimerAO = OSCL_NEW(PlayerDriverTimerAO, (this, PlayerDriverTimerAOName, Priority()+1));
    }
#endif

    LOGP("AddToScheduler");
    LOG_PROFILE("AddToScheduler, %lld", OsclTickCount::getTimeMs());
    AddToScheduler();
#if !USE_THREAD_SAFE_ENQUEUE      
    LOGP("PendForExec");
    PendForExec();
#endif

    LOG_PROFILE("OsclActiveScheduler_Current, %lld", OsclTickCount::getTimeMs());
    LOGP("OsclActiveScheduler::Current");
    OsclExecScheduler *sched = OsclExecScheduler::Current();
    LOG_PROFILE("StartScheduler, %lld", OsclTickCount::getTimeMs());
    LOGP("StartScheduler");
    error = OsclErrNone;
    OSCL_TRY(error, sched->StartScheduler(mSyncSem));
    OSCL_FIRST_CATCH_ANY(error,
                         // Some AO did a leave, log it
                         LOGE("Player Engine AO did a leave, error=%d", error)
                        );
// Honda [
#if (0)
    sched_p.sched_priority = 0;

    if (0 != sched_setscheduler(0, SCHED_NORMAL, &sched_p)) 
    {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } 
#endif
    LOGP("DeletePlayer");
    PVPlayerFactory::DeletePlayer(mPlayer);

    delete mDownloadContextData;
    mDownloadContextData = NULL;

    delete mDataSource;
    mDataSource = NULL;
    delete mAudioSink;
    PVMediaOutputNodeFactory::DeleteMediaOutputNode(mAudioNode);
    delete mAudioOutputMIO;
    delete mVideoSink;
    if (mVideoNode) {
        PVMediaOutputNodeFactory::DeleteMediaOutputNode(mVideoNode);
        delete mVideoOutputMIO;
    }

    mSyncStatus = OK;
    mSyncSem->Signal();
    // note that we only signal mSyncSem. Deleting it is handled
    // in enqueueCommand(). This is done because waiting for an
    // already-deleted OsclSemaphore doesn't work (it blocks),
    // and it's entirely possible for this thread to exit before
    // enqueueCommand() gets around to waiting for the semaphore.

    // do some of destructor's work here
    // goodbye cruel world
    delete this;

    //Moved after the delete this, as Oscl cleanup should be done in the end.
    //delete this was cleaning up OsclSemaphore objects, eventually causing a crash
    OsclScheduler::Cleanup();
    LOGP("OsclScheduler::Cleanup");

    LOGP("OMX_MasterDeinit");
    OMX_MasterDeinit();
    LOGP("UninitializeForThread");
    UninitializeForThread();

    PlayerDriver::stopMountMonitorThread(NULL);
    LOGP("cleanup end");
    return 0;
}

/*static*/ void PlayerDriver::syncCompletion(status_t s, void *cookie, bool cancelled)
{
    PlayerDriver *ed = static_cast<PlayerDriver*>(cookie);
    ed->mSyncStatus = s;
    ed->mSyncSem->Signal();
}

void PlayerDriver::handleCheckLiveStreamingComplete(PlayerCheckLiveStreaming* cmd)
{
    if (mCheckLiveValue.empty())
    {
    // <--- Morris Yang ALPS00136180 Let AP know when to get the meta data
        mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_CHECK_LIVE_STREAMING_COMPLETE);
    // --->
        return;
    }

    const char* substr = oscl_strstr((char*)(mCheckLiveValue[0].key), _STRLIT_CHAR("pause-denied;valtype=bool"));
    if (substr!=NULL) {
        if ( mCheckLiveValue[0].value.bool_value == true ) {
            LOGI("Live Streaming ... \n");
            mIsLiveStreaming = true;
        }
    }

// <--- Morris Yang ALPS00136180 Let AP know when to get the meta data
    mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_CHECK_LIVE_STREAMING_COMPLETE);
// --->
}

void PlayerDriver::handleGetDurationComplete(PlayerGetDuration* cmd)
{
    cmd->set(-1);

    if (mMetaValueList.empty())
        return;

    MediaClockConverter mcc;

    for (uint32 i = 0; i < mMetaValueList.size(); ++i) {
        // Search for the duration
        const char* substr=oscl_strstr(mMetaValueList[i].key, _STRLIT_CHAR("duration;valtype=uint32;timescale="));
        if (substr!=NULL) {
            uint32 timescale=1000;
            if (PV_atoi((substr+34), 'd', timescale) == false) {
                // Retrieving timescale failed so default to 1000
                timescale=1000;
            }
            uint32 duration = mMetaValueList[i].value.uint32_value;
            if (duration > 0 && timescale > 0) {
                //set the timescale
                mcc.set_timescale(timescale);
                //set the clock to the duration as per the timescale
                mcc.set_clock(duration,0);
                //convert to millisec
                cmd->set(mcc.get_converted_ts(1000));
            }
        }
    }
}

// <--- Demon Deng
void PlayerDriver::handleGetMetadataComplete(PlayerGetMetadata* cmd)
{
    using media::Metadata;
    if (mMetaValueList.empty())
        return;
    Parcel *records = cmd->records();
    Metadata metadata(records);

    for (uint32 i = 0; i < mMetaValueList.size(); ++i) {
        const char* substr;
        substr = oscl_strstr(mMetaValueList[i].key, _STRLIT_CHAR("codec-info/video/width;valtype=uint32"));
        if (substr != NULL) {
            uint32 width = mMetaValueList[i].value.uint32_value;
            metadata.appendInt32(Metadata::kVideoWidth, width);
            continue;
        }

        substr = oscl_strstr(mMetaValueList[i].key, _STRLIT_CHAR("codec-info/video/height;valtype=uint32"));
        if (substr != NULL) {
            uint32 height = mMetaValueList[i].value.uint32_value;
            metadata.appendInt32(Metadata::kVideoHeight, height);
            continue;
        }

    }
}
// --->

void PlayerDriver::CommandCompleted(const PVCmdResponse& aResponse)
{
    LOGP("CommandCompleted");
    PVMFStatus status = aResponse.GetCmdStatus();

    if (mDoLoop) {
        mDoLoop = false;
        RunIfNotReady();
        return;
    }

    PlayerCommand* command = static_cast<PlayerCommand*>(aResponse.GetContext());
    LOGP("Completed command %s status=%s", command ? command->toString(): "<null>", PVMFStatusToString(status));
    LOG_PROFILE("CommandCompleted %s, %lld", command ? command->toString(): "<null>", OsclTickCount::getTimeMs());
    if (command == NULL) return;

    if (command && command->code() && command->code() == PlayerCommand::PLAYER_SET_DATA_SOURCE)
    {
        debugger_test(2)
    }
    
    // FIXME: Ignore non-fatal seek errors because pvPlayerEngine returns these errors and retains it's state.
    if (mSeekPending) {
        mSeekPending = false;
        if ( ( (status == PVMFErrArgument) || (status == PVMFErrInvalidState) || (status == PVMFErrNotSupported) ) ) {
            LOGP("Ignoring error during seek");
            status = PVMFSuccess;
        }
    }

    if (status == PVMFSuccess) {
        switch (command->code()) {
            // Ryan Lin for test
            case PlayerCommand::PLAYER_START:
                LOGP("PLAYER_START complete");
                break;
            
            case PlayerCommand::PLAYER_PREPARE:
                LOGP("PLAYER_PREPARE complete mDownloadContextData=%p, mDataReadyReceived=%d", mDownloadContextData, mDataReadyReceived);
                LOG_PROFILE("PLAYER_PREPARE complete, %lld", OsclTickCount::getTimeMs());
                mPrepareDone = true;
                // If we are streaming from the network, we
                // have to wait until the first PVMFInfoDataReady
                // is sent to notify the user that it is okay to
                // begin playback.  If it is a local file, just
                // send it now at the completion of Prepare().
                if ((mDownloadContextData == NULL) || mDataReadyReceived) {
                    LOG_PROFILE("mPvPlayer->sendEvent(MEDIA_PREPARED), %lld", OsclTickCount::getTimeMs());
                    mPvPlayer->sendEvent(MEDIA_PREPARED);
                }
                break;

            case PlayerCommand::PLAYER_GET_DURATION:
                handleGetDurationComplete(static_cast<PlayerGetDuration*>(command));
                break;

            case PlayerCommand::PLAYER_CHECK_LIVE_STREAMING:
                handleCheckLiveStreamingComplete(static_cast<PlayerCheckLiveStreaming*>(command));
                break;

            case PlayerCommand::PLAYER_PAUSE:
                LOGP("pause complete");
                break;

            case PlayerCommand::PLAYER_SEEK:
                mPvPlayer->sendEvent(MEDIA_SEEK_COMPLETE);
                break;

                // <--- Demon Deng
            case PlayerCommand::PLAYER_GET_METADATA:
                handleGetMetadataComplete(static_cast<PlayerGetMetadata*>(command));
                break;
                // --->

            default: /* shut up gcc */
                break;
        }

        // Call the user's requested completion function
        command->complete(NO_ERROR, false);
    } else if (status == PVMFErrCancelled) {
        // Ignore cancelled code return status (PVMFErrCancelled), since it is not an error.
        LOGE("Command (%d) was cancelled", command->code());
        status = PVMFSuccess;
        command->complete(NO_ERROR, true);
    } else {
        // Try to map the PV error code to an Android one.
        LOGE("Command %s completed with an error or info %s", command->toString(), PVMFStatusToString(status));
        const media_event_type event_type = MapStatusToEventType(status);

        // <--- Demon Deng
        int ext2 = status;
        if (event_type == MEDIA_ERROR) {
            PVInterface* infoExtInterface = aResponse.GetEventExtensionInterface();
            if (infoExtInterface) {
                PVInterface* temp = NULL;
                infoExtInterface->queryInterface(PVMFErrorInfoMessageInterfaceUUID, temp);
                PVMFErrorInfoMessageInterface* infomsg = 
                    OSCL_STATIC_CAST(PVMFErrorInfoMessageInterface*, temp);

                while(infomsg) {
                    PVUuid eventuuid;
                    int32 infoCode;
                    infomsg->GetCodeUUID(infoCode, eventuuid);
                    LOGD("error code %d", infoCode);
                    for(int i = 0; i < 8; i++)
                        LOGD("uuid %x", eventuuid.data4[i]);

                    if (eventuuid == PVMFRTSPClientEngineNodeEventTypeUUID) {
                        if (infoCode > PVMFRTSPClientEngineNodeErrorEventStart && 
                                infoCode <= PVMFRTSPClientEngineNodeErrorSocketError) {
                            status = PVMFErrNetwork;
                            ext2 = infoCode;
                        }
                        break;
                    }

                    if (eventuuid == PVMFSocketNodeEventTypeUUID) {
                        status = PVMFErrNetwork;
                        ext2 = infoCode;
                        break;
                    }

                    infomsg = infomsg->GetNextMessage();
                }
            }
        }
        // --->

        if (MEDIA_NOP != event_type) {
        // <--- Demon Deng
            mPvPlayer->sendEvent(event_type, MapStatusToEventCode(status), ext2);
        // --->
        } else {
            LOGE("Ignoring: %d", status);
        }
        command->complete(UNKNOWN_ERROR, false);
    }

    delete command;
}

void PlayerDriver::HandleErrorEvent(const PVAsyncErrorEvent& aEvent)
{
    PVMFStatus status = aEvent.GetEventType();

    // Errors use negative codes (see pvmi/pvmf/include/pvmf_return_codes.h)
    if (status > PVMFErrFirst) {
        LOGE("HandleErrorEvent called with an non-error event [%d]!!", status);
    }
    LOGE("HandleErrorEvent: %s", PVMFStatusToString(status));
    // TODO: Map more of the PV error code into the Android Media Player ones.
    mPvPlayer->sendEvent(MEDIA_ERROR, ::android::MEDIA_ERROR_UNKNOWN, status);
}

void PlayerDriver::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{
    PVMFStatus status = aEvent.GetEventType();

    // Errors use negative codes (see pvmi/pvmf/include/pvmf_return_codes.h)
    if (status <= PVMFErrFirst) {
        // Errors should go to the HandleErrorEvent handler, not the
        // informational one.
        LOGE("HandleInformationalEvent called with an error event [%d]!!", status);
    }

    LOGP("HandleInformationalEvent: %s", PVMFStatusToString(status));

    switch (status) {
        case PVMFInfoEndOfData:
            mEndOfData = true;
            if (mIsLooping) {
                mDoLoop = true;
                Cancel();
                RunIfNotReady();
            } else {
                mPvPlayer->sendEvent(MEDIA_PLAYBACK_COMPLETE);
            }
            break;

        case PVMFInfoErrorHandlingComplete:
            LOGW("PVMFInfoErrorHandlingComplete");
            RunIfNotReady();
            break;

        case PVMFInfoBufferingStart:
            mPvPlayer->sendEvent(MEDIA_BUFFERING_UPDATE, 0);
            break;

        case PVMFInfoBufferingStatus:
            {
                const void *buffer = aEvent.GetLocalBuffer();
                const size_t size = aEvent.GetLocalBufferSize();

                int percentage;
                // For HTTP sessions, if PVMFInfoContentLength has been
                // received, only then the buffering status is a percentage
                // of content length. Otherwise, it is the total number of
                // bytes downloaded.
                // For RTSP session, the buffering status is a percentage
                // of the data that needs to be downloaded to start/resume
                // playback.
                if ( (mContentLengthKnown || (getFormatType() == PVMF_MIME_DATA_SOURCE_RTSP_URL) ) &&
                    (GetBufferingPercentage(buffer, size, &percentage)))
                {
                    nsecs_t now = systemTime();
                    if (now - mLastBufferingLog > kBufferingUpdatePeriod) {
                        LOGI("buffering (%d)", percentage);
                        mLastBufferingLog = now;
                    }
                    mPvPlayer->sendEvent(MEDIA_BUFFERING_UPDATE, percentage);
                }
            }
            break;

        case PVMFInfoDurationAvailable:
            {
                PVUuid infomsguuid = PVMFDurationInfoMessageInterfaceUUID;
                PVMFDurationInfoMessageInterface* eventMsg = NULL;
                PVInterface* infoExtInterface = aEvent.GetEventExtensionInterface();
                if (infoExtInterface &&
                    infoExtInterface->queryInterface(infomsguuid, (PVInterface*&)eventMsg))
                {
                    PVUuid eventuuid;
                    int32 infoCode;
                    eventMsg->GetCodeUUID(infoCode, eventuuid);
                    if (eventuuid == infomsguuid)
                    {
                        uint32 SourceDurationInMS = eventMsg->GetDuration();
                        LOGP(".... with duration = %u ms",SourceDurationInMS);
						mPvPlayer->sendEvent(MEDIA_DURATION_UPDATE,SourceDurationInMS,0);
                    }
                }
            }
            break;

        case PVMFInfoDataReady:
            if (mDataReadyReceived)
                break;
            mDataReadyReceived = true;
            // If this is a network stream, we are now ready to play.
            if (mDownloadContextData && mPrepareDone) {
                mPvPlayer->sendEvent(MEDIA_PREPARED);
            }
            break;

        case PVMFInfoVideoTrackFallingBehind:
            // FIXME:
            // When this happens, sometimes, we only have audio but no video and it
            // is not recoverable. We use the same approach as we did in previous
            // releases, and send an error event instead of an informational event
            // when this happens.
            LOGW("Video track fell behind");
            mPvPlayer->sendEvent(MEDIA_ERROR, ::android::MEDIA_ERROR_UNKNOWN,
                                 PVMFInfoVideoTrackFallingBehind);
            break;

        case PVMFInfoPoorlyInterleavedContent:
            // TODO: This event should not be passed to the user in the ERROR channel.
            LOGW("Poorly interleaved content.");
            mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_BAD_INTERLEAVING,
                                 PVMFInfoPoorlyInterleavedContent);
            break;

        case PVMFInfoContentTruncated:
            LOGE("Content is truncated.");
            // FIXME:
            // While streaming YouTube videos, we receive PVMFInfoContentTruncated event
            // after some seek operation. PV is still looking into OpenCore to see whether
            // there is any bug associated with it; Meanwhile, lets treat this as an error
            // since after playerdriver receives this event, playback session cannot be
            // recovered.
            /* // Don't report error in this case as the code does not exist in latest OpenCORE
            mPvPlayer->sendEvent(MEDIA_ERROR, ::android::MEDIA_ERROR_UNKNOWN,
                                 PVMFInfoContentTruncated);
            */
            break;

        case PVMFInfoContentLength:
            mContentLengthKnown = true;
            break;

     // <--- Morris Yang ALPS00006721 [
         case PVMFInfoNotSeekable:
         {
         	LOGE ("Content is not seekable");
         	mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_NOT_SEEKABLE,
                                 PVMFInfoNotSeekable);
         	break;
         }
     // --->

	 // <--- sam sun for aac seek table gen [
         case PVMFInfoSeekable:
         {
         	LOGE ("Content is seekable");
         	mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_SEEKABLE,
                                 PVMFInfoSeekable);
         	break;
         }
     // --->

        /* Certain events we don't really care about, but don't
         * want log spewage, so just no-op them here.
         */
        case PVMFInfoPositionStatus:
        case PVMFInfoBufferingComplete:
        case PVMFInfoContentType:
        case PVMFInfoUnderflow:
        case PVMFInfoDataDiscarded:
            break;

        default:
            LOGP("HandleInformationalEvent: type=%d UNHANDLED", status);
            mPvPlayer->sendEvent(MEDIA_INFO, ::android::MEDIA_INFO_UNKNOWN, status);
            break;
    }
}

// ----------------------------------------------------------------------------
// PlayerCommand implementation
// ----------------------------------------------------------------------------
const char* PlayerCommand::toString() const {
    return PlayerCommandCodeToString(code());
}

namespace android {

#undef LOG_TAG
#define LOG_TAG "PVPlayer"

#ifdef MAX_OPENCORE_INSTANCES
/*static*/ volatile int32_t PVPlayer::sNumInstances = 0;
#endif

// ----------------------------------------------------------------------------
// implement the Packet Video player
// ----------------------------------------------------------------------------
PVPlayer::PVPlayer()
{
    LOG_PROFILE("PVPlayer constructor, %lld", OsclTickCount::getTimeMs());
    LOGP("PVPlayer constructor");
    mDataSourcePath = NULL;
    mSharedFd = -1;
    mIsDataSourceSet = false;
    mDuration = -1;
    mPlayerDriver = NULL;
// <--- Morris Yang ALPS00221177 add for m.youtube.com
    mHttpCookie = NULL;
// <--
// <--- Demon Deng
    mRTSPProxyPort = NULL;
    mRTSPProxyName = NULL;
    mRTSPMinUDPPort = NULL;
    mRTSPMaxUDPPort = NULL;
// --->

#ifdef MAX_OPENCORE_INSTANCES
    if (android_atomic_inc(&sNumInstances) >= MAX_OPENCORE_INSTANCES) {
        LOGW("Exceeds maximum number of OpenCore instances");
        mInit = -EBUSY;
        return;
    }
#endif

    LOG_PROFILE("construct PlayerDriver, %lld", OsclTickCount::getTimeMs());
    LOGP("construct PlayerDriver");
    mPlayerDriver = new PlayerDriver(this);
    LOGP("send PLAYER_SETUP");
    LOG_PROFILE("send PLAYER_SETUP, %lld", OsclTickCount::getTimeMs());
    PlayerSetup* setup = new PlayerSetup(0,0);
    mInit = mPlayerDriver->enqueueCommand(setup);
    if (mInit == NO_INIT) {
        delete setup;
    }
}

status_t PVPlayer::initCheck()
{
    return mInit;
}

PVPlayer::~PVPlayer()
{
    LOG_PROFILE("PVPlayer destructor, %lld", OsclTickCount::getTimeMs());
    LOGP("PVPlayer destructor");
    if (mPlayerDriver != NULL) {
        PlayerQuit quit = PlayerQuit(0,0);
        mPlayerDriver->enqueueCommand(&quit); // will wait on mSyncSem, signaled by player thread
    }
    free(mDataSourcePath);
    if (mSharedFd >= 0) {
        close(mSharedFd);
    }

// <--- Morris Yang ALPS00221177 add for m.youtube.com
    if (mHttpCookie)
    {
        free(mHttpCookie);
    }
// --->
// <---
    if (mRTSPMaxUDPPort) {
        free(mRTSPMaxUDPPort);
        mRTSPMaxUDPPort = NULL;
    }
    if (mRTSPMinUDPPort) {
        free(mRTSPMinUDPPort);
        mRTSPMinUDPPort = NULL;
    }
    if (mRTSPProxyName) {
        free(mRTSPProxyName);
        mRTSPProxyName = NULL;
    }
    if (mRTSPProxyPort) {
        free(mRTSPProxyPort);
        mRTSPProxyPort = NULL;
    }
// --->

#ifdef MAX_OPENCORE_INSTANCES
    android_atomic_dec(&sNumInstances);
#endif
}
status_t    PVPlayer::setDataSource( const char *url, const KeyedVector<String8, String8> *headers)
{
    LOGP("setDataSource(%s)", url);
    LOG_PROFILE("setDataSource(1), %lld", OsclTickCount::getTimeMs());
    if (mSharedFd >= 0) {
        close(mSharedFd);
        mSharedFd = -1;
    }
    free(mDataSourcePath);
    mDataSourcePath = NULL;

// <--- Morris Yang ALPS00221177 add for m.youtube.com
    if (headers != NULL)
    {
        for (size_t i = 0; i < headers->size(); ++i) {
            if (strstr(headers->keyAt(i).string(), "Cookie"))   // we only care about Cookie
            {
                mHttpCookie = strdup(headers->valueAt(i).string());
                LOGD ("Cookie: %s", mHttpCookie);
                break;
            }
            // <--- Demon Deng for OMA DM/CP
            if (!strcmp(headers->keyAt(i).string(), "PROXY"))
            {
                mRTSPProxyName = strdup(headers->valueAt(i).string());
                LOGD ("RTSP Proxy Name: %s", mRTSPProxyName);
                continue;
            }
            if (!strcmp(headers->keyAt(i).string(), "PORT"))
            {
                mRTSPProxyPort = strdup(headers->valueAt(i).string());
                LOGD ("RTSP Proxy Port: %s", mRTSPProxyPort);
                continue;
            }
            if (!strcmp(headers->keyAt(i).string(), "MIN-UDP-PORT"))
            {
                mRTSPMinUDPPort = strdup(headers->valueAt(i).string());
                LOGD ("RTSP Min UDP Port: %s", mRTSPMinUDPPort);
                continue;
            }
            if (!strcmp(headers->keyAt(i).string(), "MAX-UDP-PORT"))
            {
                mRTSPMaxUDPPort = strdup(headers->valueAt(i).string());
                LOGD ("RTSP Max UDP Port: %s", mRTSPMaxUDPPort);
                continue;
            }
            // --->
        }
    }
// --->

    // Don't let somebody trick us in to reading some random block of memory
    if (strncmp("sharedfd://", url, 11) == 0)
        return android::UNKNOWN_ERROR;
    mDataSourcePath = strdup(url);
    return OK;
}

status_t PVPlayer::setDataSource(int fd, int64_t offset, int64_t length) {

    // This is all a big hack to allow PV to play from a file descriptor.
    // Eventually we'll fix PV to use a file descriptor directly instead
    // of using mmap().
    LOGP("setDataSource(%d, %lld, %lld)", fd, offset, length);
    LOG_PROFILE("setDataSource(3), %lld", OsclTickCount::getTimeMs());
    if (mSharedFd >= 0) {
        close(mSharedFd);
        mSharedFd = -1;
    }
    free(mDataSourcePath);
    mDataSourcePath = NULL;

    char buf[80];
    mSharedFd = dup(fd);
    sprintf(buf, "sharedfd://%d:%lld:%lld", mSharedFd, offset, length);
    mDataSourcePath = strdup(buf);
    return OK;
}

status_t PVPlayer::setVideoSurface(const sp<ISurface>& surface)
{
    LOGP("setVideoSurface(%p)", surface.get());
    LOG_PROFILE("setVideoSurface, %lld", OsclTickCount::getTimeMs());
    mSurface = surface;
    return OK;
}

status_t PVPlayer::prepare()
{
    status_t ret;

    // We need to differentiate the two valid use cases for prepare():
    // 1. new PVPlayer/reset()->setDataSource()->prepare()
    // 2. new PVPlayer/reset()->setDataSource()->prepare()/prepareAsync()
    //    ->start()->...->stop()->prepare()
    // If data source has already been set previously, no need to run
    // a sequence of commands and only the PLAYER_PREPARE code needs
    // to be run.
    if (!mIsDataSourceSet) {
        // set data source
        LOG_PROFILE("prepare, %lld", OsclTickCount::getTimeMs());
        LOGP("prepare");
        LOGP("  data source = %s", mDataSourcePath);
        ret = mPlayerDriver->enqueueCommand(new PlayerSetDataSource(mDataSourcePath,0,0));
        if (ret != OK)
            return ret;

        // init
        LOG_PROFILE("init, %lld", OsclTickCount::getTimeMs());
        LOGP("  init");
        ret = mPlayerDriver->enqueueCommand(new PlayerInit(0,0));
        if (ret != OK)
            return ret;

        // set video surface, if there is one
        if (mSurface != NULL) {
            LOG_PROFILE("set video surface, %lld", OsclTickCount::getTimeMs());
            LOGP("  set video surface");
            ret = mPlayerDriver->enqueueCommand(new PlayerSetVideoSurface(mSurface,0,0));
            if (ret != OK)
                return ret;
        }

        // set audio output
        // If we ever need to expose selectable audio output setup, this can be broken
        // out.  In the meantime, however, system audio routing APIs should suffice.
        LOG_PROFILE("set audio sink, %lld", OsclTickCount::getTimeMs());
        LOGP("  set audio sink");
        ret = mPlayerDriver->enqueueCommand(new PlayerSetAudioSink(mAudioSink,0,0));
        if (ret != OK)
            return ret;

        // New data source has been set successfully.
        mIsDataSourceSet = true;
    }

    // prepare
    LOGP("  prepare");
    LOG_PROFILE("prepare, %lld", OsclTickCount::getTimeMs());
    return mPlayerDriver->enqueueCommand(new PlayerPrepare(0,0));
}

void PVPlayer::do_nothing_debug(status_t s, void *cookie, bool cancelled)
{
    LOGD("return do nothing: %lld", OsclTickCount::getTimeMs());
}

void PVPlayer::check_for_live_streaming(status_t s, void *cookie, bool cancelled)
{
    LOGP("check_for_live_streaming s=%d, cancelled=%d", s, cancelled);
    if (s == NO_ERROR && !cancelled) {
        PVPlayer *p = (PVPlayer*)cookie;
        if ( (p->mPlayerDriver->getFormatType() == PVMF_MIME_DATA_SOURCE_RTSP_URL) ||
             (p->mPlayerDriver->getFormatType() == PVMF_MIME_DATA_SOURCE_MS_HTTP_STREAMING_URL) ) {
            p->mPlayerDriver->enqueueCommand(new PlayerCheckLiveStreaming( do_nothing, NULL));
        }
    }
}

void PVPlayer::run_init(status_t s, void *cookie, bool cancelled)
{
    LOGP("run_init s=%d, cancelled=%d", s, cancelled);
    LOG_PROFILE("run_init, %lld", OsclTickCount::getTimeMs());
    if (s == NO_ERROR && !cancelled) {
        PVPlayer *p = (PVPlayer*)cookie;
        p->mPlayerDriver->enqueueCommand(new PlayerInit(run_set_video_surface, cookie));
    }
}

void PVPlayer::run_set_video_surface(status_t s, void *cookie, bool cancelled)
{
    LOGP("run_set_video_surface s=%d, cancelled=%d", s, cancelled);
    LOG_PROFILE("run_set_video_surface, %lld", OsclTickCount::getTimeMs());
    if (s == NO_ERROR && !cancelled) {
        // If we don't have a video surface, just skip to the next step.
        PVPlayer *p = (PVPlayer*)cookie;
        if (p->mSurface == NULL) {
            run_set_audio_output(s, cookie, false);
        } else {
            p->mPlayerDriver->enqueueCommand(new PlayerSetVideoSurface(p->mSurface, run_set_audio_output, cookie));
        }
    }
}

void PVPlayer::run_set_audio_output(status_t s, void *cookie, bool cancelled)
{
    LOGP("run_set_audio_output s=%d, cancelled=%d", s, cancelled);
    LOG_PROFILE("run_set_audio_output, %lld", OsclTickCount::getTimeMs());
    if (s == NO_ERROR && !cancelled) {
        PVPlayer *p = (PVPlayer*)cookie;
        p->mPlayerDriver->enqueueCommand(new PlayerSetAudioSink(p->mAudioSink, run_prepare, cookie));
    }
}

void PVPlayer::run_prepare(status_t s, void *cookie, bool cancelled)
{
    LOGP("run_prepare s=%d, cancelled=%d", s, cancelled);
    LOG_PROFILE("run_prepare, %lld", OsclTickCount::getTimeMs());
    if (s == NO_ERROR && !cancelled) {
        PVPlayer *p = (PVPlayer*)cookie;
        p->mPlayerDriver->enqueueCommand(new PlayerPrepare(check_for_live_streaming, cookie));
    }
}

status_t PVPlayer::prepareAsync()
{
    LOGP("prepareAsync");
    LOG_PROFILE("prepareAsync, %lld", OsclTickCount::getTimeMs());
    status_t ret = OK;

    if (!mIsDataSourceSet) {  // If data source has NOT been set.
        // Set our data source as cached in setDataSource() above.
        LOGP("  data source = %s", mDataSourcePath);
        ret = mPlayerDriver->enqueueCommand(new PlayerSetDataSource(mDataSourcePath,run_init,this));
        mIsDataSourceSet = true;
    } else {  // If data source has been already set.
        // No need to run a sequence of commands.
        // The only code needed to run is PLAYER_PREPARE.
        ret = mPlayerDriver->enqueueCommand(new PlayerPrepare(check_for_live_streaming, this));
    }

    return ret;
}

status_t PVPlayer::start()
{
    LOGP("start");
    LOG_PROFILE("start, %lld", OsclTickCount::getTimeMs());
    return mPlayerDriver->enqueueCommand(new PlayerStart(0,0));
}

status_t PVPlayer::stop()
{
    LOGP("stop");
    LOG_PROFILE("stop, %lld", OsclTickCount::getTimeMs());
    return mPlayerDriver->enqueueCommand(new PlayerStop(0,0));
}

status_t PVPlayer::pause()
{
    LOGP("pause");
    LOG_PROFILE("pause, %lld", OsclTickCount::getTimeMs());
    return mPlayerDriver->enqueueCommand(new PlayerPause(0,0));
}

bool PVPlayer::isPlaying()
{
    int status = 0;
    if (mPlayerDriver->enqueueCommand(new PlayerGetStatus(&status,0,0)) == NO_ERROR) {
        return (status == PVP_STATE_STARTED);
    }
    return false;
}

status_t PVPlayer::getCurrentPosition(int *msec)
{
    status_t s = mPlayerDriver->enqueueCommand(new PlayerGetPosition(msec,0,0));

    // added by HP Cheng to prevent position to exceed duration
    if(mDuration > 0 && *msec > mDuration) {
        *msec = mDuration;
    }
    LOGP("*** getCurrentPosition - %d ***", *msec);
    return s;
}

status_t PVPlayer::getDuration(int *msec)
{
    status_t ret = mPlayerDriver->enqueueCommand(new PlayerGetDuration(msec,0,0));
    if (ret == NO_ERROR) mDuration = *msec;
    LOGD("!!!!!**************************************************!!!!!");
    LOGD("!!!!!             Media Duration - %d ms          ", *msec);
    LOGD("!!!!!**************************************************!!!!!");
    return ret;
}

status_t PVPlayer::seekTo(int msec)
{
    LOGP("seekTo(%d)", msec);
    // can't always seek to end of streams - so we fudge a little
    if ((msec == mDuration) && (mDuration > 0)) {
        msec--;
        LOGP("Seek adjusted 1 msec from end");
    }

 // <--- Morris Yang ALPS00225113 (For RTSP streaming, the server may not respond RTSP 200 OK if PLAY request range is equal to the session duration)
      PVMFFormatType _fmtType = mPlayerDriver->getFormatType();
      if (_fmtType.isNetworkSource() && (false == mPlayerDriver->isLiveStreaming()))
      {
          if ((mDuration > 0) && ((mDuration - msec) < 2))
          {
          	msec = msec-2; // adjust 2 more msec from the end
          }
      }
 // --->
    return mPlayerDriver->enqueueCommand(new PlayerSeek(msec,do_nothing,0));
}

status_t PVPlayer::reset()
{
    LOGP("reset");
// <--- Morris Yang pvplayer state protection
      int status = 0;
      if (mPlayerDriver->enqueueCommand(new PlayerGetStatus(&status,0,0)) == NO_ERROR) {
        if ((status == PVP_STATE_PREPARED) || (status == PVP_STATE_STARTED) || (status == PVP_STATE_PAUSED))
        {
        	LOGE ("reset issued in wrong state, trigger stop...");
        	mPlayerDriver->enqueueCommand(new PlayerStop(0,0));
        }
     }   
// --->
    
    LOG_PROFILE("reset, %lld", OsclTickCount::getTimeMs());
    status_t ret = mPlayerDriver->enqueueCommand(new PlayerCancelAllCommands(0,0));

    // Log failure from CancelAllCommands() and call Reset() regardless.
    if (ret != NO_ERROR) {
        LOGE("failed to cancel all exiting PV player engine commands with error code (%d)", ret);
    }
    ret = mPlayerDriver->enqueueCommand(new PlayerReset(0,0));

    // We should never fail in Reset(), but logs the failure just in case.
    if (ret != NO_ERROR) {
        LOGE("failed to reset PV player engine with error code (%d)", ret);
    } else {
        ret = mPlayerDriver->enqueueCommand(new PlayerRemoveDataSource(0,0));
    }

    mSurface.clear();
    LOGP("unmap file");
    if (mSharedFd >= 0) {
        close(mSharedFd);
        mSharedFd = -1;
    }
    mIsDataSourceSet = false;
    return ret;
}

status_t PVPlayer::setLooping(int loop)
{
    LOGP("setLooping(%d)", loop);
    return mPlayerDriver->enqueueCommand(new PlayerSetLoop(loop,0,0));
}

// This is a stub for the direct invocation API.
// From include/media/MediaPlayerInterface.h where the abstract method
// is declared:
//
//   Invoke a generic method on the player by using opaque parcels
//   for the request and reply.
//   @param request Parcel that must start with the media player
//   interface token.
//   @param[out] reply Parcel to hold the reply data. Cannot be null.
//   @return OK if the invocation was made successfully.
//
// This stub should be replaced with a concrete implementation.
//
// Typically the request parcel will contain an opcode to identify an
// operation to be executed. There might also be a handle used to
// create a session between the client and the player.
//
// The concrete implementation can then dispatch the request
// internally based on the double (opcode, handle).
status_t PVPlayer::invoke(const Parcel& request, Parcel *reply)
{
    return INVALID_OPERATION;
}


// Called by the MediaPlayerService::Client to retrieve a set or all
// the metadata if ids is empty.
status_t PVPlayer::getMetadata(const media::Metadata::Filter& ids,
                               Parcel *records) {
    using media::Metadata;

    if (!mPlayerDriver || !mPlayerDriver->prepareDone()) {
        return INVALID_OPERATION;
    }

    if (ids.size() != 0) {
        LOGW("Metadata filtering not implemented, ignoring.");
    }

    Metadata metadata(records);
    bool ok = true;

    // Right now, we only communicate info about the liveness of the
    // stream to enable/disable pause and seek in the UI.
    const bool live = mPlayerDriver->isLiveStreaming();

    ok = ok && metadata.appendBool(Metadata::kPauseAvailable, !live);
    ok = ok && metadata.appendBool(Metadata::kSeekBackwardAvailable, !live);
    ok = ok && metadata.appendBool(Metadata::kSeekForwardAvailable, !live);

    // <--- Demon Deng
    status_t ret = mPlayerDriver->enqueueCommand(new PlayerGetMetadata(ids, records, 0, 0));
    ok = ok && (ret == NO_ERROR);
    // --->

    return ok ? OK : UNKNOWN_ERROR;
}

} // namespace android

////////////////////////////////////////////////////////////////////////////////////////////////
#undef LOG_TAG
#define LOG_TAG "ThreadSafeEnqueueAO"

ThreadSafeEnqueueAO::ThreadSafeEnqueueAO(void* aObserver,
        uint32 aDepth,
        const char* aAOname,
        int32 aPriority)
        : ThreadSafeCallbackAO(aObserver, aDepth, aAOname, aPriority)
{
    LOGP("constructor");
}

ThreadSafeEnqueueAO::~ThreadSafeEnqueueAO()
{
}

OsclReturnCode ThreadSafeEnqueueAO::ProcessEvent(OsclAny* EventData)
{
    // In this case, ProcessEvent calls the method of the primary test AO to process the Event
    if (iObserver != NULL)
    {
        PlayerDriver* ptr = (PlayerDriver*) iObserver;

        if (ptr->IsAdded())
        {
            ptr->enqueueCommandDirectly((PlayerCommand*)EventData);
        }

    }
    else
    {
        LOGE("iObserver is NULL");
    }
    return OsclSuccess;
}


////////////////////////////////////////////////////////////////////////////////////////////////
#undef LOG_TAG
#define LOG_TAG "PlayerDriverTimer"

PlayerDriverTimerAO::PlayerDriverTimerAO(void* aObserver , const char* aAOname , int32 aPriority)
    : OsclTimerObject(aPriority, aAOname)
{    
    int32 err = 0;

    OSCL_TRY(err,
             ThreadLogon(); // add to scheduler
            );

    if (err != 0)
    {
        LOGE("fail");
        OSCL_LEAVE(OsclFailure);
    }

    iObserver = aObserver;

    LOGV("constructor");
    RunIfNotReady(1000);
}

PlayerDriverTimerAO::~PlayerDriverTimerAO()
{
    int32 err = 0;

    OSCL_TRY(err, ThreadLogoff(););

    if (err != 0)
    {
        OSCL_LEAVE(OsclFailure);
    }

    iObserver = NULL;
}

void PlayerDriverTimerAO::ThreadLogon()
{
    if (!IsAdded())
    {
        AddToScheduler();
    }
}

void PlayerDriverTimerAO::ThreadLogoff()
{
    //thread logoff
    if (IsAdded())
    {
        RemoveFromScheduler();
    }
}

void PlayerDriverTimerAO::Run()
{
    LOGV("PlayerDriverTimerAO::Run() in");
    if (iObserver != NULL)
    {
        PlayerDriver* ptr = (PlayerDriver*) iObserver;
        if (ptr->IsAdded())
        {
            int msec;
            ptr->enqueueCommand(new PlayerGetPosition(&msec,PlayerDriverTimerAO::syncCompletion,0));
            LOGV("current position: %d", msec);
        }

    }
    RunIfNotReady(1000);

    LOGV("PlayerDriverTimerAO::Run() out");
}

void PlayerDriverTimerAO::syncCompletion(status_t s, void *cookie, bool cancelled)
{
}
