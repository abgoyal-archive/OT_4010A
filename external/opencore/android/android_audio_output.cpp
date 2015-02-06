

//#define LOG_NDEBUG 0
#define LOG_TAG "AudioOutput"
#include <utils/Log.h>

#include "android_audio_output.h"

#include <sys/prctl.h>
#include <sys/resource.h>
#include <utils/threads.h>
#include <media/AudioTrack.h>
#include <linux/rtpm_prio.h>

#include "oscl_file_cfg.h"

using namespace android;

// Ryan Lin !!
#undef LOG_TAG
#define LOG_TAG "AudioOutput"

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

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE) JJLOGE MESSAGE
#define JJLOGE(id, ...) LOGP(__VA_ARGS__)

// TODO: dynamic buffer count based on sample rate and # channels
static const int kNumOutputBuffers = 3;

// maximum allowed clock drift before correction
static const int32 kMaxClockDriftInMsecs = 25;    // should be tight enough for reasonable sync
// modify ballowed clock adjust
static const int32 kMaxClockCorrection = 100;     // maximum clock correction per update

OSCL_EXPORT_REF AndroidAudioOutput::AndroidAudioOutput() :
    AndroidAudioMIO("AndroidAudioOutput"),
    iExitAudioThread(false),
    iReturnBuffers(false),
    iActiveTiming(NULL)
    , mAVSyncCtrl(NULL)
{
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_audiooutput");

    LOGP("constructor");
    iClockTimeOfWriting_ns = 0;
    iInputFrameSizeInBytes = 0;

    // semaphore used to communicate between this  mio and the audio output thread
    iAudioThreadSem = new OsclSemaphore();
    //modify by chipeng.
    iAudioThreadSem->Create(0,3);
    iAudioThreadTermSem = new OsclSemaphore();
    iAudioThreadTermSem->Create(0);
    iAudioThreadReturnSem = new OsclSemaphore();
    iAudioThreadReturnSem->Create(0);
    iAudioThreadCreatedSem = new OsclSemaphore();
    iAudioThreadCreatedSem->Create(0);

    // locks to access the queues by this mio and by the audio output thread
    iOSSRequestQueueLock.Create();
    iOSSRequestQueue.reserve(iWriteResponseQueue.capacity());

    // create active timing object
    OsclMemAllocator alloc;
    OsclAny*ptr=alloc.allocate(sizeof(AndroidAudioMIOActiveTimingSupport));
    if (ptr) {
        iActiveTiming=new(ptr)AndroidAudioMIOActiveTimingSupport(kMaxClockDriftInMsecs, kMaxClockCorrection);
        iActiveTiming->setThreadSemaphore(iAudioThreadSem);
    }
}

OSCL_EXPORT_REF AndroidAudioOutput::~AndroidAudioOutput()
{
    LOGP("destructor");

    mAVSyncCtrl = NULL;
    // make sure output thread has exited
    RequestAndWaitForThreadExit();

    // cleanup active timing object
    if (iActiveTiming) {
        iActiveTiming->~AndroidAudioMIOActiveTimingSupport();
        OsclMemAllocator alloc;
        alloc.deallocate(iActiveTiming);
    }

    // clean up some thread interface objects
    iAudioThreadSem->Close();
    delete iAudioThreadSem;
    iAudioThreadTermSem->Close();
    delete iAudioThreadTermSem;
    iAudioThreadReturnSem->Close();
    delete iAudioThreadReturnSem;
    iAudioThreadCreatedSem->Close();
    delete iAudioThreadCreatedSem;

    iOSSRequestQueueLock.Close();
}

