

#ifndef ANDROID_SURFACE_OUTPUT_H_INCLUDED
#define ANDROID_SURFACE_OUTPUT_H_INCLUDED

#include "pvmi_mio_control.h"
#include "pvmi_media_transfer.h"
#include "oscl_scheduler_ao.h"
#include "pvmi_media_io_observer.h"
#include "oscl_file_io.h"
#include "pvmi_config_and_capability.h"
#include "oscl_string_containers.h"
#include "pvmi_media_io_clock_extension.h"

#ifdef PERFORMANCE_MEASUREMENTS_ENABLED
#include "pvprofile.h"
#endif

// FIXME: Move to OMAP library
// Linux and Kernel Includes for Frame Buffer
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
//#include <linux/fb.h>
//#include <linux/videodev.h>

// SurfaceFlinger
#include <surfaceflinger/ISurface.h>

// interprocess shared memory support
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>

#ifdef MT6516_OMX_USE_PMEM
#include <binder/MemoryHeapBaseMTK.h>
#include "oscl_pmem.h"
#endif

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#ifndef OSCL_QUEUE_H_INCLUDED
#include "oscl_queue.h"
#endif

// color converter
#include "cczoomrotation16.h"

#include "threadsafe_callback_ao.h"

#ifdef MT6516_OMX_USE_PMEM
#define USE_PMEM 1
#endif

// define bits, mask and validity check for video parameters
#define VIDEO_PARAMETERS_INVALID 0
#define VIDEO_SUBFORMAT_VALID (1 << 0)
#define DISPLAY_HEIGHT_VALID (1 << 1)
#define DISPLAY_WIDTH_VALID (1 << 2)
#define VIDEO_HEIGHT_VALID (1 << 3)
#define VIDEO_WIDTH_VALID (1 << 4)
#define VIDEO_PARAMETERS_MASK (VIDEO_SUBFORMAT_VALID | DISPLAY_HEIGHT_VALID | \
        DISPLAY_WIDTH_VALID | VIDEO_HEIGHT_VALID | VIDEO_WIDTH_VALID)
#define VIDEO_PARAMETERS_VALID (VIDEO_SUBFORMAT_VALID | DISPLAY_HEIGHT_VALID | \
        DISPLAY_WIDTH_VALID | VIDEO_HEIGHT_VALID | VIDEO_WIDTH_VALID)

namespace android {
    class PVPlayer;
}

class PVLogger;
class PVMFMediaClock;
class AndroidSurfaceOutput;

using namespace android;

// FIXME: Not used?
// typedef void (*frame_decoded_f)(void *cookie, int width, int height, int pitch, int format, uint8* data);

// This class implements the reference media IO for file output.
// This class constitutes the Media IO component

class BufRetEntry
{
public:
//    uint8 m_pDataAddr;
    PVMFStatus m_status;
    PVMFCommandId m_cmdid;
    OsclAny* m_Context;
    PVMFTimestamp m_Timestamp;    
};
typedef Oscl_Map<void*, BufRetEntry, OsclMemAllocator> BufRetMap;

class BufQueueEntry
{
public:
    uint8* mData;
    uint32 mDataLen;    
    OsclAny* mContext;
    PVMFCommandId mCmdid;
    

    PvmiMediaXferHeader m_data_header_info;
};
typedef Oscl_Queue<BufQueueEntry*, OsclMemAllocator> DispRequestQueue;
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SurfaceOutputReturnData
{
    int mMsg;
    int mExt1;
} SurfaceOutputReturnData;

const char ThreadSafeSurfaceOutputAOName[] = "ThreadSafeSurfaceOutputAO";
class ThreadSafeSurfaceOutputAO : public ThreadSafeCallbackAO
{
    public:
        // Constructor
        ThreadSafeSurfaceOutputAO(
            void* aObserver = NULL,
            uint32 aDepth = DEFAULT_QUEUE_DEPTH,
            const char* aAOname = ThreadSafeSurfaceOutputAOName,
            int32 aPriority = OsclActiveObject::EPriorityHighest);


