

#ifndef ANDROID_AUDIO_OUTPUT_H
#define ANDROID_AUDIO_OUTPUT_H

#include <utils/Errors.h>

#include "android_audio_mio.h"
#include "utils/Timers.h"

using namespace android;

class AndroidAudioOutput : public AndroidAudioMIO
{
public:
    OSCL_IMPORT_REF AndroidAudioOutput();
    OSCL_IMPORT_REF ~AndroidAudioOutput();

    virtual PVMFCommandId QueryUUID(const PvmfMimeString& aMimeType, Oscl_Vector<PVUuid,
            OsclMemAllocator>& aUuids, bool aExactUuidsOnly=false,
            const OsclAny* aContext=NULL);

    virtual PVMFCommandId QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr,
            const OsclAny* aContext=NULL);

    virtual void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
            int num_elements, PvmiKvp * & aRet_kvp);

    virtual PVMFCommandId DiscardData(PVMFTimestamp aTimestamp=0, const OsclAny* aContext=NULL);

    virtual void cancelCommand(PVMFCommandId aCmdId);

    virtual PVMFCommandId Stop(const OsclAny* aContext=NULL);
    virtual PVMFCommandId Reset(const OsclAny* aContext=NULL);
    virtual void setAudioSink(const sp<MediaPlayerInterface::AudioSink>& audioSink);

private:

    // Audio output request handling
    // this mio queues the requests, the audio output thread dequeues and processes them
    class OSSRequest
    {
        public:
            OSSRequest(uint8* data, uint32 len, PVMFCommandId id, const OsclAny* ctx, const PVMFTimestamp& ts) :
                iData(data), iDataLen(len), iCmdId(id), iContext(ctx), iTimestamp(ts)
        {}
            uint8* iData;
            uint32 iDataLen;
            PVMFCommandId iCmdId;
            const OsclAny* iContext;
            PVMFTimestamp iTimestamp;
    };

    virtual void writeAudioBuffer(uint8* aData, uint32 aDataLen, PVMFCommandId cmdId,
            OsclAny* aContext, PVMFTimestamp aTimestamp);

    virtual void Run();

    virtual void returnAllBuffers();

    void RequestAndWaitForThreadExit();

    static int start_audout_thread_func(TOsclThreadFuncArg arg);
    int audout_thread_func();

    // semaphores used to communicate with the audio output thread
    OsclSemaphore* iAudioThreadSem;
    OsclSemaphore* iAudioThreadTermSem;
    OsclSemaphore* iAudioThreadReturnSem;
    OsclSemaphore* iAudioThreadCreatedSem;

    volatile bool iExitAudioThread;
    volatile bool iReturnBuffers;

    // active timing
    AndroidAudioMIOActiveTimingSupport* iActiveTiming;

    // oss request queue, needs to use lock mechanism to access
    Oscl_Vector<OSSRequest,OsclMemAllocator> iOSSRequestQueue;

    // lock used to access the oss request queue
    OsclMutex iOSSRequestQueueLock;

    // number of bytes in an input frame
    int iInputFrameSizeInBytes;

    // Wallclock time in nano secs to find the interval between write calls done to device.
    nsecs_t     iClockTimeOfWriting_ns;

    // add by Ryan for av sync
public:
    virtual void ResetMIO();
    virtual void ClearBuffer();
    virtual void SetAVSyncCtrl(PVMFAVSyncbase* aAVSyncCtrl);

private:
    // Ryan Lin !!!! for av sync
    bool m_bWaitDisplayBufLock;
    PVMFAVSyncbase* mAVSyncCtrl;
};

#endif // ANDROID_AUDIO_OUTPUT_H