PVMFCommandId AndroidAudioOutput::QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr, const OsclAny* aContext)
{
    LOGP("QueryInterface in");
    // check for active timing extension
    if (iActiveTiming && (aUuid == PvmiClockExtensionInterfaceUuid)) {
        PvmiClockExtensionInterface* myInterface = OSCL_STATIC_CAST(PvmiClockExtensionInterface*,iActiveTiming);
        aInterfacePtr = OSCL_STATIC_CAST(PVInterface*, myInterface);
        return QueueCmdResponse(PVMFSuccess, aContext);
    }

    // pass to base class
    else return AndroidAudioMIO::QueryInterface(aUuid, aInterfacePtr, aContext);
}

PVMFCommandId AndroidAudioOutput::QueryUUID(const PvmfMimeString& aMimeType,
                                        Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                        bool aExactUuidsOnly, const OsclAny* aContext)
{
    LOGP("QueryUUID in");
    int32 err;
    OSCL_TRY(err,
            aUuids.push_back(PVMI_CAPABILITY_AND_CONFIG_PVUUID);
            if (iActiveTiming) {
            PVUuid uuid;
            iActiveTiming->queryUuid(uuid);
            aUuids.push_back(uuid);
            }
            );
    return QueueCmdResponse(err == OsclErrNone ? PVMFSuccess : PVMFFailure, aContext);
}

PVMFCommandId AndroidAudioOutput::Stop(const OsclAny* aContext)
{
    LOGP("AndroidAudioOutput Stop (%p)", aContext);
    // return all buffer by writecomplete
    returnAllBuffers();
    return AndroidAudioMIO::Stop(aContext);
}

PVMFCommandId AndroidAudioOutput::Reset(const OsclAny* aContext)
{
    LOGP("AndroidAudioOutput Reset (%p)", aContext);
    // return all buffer by writecomplete
    returnAllBuffers();
    // request output thread to exit
    RequestAndWaitForThreadExit();
    return AndroidAudioMIO::Reset(aContext);
}

void AndroidAudioOutput::ResetMIO()
{
    LOGP("***** ResetMIO *****");
    /*
    if (mAVSyncCtrl != NULL)
    {
        if (mAVSyncCtrl->isMutexLock()== true)
        {
            mAVSyncCtrl->UnlockSyncMutex();
        }
    }*/
    m_bWaitDisplayBufLock = true;
}

void AndroidAudioOutput::ClearBuffer()
{
    LOGP("***** ClearBuffer *****");

    returnAllBuffers();
}

void AndroidAudioOutput::SetAVSyncCtrl(PVMFAVSyncbase* aAVSyncCtrl)
{
    LOGP("SetAVSyncCtrl");
    mAVSyncCtrl = aAVSyncCtrl;
}

void AndroidAudioOutput::cancelCommand(PVMFCommandId command_id)
{
    LOGP("cancelCommand (%u) RequestQ size %d", command_id,iOSSRequestQueue.size());
    iOSSRequestQueueLock.Lock();
    for (uint32 i = 0; i < iOSSRequestQueue.size(); i++) {
        if (iOSSRequestQueue[i].iCmdId == command_id) {
            iDataQueued -= iOSSRequestQueue[i].iDataLen;
            if (iPeer)
                iPeer->writeComplete(PVMFSuccess, iOSSRequestQueue[i].iCmdId, (OsclAny*)iOSSRequestQueue[i].iContext);
            iOSSRequestQueue.erase(&iOSSRequestQueue[i]);
            break;
        }
    }
    iOSSRequestQueueLock.Unlock();
    LOGP("cancelCommand data queued = %u", iDataQueued);

    ProcessWriteResponseQueue();
}