        // OVERLOADED ProcessEvent
        OsclReturnCode ProcessEvent(OsclAny* EventData);
        virtual ~ThreadSafeSurfaceOutputAO();

        // for surface flinger output
        void notify(int msg, int ext1/*, int ext2*/);
                                                                                                                                    
    private:                                                                                                                        
        // for profiling                                                                                                            
        uint32 iSeqNum;                                                                                                             
};

////////////////////////////////////////////////////////////////////////////////////////////////

class AndroidSurfaceOutput :    public OsclTimerObject
                        ,public PvmiMIOControl
                        ,public PvmiMediaTransfer
                        ,public PvmiCapabilityAndConfig
{
public:
    AndroidSurfaceOutput();

    // parameter initialization
    virtual status_t set(android::PVPlayer* pvPlayer, const sp<ISurface>& surface, bool emulation);
    virtual status_t setVideoSurface(const sp<ISurface>& surface);

    // For frame buffer
    virtual bool initCheck();
    virtual PVMFStatus writeFrameBuf(uint8* aData, uint32 aDataLen, const PvmiMediaXferHeader& data_header_info);
    virtual void postLastFrame();
    virtual void closeFrameBuf();

    virtual ~AndroidSurfaceOutput();

    bool GetVideoSize(int *w, int *h);

    // APIs from PvmiMIOControl

    PVMFStatus connect(PvmiMIOSession& aSession, PvmiMIOObserver* aObserver);

    PVMFStatus disconnect(PvmiMIOSession aSession);

    PVMFCommandId QueryUUID(const PvmfMimeString& aMimeType, Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                            bool aExactUuidsOnly=false, const OsclAny* aContext=NULL);

    PVMFCommandId QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr, const OsclAny* aContext=NULL);

    PvmiMediaTransfer* createMediaTransfer(PvmiMIOSession& aSession, PvmiKvp* read_formats=NULL, int32 read_flags=0,
                                            PvmiKvp* write_formats=NULL, int32 write_flags=0);

    void deleteMediaTransfer(PvmiMIOSession& aSession, PvmiMediaTransfer* media_transfer);

    void processWriteResponseQueue(int numFramesToHold);

    PVMFCommandId Init(const OsclAny* aContext=NULL);

    PVMFCommandId Reset(const OsclAny* aContext=NULL);

    PVMFCommandId Start(const OsclAny* aContext=NULL);

    PVMFCommandId Pause(const OsclAny* aContext=NULL);

    PVMFCommandId Flush(const OsclAny* aContext=NULL);

    PVMFCommandId DiscardData(const OsclAny* aContext=NULL);

    PVMFCommandId DiscardData( PVMFTimestamp aTimestamp=0, const OsclAny* aContext=NULL);

    PVMFCommandId Stop(const OsclAny* aContext=NULL);

    PVMFCommandId CancelAllCommands(const OsclAny* aContext=NULL);

    PVMFCommandId CancelCommand(PVMFCommandId aCmdId, const OsclAny* aContext=NULL);

    void ThreadLogon();

    void ThreadLogoff();

    // APIs from PvmiMediaTransfer

    void setPeer(PvmiMediaTransfer* aPeer);

    void useMemoryAllocators(OsclMemAllocator* write_alloc=NULL);

    PVMFCommandId writeAsync(uint8 format_type, int32 format_index,
            uint8* data, uint32 data_len,
            const PvmiMediaXferHeader& data_header_info,
            OsclAny* aContext=NULL);

    void writeComplete(PVMFStatus aStatus,
            PVMFCommandId  write_cmd_id,
            OsclAny* aContext);

    PVMFCommandId readAsync(uint8* data, uint32 max_data_len,
            OsclAny* aContext=NULL,
            int32* formats=NULL, uint16 num_formats=0);

    void readComplete(PVMFStatus aStatus, PVMFCommandId  read_cmd_id, int32 format_index,
            const PvmiMediaXferHeader& data_header_info, OsclAny* aContext);

    void statusUpdate(uint32 status_flags);

    void cancelCommand(PVMFCommandId  command_id);

    void cancelAllCommands();

    // Pure virtuals from PvmiCapabilityAndConfig

    void setObserver (PvmiConfigAndCapabilityCmdObserver* aObserver);

    PVMFStatus getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier,
            PvmiKvp*& aParameters, int& num_parameter_elements, PvmiCapabilityContext aContext);

    PVMFStatus releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements);

    void createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);

    void setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext, 
            PvmiKvp* aParameters, int num_parameter_elements);

    void DeleteContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);

    void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, 
            int num_elements, PvmiKvp * & aRet_kvp);

    PVMFCommandId setParametersAsync(PvmiMIOSession aSession, PvmiKvp* aParameters, 
            int num_elements, PvmiKvp*& aRet_kvp, OsclAny* context=NULL);

    uint32 getCapabilityMetric (PvmiMIOSession aSession);

    PVMFStatus verifyParametersSync (PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements);