void AndroidAudioOutput::returnAllBuffers()
{
    LOGP("returnAllBuffers RequestQ size %d",iOSSRequestQueue.size());
    iOSSRequestQueueLock.Lock();
    int iOSSRequestQueueSize = iOSSRequestQueue.size();
    while (iOSSRequestQueue.size()) {
        iDataQueued -= iOSSRequestQueue[0].iDataLen;
        if (iPeer)
            iPeer->writeComplete(PVMFSuccess, iOSSRequestQueue[0].iCmdId, (OsclAny*)iOSSRequestQueue[0].iContext);
        iOSSRequestQueue.erase(&iOSSRequestQueue[0]);
    }
    iOSSRequestQueueLock.Unlock();
    LOGP("returnAllBuffers data queued = %u", iDataQueued);
    // modify by chipeng,if iOSSRequestQueue has no data , no need to set.
    if (iAudioThreadSem && iAudioThreadCreatedAndMIOConfigured&& iOSSRequestQueueSize) {
        LOGP("signal thread to return buffers");
        iReturnBuffers = true;
        iAudioThreadSem->Signal();
        while (iAudioThreadReturnSem->Wait() != OsclProcStatus::SUCCESS_ERROR)
            ;
        LOGP("return buffers signal completed");
    }
}


PVMFCommandId AndroidAudioOutput::DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext)
{
    LOGP("DiscardData timestamp(%u) RequestQ size %d", aTimestamp,iOSSRequestQueue.size());

    if(iActiveTiming){
        LOGP("Force clock update");
        iActiveTiming->ForceClockUpdate();
    }

    bool sched = false;
    PVMFCommandId audcmdid;
    const OsclAny* context;
    PVMFTimestamp timestamp;

    // the OSSRequest queue should be drained
    // all the buffers in them should be returned to the engine
    // writeComplete cannot be called from here
    // thus the best way is to queue the buffers onto the write response queue
    // and then call RunIfNotReady
    iOSSRequestQueueLock.Lock();
    for (int32 i = (iOSSRequestQueue.size() - 1); i >= 0; i--) {
        if (iOSSRequestQueue[i].iTimestamp < aTimestamp) {
            audcmdid = iOSSRequestQueue[i].iCmdId;
            context = iOSSRequestQueue[i].iContext;
            timestamp = iOSSRequestQueue[i].iTimestamp;
            iDataQueued -= iOSSRequestQueue[i].iDataLen;
            LOGP("discard buffer (%d) context(%p) timestamp(%u) Datalen(%d)", audcmdid,context, timestamp,iOSSRequestQueue[i].iDataLen);
            iOSSRequestQueue.erase(&iOSSRequestQueue[i]);
            sched = true;

            WriteResponse resp(PVMFSuccess, audcmdid, context, timestamp);
            iWriteResponseQueueLock.Lock();
            iWriteResponseQueue.push_back(resp);
            iWriteResponseQueueLock.Unlock();
        }
    }
    LOGP("DiscardData data queued = %u, setting flush pending", iDataQueued);
    iFlushPending=true;

    iOSSRequestQueueLock.Unlock();

    if (sched)
        RunIfNotReady();

    return AndroidAudioMIO::DiscardData(aTimestamp, aContext);
}

void AndroidAudioOutput::RequestAndWaitForThreadExit()
{
    LOGP("RequestAndWaitForThreadExit In");
    if (iAudioThreadSem && iAudioThreadCreatedAndMIOConfigured) {
        LOGP("signal thread for exit");
        iExitAudioThread = true;
        iAudioThreadSem->Signal();
        while (iAudioThreadTermSem->Wait() != OsclProcStatus::SUCCESS_ERROR)
            ;
        LOGP("thread term signal received");
        iAudioThreadCreatedAndMIOConfigured = false;
    }
}