protected:
    void initData();
    void resetVideoParameterFlags();
    bool checkVideoParameterFlags();

    // From OsclTimerObject
    void Run();

    void Reschedule();

    void Cleanup();
    void ResetData();

    PvmiMediaTransfer* iPeer;

    // The PvmiMIOControl class observer.
    PvmiMIOObserver* iObserver;

    //for generating command IDs
    uint32 iCommandCounter;

    //State
    enum PVRefFOState
    {
        STATE_IDLE,
        STATE_LOGGED_ON,
        STATE_INITIALIZED,
        STATE_STARTED,
        STATE_PAUSED
    };
    PVRefFOState iState;

    //Control command handling.
    class CommandResponse
    {
    public:
        CommandResponse(PVMFStatus s,PVMFCommandId id,const OsclAny* ctx)
            :iStatus(s),iCmdId(id),iContext(ctx)
        {}

        PVMFStatus iStatus;
        PVMFCommandId iCmdId;
        const OsclAny* iContext;
    };
    Oscl_Vector<CommandResponse,OsclMemAllocator> iCommandResponseQueue;
    void QueueCommandResponse(CommandResponse&);

    //Write command handling
    class WriteResponse
    {
    public:
        WriteResponse(PVMFStatus s,PVMFCommandId id,const OsclAny* ctx,const PVMFTimestamp& ts)
            :iStatus(s),iCmdId(id),iContext(ctx),iTimestamp(ts)
        {}

        PVMFStatus iStatus;
        PVMFCommandId iCmdId;
        const OsclAny* iContext;
        PVMFTimestamp iTimestamp;
    };
    Oscl_Vector<WriteResponse,OsclMemAllocator> iWriteResponseQueue;

    // Output file parameters
    OSCL_wHeapString<OsclMemAllocator> iOutputFileName;
    Oscl_FileServer iFs;
    bool iFsConnected;
    Oscl_File iOutputFile;
    bool iFileOpened;

    bool iEosReceived;

    // Video parameters
    uint32 iVideoParameterFlags;
    OSCL_HeapString<OsclMemAllocator> iVideoFormatString;
    PVMFFormatType iVideoFormat;
    int32 iVideoHeight;
    int32 iVideoWidth;
    int32 iVideoDisplayHeight;
    int32 iVideoDisplayWidth;

    // hardware specific
    PVMFFormatType iVideoSubFormat;
    bool iVideoSubFormatValid;

    //For logging
    PVLogger* iLogger;

    //For implementing the write flow control
    bool CheckWriteBusy(uint32);

    unsigned long iFrameNumber;

    // software color conversion for software codecs
    ColorConvertBase* iColorConverter;

    android::PVPlayer*          mPvPlayer;
    bool                        mInitialized;
    bool                        mEmulation;
    sp<ISurface>                mSurface;

    // frame buffer support
    // This should match (NUMBER_OUTPUT_BUFFER_AVC = 8) and (NUMBER_OUTPUT_BUFFER_MP4 = 8)
    // Honda [
    //static const int kBufferCount = 8;
    int                         kBufferCount;
    // ]
    int                         mFrameBufferIndex;
    ISurface::BufferHeap        mBufferHeap;
    // Honda [
    //size_t                    mFrameBuffers[kBufferCount];
    size_t                      *mFrameBuffers;
    // ]