void AndroidAudioOutput::setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                        int num_elements, PvmiKvp * & aRet_kvp)
{
    LOGP("AndroidAudioOutput setParametersSync In");
    AndroidAudioMIO::setParametersSync(aSession, aParameters, num_elements, aRet_kvp);

    // initialize thread when we have enough information
    if (iAudioSamplingRateValid && iAudioNumChannelsValid && iAudioFormat != PVMF_MIME_FORMAT_UNKNOWN) {
        LOGP("start audio thread");
        OsclThread AudioOutput_Thread;
        iExitAudioThread = false;
        iReturnBuffers = false;
        OsclProcStatus::eOsclProcError ret = AudioOutput_Thread.Create((TOsclThreadFuncPtr)start_audout_thread_func,
                                                    0, (TOsclThreadFuncArg)this, Start_on_creation);

        //Don't signal the MIO node that the configuration is complete until the driver latency has been set
        while (iAudioThreadCreatedSem->Wait() != OsclProcStatus::SUCCESS_ERROR)
           ;

        // <--- Demon Deng, ALPS00029598, flag will be set in audiothread
        if(OsclProcStatus::SUCCESS_ERROR == ret && 
                iAudioThreadCreatedAndMIOConfigured){
        // --->
            iAudioThreadCreatedAndMIOConfigured = true;
            if(iObserver){
                LOGP("event PVMFMIOConfigurationComplete to peer");
                iObserver->ReportInfoEvent(PVMFMIOConfigurationComplete);
                // Honda add
				iObserver->ReportInfoEvent(PVMFInfoSinkThreadCreate);
            }
        }
        else{
            iAudioThreadCreatedAndMIOConfigured = false;
            if(iObserver){
                LOGE("event PVMFErrResourceConfiguration to peer");
                iObserver->ReportErrorEvent(PVMFErrResourceConfiguration);
            }
        }
    }
    LOGP("AndroidAudioOutput setParametersSync out");
}

void AndroidAudioOutput::Run()
{
    // if running, update clock
    if ((iState == STATE_MIO_STARTED) && iInputFrameSizeInBytes) {
        uint32 msecsQueued = iDataQueued / iInputFrameSizeInBytes * iActiveTiming->msecsPerFrame();
        LOGP("%u msecs of data queued, %u bytes of data queued", msecsQueued,iDataQueued);
        iActiveTiming->UpdateClock();
    }
    AndroidAudioMIO::Run();
}

void AndroidAudioOutput::writeAudioBuffer(uint8* aData, uint32 aDataLen, PVMFCommandId cmdId, OsclAny* aContext, PVMFTimestamp aTimestamp)
{
    // queue up buffer and signal audio thread to process it
    //LOGD("writeAudioBuffer :: DataLen(%d), cmdId(%d), Context(%p), Timestamp (%d)",aDataLen, cmdId, aContext, aTimestamp);
    OSSRequest req(aData, aDataLen, cmdId, aContext, aTimestamp);
    iOSSRequestQueueLock.Lock();
    iOSSRequestQueue.push_back(req);
    iDataQueued += aDataLen;

    // wake up the audio output thread to process this buffer only if clock has started running
    if (iActiveTiming->clockState() == PVMFMediaClock::RUNNING) {
        LOGP("clock is ticking signal thread for data");
        iAudioThreadSem->Signal();
    }
    iOSSRequestQueueLock.Unlock();
}

void AndroidAudioOutput::setAudioSink(const sp<MediaPlayerInterface::AudioSink>& audioSink)
{
    LOGP("AndroidAudioOutput::setAudioSink");
    AndroidAudioMIO::setAudioSink(audioSink);
    iActiveTiming->setAudioSink (audioSink);
}


//------------------------------------------------------------------------
// audio thread
//

#undef LOG_TAG
#define LOG_TAG "audiothread"

// this is the audio output thread
// used to send data to the linux audio output device
// communicates with the audio MIO via a semaphore, a request queue and a response queue
/*static*/ int AndroidAudioOutput::start_audout_thread_func(TOsclThreadFuncArg arg)
{
    LOGP("start_audout_thread_func in");
    AndroidAudioOutput *obj = (AndroidAudioOutput *)arg;
    prctl(PR_SET_NAME, (unsigned long) "audio out", 0, 0, 0);
    int err = obj->audout_thread_func();
    LOGP("start_audout_thread_func out return code %d",err);
    return err;
}