#if USE_PMEM  
    // Honda add
    sp<MemoryHeapBaseMTK>       mHeap; 
#endif    
    void convertFrame(void* src, void* dst, size_t len);
    //This bool is set true when all necassary parameters have been received.
    bool iIsMIOConfigured;

    /*
     * The value of mNumberOfFramesToHold is hardware/platform specific.
     * 1. On non-overlay based platforms, its value it set to 2
     *    so as to avoid potential tearings oberved during video playback.
     * 2. On overlay-based platforms, its value should be overwritten.
     *    We have observed video decoder starvation when a value other than 1.
     *
     * We set the default value to 2 in this class. Please change its value
     * accordingly in the derived class.
     */
    int mNumberOfFramesToHold;

#ifdef PERFORMANCE_MEASUREMENTS_ENABLED
        PVProfile PVOmapVideoProfile;
#endif

    // add by Ryan
public:
    // for adding output thread
    void pushBufToRetQueue(uint8* aData, PVMFStatus astatus, PVMFCommandId acmdid, OsclAny* aContext, PVMFTimestamp aTimestamp);    
    PVMFStatus checkRetBuf(uint8* aData);
    PVMFStatus pushBufToRequestQueue(uint8* aData, uint32 aDataLen, const PvmiMediaXferHeader& data_header_info, OsclAny* aContext);
    PVMFStatus handleDispQueue();

    void returnAllBuffers();

    // for av sync
    void SetAVSyncCtrl(PVMFAVSyncbase* aAVSyncCtrl);
    void ResetMIO();
    void ClearBuffer();

    // for synchronization with surface flinger
    void notify(int msg, int ext1/*, int ext2*/);
    uint32 mSeqNum;
    uint32 mPBISeqNum;

    void changePriority(int policy, int priority);
    void triggerOutputThread();
    
protected:
    enum 
    {
        SURFACE_NOP               = 0, 
        SURFACE_END               = 1,
        SURFACE_ERROR             = 100,
        SURFACE_INFO              = 200,
    };
    
    int32 iNumberOfBuffers;
    int32 iBufferSize;
    int mCurBufferIndex;

    // add by Ryan as creating a new thread for output
    void RequestAndWaitForThreadExit();
    
    static int start_surfout_thread_func(TOsclThreadFuncArg arg);
    int surfout_thread_func();

    volatile bool iExitSurfaceThread;
    volatile bool iReturnBuffers;
    bool m_bCreateThread;

    // semaphores used to communicate with the surface output thread
    OsclSemaphore* iSurfaceThreadSem;
    OsclSemaphore* iSurfaceThreadTermSem;

    // lock used to access the oss request queue
    OsclMutex iDispRequestQueueLock;

    // display request queue, needs to use lock mechanism to access
    DispRequestQueue iDispRequestQueue;
    
    PVMFAVSyncbase* mAVSyncCtrl;

    // for synchronization with surface flinger
    uint8* m_DataAddr;

    BufRetMap m_BufRetMap;
    void HandleRetBuf(uint8* addr);
    mutable Mutex   mLock;
    ThreadSafeSurfaceOutputAO* iThreadSafeSurfaceOutputAO;
    
    bool iIsFirstWriteFrameBuf;    
    
    uint32 mDropFrameCount;

    void LogProfile(uint32 aSeqNum);

    // calculate display delay
    long long m_iPostBufferTime;
    long long m_iGetNotifyTime;

    PVMFStatus CheckFirstWriteFrameBuffer(void* aPmemInfo);
};

#endif // ANDROID_SURFACE_OUTPUT_H_INCLUDED