int AndroidAudioOutput::audout_thread_func()
{
    enum { IDLE, STOPPED, STARTED, PAUSED } state = IDLE;
    int64_t lastClock = 0;

    LOGP("audout_thread_func");

#if 1
    // add by Chipeng
    // adjust Audio out  priority
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    sched_p.sched_priority = RTPM_PRIO_AUDIO_OUT;

    if(0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } else {
        sched_p.sched_priority = RTPM_PRIO_AUDIO_OUT;
        sched_getparam(0, &sched_p);
        LOGP("sched_setscheduler ok, priority: %d", sched_p.sched_priority);
    }
#endif

#if defined(HAVE_SCHED_SETSCHEDULER) && defined(HAVE_GETTID)
    setpriority(PRIO_PROCESS, gettid(), ANDROID_PRIORITY_AUDIO);
#endif

    if (iAudioNumChannelsValid == false || iAudioSamplingRateValid == false || iAudioFormat == PVMF_MIME_FORMAT_UNKNOWN) {
        LOGE("channel count or sample rate is invalid");
        // <--- Demon Deng, ALPS00029598, signal setParametersSync
        iAudioThreadCreatedAndMIOConfigured = false;
        iAudioThreadCreatedSem->Signal();
        // --->
        return -1;
    }

    LOGP("Creating AudioTrack object: rate=%d, channels=%d, buffers=%d", iAudioSamplingRate, iAudioNumChannels, kNumOutputBuffers);
    status_t ret = mAudioSink->open(iAudioSamplingRate, iAudioNumChannels, ((iAudioFormat==PVMF_MIME_PCM8)?AudioSystem::PCM_8_BIT:AudioSystem::PCM_16_BIT), kNumOutputBuffers);
    iAudioSamplingRateValid = false; // purpose of these flags is over here, reset these for next validation recording.
    iAudioNumChannelsValid  = false;
    iAudioFormat = PVMF_MIME_FORMAT_UNKNOWN;
    if (ret != 0) {
        iAudioThreadCreatedAndMIOConfigured = false;
        LOGE("Error creating AudioTrack");
        // <--- Demon Deng, ALPS00029598, signal setParametersSync
        iAudioThreadCreatedSem->Signal();
        // --->
        return -1;
    }

    // calculate timing data
    int outputFrameSizeInBytes = mAudioSink->frameSize();
    float msecsPerFrame = mAudioSink->msecsPerFrame();
    uint32 latency = mAudioSink->latency();
    LOGP("driver latency(%u),outputFrameSizeInBytes(%d),msecsPerFrame(%f),frame count(%d)", latency,outputFrameSizeInBytes,msecsPerFrame,mAudioSink->frameCount());

    // initialize active timing
    iActiveTiming->setFrameRate(msecsPerFrame);
    iActiveTiming->setDriverLatency(latency);

    // <--- Demon Deng, ALPS00029598, set iAudioThreadCreatedAndMIOConfigured
    iAudioThreadCreatedAndMIOConfigured = true;
    // --->
    iAudioThreadCreatedSem->Signal();
    // this must be set after iActiveTiming->setFrameRate to prevent race
    // condition in Run()
    iInputFrameSizeInBytes = outputFrameSizeInBytes;

    // buffer management
    uint32 bytesAvailInBuffer = 0;
    uint32 bytesToWrite;
    uint32 bytesWritten;
    uint8* data = 0;
    uint32 len = 0;
    PVMFCommandId cmdid = 0;
    const OsclAny* context = 0;
    PVMFTimestamp timestamp = 0;

    // Ryan Lin !!!
    m_bWaitDisplayBufLock = true;

    // wait for signal from MIO thread
    LOGP("wait for signal");
    iAudioThreadSem->Wait();
    LOGP("ready to work");

    while (1)
    {
        // if paused, stop the output track
        switch ( iActiveTiming->clockState()) {
        case PVMFMediaClock::RUNNING:
            // start output
            if (state != STARTED) {
                if (iFlushPending) {
                    LOGP("flush");
                    mAudioSink->flush();
                    iFlushPending = false;
                    bytesAvailInBuffer = 0;
                    iClockTimeOfWriting_ns = 0;
                    // discard partial buffer and send response to MIO
                    if (data && len) {
                        LOGP("discard partial buffer and send response to MIO");
                        sendResponse(cmdid, context, timestamp);
                        data = 0;
                        len = 0;
                    }
                }
                if (iDataQueued || len ) {
                    LOGD("iDataQueued || len start mAudioSink->start();");
                    mAudioSink->start();
                    state = STARTED;
                } else {
                    LOGP("clock running and no data queued - don't start track");
                }
            }
            else{
                 if (iFlushPending) {
                    LOGD("iFlushPending flush 2");
                    mAudioSink->pause();
                    iFlushPending = false;
                    bytesAvailInBuffer = 0;
                    iClockTimeOfWriting_ns = 0;
                    // discard partial buffer and send response to MIO
                    if (data && len) {
                        LOGD("discard partial buffer and send response to MIO");
                        sendResponse(cmdid, context, timestamp);
                        data = 0;
                        len = 0;
                    }
                    mAudioSink->start();
                }
            }
            break;
        case PVMFMediaClock::STOPPED:
             LOGP("clock has been stopped...");
        case PVMFMediaClock::PAUSED:
            if (state == STARTED) {
                LOGP("pause");
                mAudioSink->pause();
            }
            state = PAUSED;
            if(!iExitAudioThread && !iReturnBuffers) {
                LOGP("wait");
                iAudioThreadSem->Wait();
                LOGP("awake");
            }
            break;
        default:
            break;
        }

        // if out of data, check the request queue
        if (len == 0)
        {
            // add for av sync by Ryan Lin
            if (m_bWaitDisplayBufLock)
            {
                LOGP("Wait lock: %s", m_bWaitDisplayBufLock?"true":"false");
                m_bWaitDisplayBufLock = false;

                if (mAVSyncCtrl != NULL)
                {
                    mAVSyncCtrl->LockSyncMutex();
                    iActiveTiming->UpdateStartTime();
                    //uint32 _DelayTime = mAVSyncCtrl->GetAudioDelayTime();
                    //LOGP("audio output delay time: %d", _DelayTime);
                }
            }

            //LOGP("no playable data, Request Q size %d",iOSSRequestQueue.size());
            iOSSRequestQueueLock.Lock();
            bool empty = iOSSRequestQueue.empty();
            if (!empty) {
                data = iOSSRequestQueue[0].iData;
                len = iOSSRequestQueue[0].iDataLen;
                cmdid = iOSSRequestQueue[0].iCmdId;
                context = iOSSRequestQueue[0].iContext;
                timestamp = iOSSRequestQueue[0].iTimestamp;
                iDataQueued -= len;
                iOSSRequestQueue.erase(&iOSSRequestQueue[0]);
                LOGP("receive buffer (%d), timestamp = %u data queued = %u", cmdid, timestamp,iDataQueued);
            }
            iOSSRequestQueueLock.Unlock();

            // if queue is empty, wait for more work
            // FIXME: Why do end up here so many times when stopping?
            if (empty && !iExitAudioThread && !iReturnBuffers) {
                LOGP("queue is empty, wait for more work");
                iAudioThreadSem->Wait();
            }

            // empty buffer means "End-Of-Stream" - send response to MIO
            else if (len == 0 && (!empty)) {
                LOGP("EOS");
                state = STOPPED;
                mAudioSink->stop();
                if(!iExitAudioThread){
                    nsecs_t interval_nanosec = 0; // Interval between last writetime and EOS processing time in nanosec
                    nsecs_t now = systemTime(SYSTEM_TIME_MONOTONIC);
                    LOGP("now = %lld ,iClockTimeOfWriting_ns = %lld",now, iClockTimeOfWriting_ns);
                    if((now >= iClockTimeOfWriting_ns) && (iClockTimeOfWriting_ns !=0)){
                        interval_nanosec = now - iClockTimeOfWriting_ns;
                    }
                    else{ //when timevalue wraps
                         interval_nanosec = 0;
                    }
                    LOGP(" I am early,going for sleep for latency = %u millsec, interval_nanosec = %lld",latency, interval_nanosec);
                    struct timespec requested_time_delay, remaining;
                    requested_time_delay.tv_sec = latency/1000;
                    nsecs_t latency_nanosec = (latency%1000)*1000*1000;
                    if(interval_nanosec < latency_nanosec){
                        requested_time_delay.tv_nsec = latency_nanosec - interval_nanosec;
                        nanosleep (&requested_time_delay, &remaining);
                        LOGP(" Wow, what a great siesta....send response to engine");
                    }
                    else{// interval is greater than latency so no need of sleep
                        LOGP(" No time to sleep :( send response to engine anyways");
                    }
                    iClockTimeOfWriting_ns = 0;
                    sendResponse(cmdid, context, timestamp);
                }
            }
        }

        if (iReturnBuffers) {
            LOGP("Return buffers from the audio thread");
            if (len) sendResponse(cmdid, context, timestamp);
            iReturnBuffers=false;
            data = 0;
            len = 0;
            iAudioThreadReturnSem->Signal();
        }

        // check for exit signal
        if (iExitAudioThread) {
            LOGP("exit received");
            if (len) sendResponse(cmdid, context, timestamp);
            break;
        }

        // data to output?
        if (len && (state == STARTED) && !iExitAudioThread)
        {
            // always align to AudioFlinger buffer boundary
            if (bytesAvailInBuffer == 0)
            {
                // this buffer should align to format and channel
                //bytesAvailInBuffer = mAudioSink->bufferSize();
                // modify by chipeng , for write should write samll part of data.
                bytesAvailInBuffer= mAudioSink->frameSize() * (mAudioSink->frameCount()>>2);
                /*
                LOGD("bytesAvailInBuffer = %u frameSize = %d frameCount = %d ",
                    bytesAvailInBuffer,mAudioSink->frameSize(), mAudioSink->frameCount());
                    */

            }
                bytesToWrite = bytesAvailInBuffer > len ? len : bytesAvailInBuffer;
                bytesWritten = mAudioSink->write(data, bytesToWrite);
                /*
                LOGD("16 bit :: cmdid = %d, len = %u, bytesAvailInBuffer = %u, bytesToWrite = %u bytesWritten = %d timestamp = %d",
                    cmdid, len, bytesAvailInBuffer, bytesToWrite,timestamp);
                    */

                if (bytesWritten != bytesToWrite ) {
                    LOGD("audioout thread writing audio data bytesWritten = %d bytesToWrite = %d",bytesWritten,bytesToWrite);
                    iAudioThreadSem->Wait();
                }

                if(iFlushPending){
                    LOGD("thread iFlushPending == true");
                    continue;
                }

                data += bytesWritten;
                len -= bytesWritten;
                iClockTimeOfWriting_ns = systemTime(SYSTEM_TIME_MONOTONIC);

            // count bytes sent
            bytesAvailInBuffer -= bytesWritten;

            // update frame count for latency calculation
            iActiveTiming->incFrameCount(bytesWritten / outputFrameSizeInBytes);
            //LOGP("outputFrameSizeInBytes = %u,bytesWritten = %u,bytesAvailInBuffer = %u", outputFrameSizeInBytes,bytesWritten,bytesAvailInBuffer);
            // if done with buffer - send response to MIO
            if (data && !len) {
                LOGP("done with the data cmdid %d, context %p, timestamp %d ",cmdid, context, timestamp);
                sendResponse(cmdid, context, timestamp);
                data = 0;
            }
        }
    } // while loop

    LOGP("stop and delete track   mAudioSink->stop();");
    mAudioSink->stop();
    iClockTimeOfWriting_ns = 0;

    LOGP("audout_thread_func exit");
    iAudioThreadTermSem->Signal();

    return 0;
}
