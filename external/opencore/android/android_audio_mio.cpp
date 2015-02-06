

//#define LOG_NDEBUG 0
#define LOG_TAG "AudioMIO"
#include <utils/Log.h>

#include "android_audio_mio.h"
#include "pvlogger.h"
#include "pv_mime_string_utils.h"
#include "oscl_snprintf.h"
#include "oscl_mem.h"
#include "oscl_dll.h"
#include "oscl_mem.h"

#include <sys/prctl.h>

#include <utils/Timers.h>
#include <sys/resource.h>
#include <limits.h>

#include <utils/threads.h>

#include <media/AudioTrack.h>

#include "oscl_file_cfg.h"
#include "pvmf_common_audio_decnode.h"

// Ryan Lin !!
#undef LOG_TAG
#define LOG_TAG "AudioMIO"

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

using namespace android;

// depth of buffer/command queues in MIO
static const int kCommandQueueDepth = 10;

// Define entry point for this DLL
OSCL_DLL_ENTRY_POINT_DEFAULT()

// Audio Media IO component base class implementation
OSCL_EXPORT_REF AndroidAudioMIO::AndroidAudioMIO(const char* name)
    : OsclTimerObject(OsclActiveObject::EPriorityNominal, name),
    iWriteCompleteAO(NULL)
{
    initData();
}

OSCL_EXPORT_REF AndroidAudioMIO::~AndroidAudioMIO()
{
    LOGP("destructor");
    Cleanup();
}

void AndroidAudioMIO::initData()
{
    LOGP("initData in");
    ResetData();

    iCommandCounter = 0;
    iLogger = NULL;
    iCommandResponseQueue.reserve(kCommandQueueDepth);
    iWriteResponseQueueLock.Create();
    iWriteResponseQueue.reserve(kCommandQueueDepth);
    iObserver = NULL;
    iLogger = NULL;
    iPeer = NULL;
    iState = STATE_MIO_IDLE;
    iWriteBusy = false;
    iFlushPending = false;
    iDataQueued = 0;
    LOGP("initData out");
}

void AndroidAudioMIO::ResetData()
//reset all data from this session.
{
    LOGP("ResetData in");
    // reset all the received media parameters.
    iAudioFormat = PVMF_MIME_FORMAT_UNKNOWN;
    iAudioNumChannelsValid = false;
    iAudioSamplingRateValid = false;
    iAudioThreadCreatedAndMIOConfigured = false;
    LOGP("ResetData out");
}

//cleanup all allocated memory and release resources.
void AndroidAudioMIO::Cleanup()
{
    LOGP("Cleanup in");
    while (!iCommandResponseQueue.empty()) {
        if (iObserver) {
            iObserver->RequestCompleted(PVMFCmdResp(iCommandResponseQueue[0].iCmdId,
                        iCommandResponseQueue[0].iContext, iCommandResponseQueue[0].iStatus));
        }
        iCommandResponseQueue.erase(&iCommandResponseQueue[0]);
    }

    // return empty buffers
    returnAllBuffers();

    // delete the request active object
    if (iWriteCompleteAO) {
        OSCL_DELETE(iWriteCompleteAO);
        iWriteCompleteAO = NULL;
    }
    iWriteResponseQueueLock.Close();
    LOGP("Cleanup out");
}

PVMFStatus AndroidAudioMIO::connect(PvmiMIOSession& aSession, PvmiMIOObserver* aObserver)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::connect() called"));

    // currently supports only one session
    if (iObserver) return PVMFFailure;
    iObserver = aObserver;
    return PVMFSuccess;
}

PVMFStatus AndroidAudioMIO::disconnect(PvmiMIOSession aSession)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::disconnect() called"));
    iObserver = NULL;
    return PVMFSuccess;
}


PvmiMediaTransfer* AndroidAudioMIO::createMediaTransfer(PvmiMIOSession& aSession,
                            PvmiKvp* read_formats, int32 read_flags,
                            PvmiKvp* write_formats, int32 write_flags)
{
    // create the request active object
    // such when audio output thread is done with a buffer
    // it can put the buffer on the write response queue
    // and schedule this MIO to run, to return the buffer
    // to the engine
    // <--- Donglei Ji ALPS00130835 (5 to 9)
    iWriteCompleteAO = OSCL_NEW(AndroidAudioOutputThreadSafeCallbackAO,(this, 9));
    // --->

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::createMediaTransfer() called"));
    return (PvmiMediaTransfer*)this;
}

PVMFCommandId AndroidAudioMIO::QueueCmdResponse(PVMFStatus status, const OsclAny* aContext)
{
    PVMFCommandId cmdId = iCommandCounter++;
    CommandResponse resp(status, cmdId, aContext);
    iCommandResponseQueue.push_back(resp);

    // cancel any timer delay so the command response will happen ASAP.
    if (IsBusy()) Cancel();
    RunIfNotReady();
    return cmdId;
}

// return any held buffers to the engine
void AndroidAudioMIO::ProcessWriteResponseQueue()
{
    //LOGP("ProcessWriteResponseQueue in [Response Q size %d]",iWriteResponseQueue.size());
    PVMFStatus status = 0;
    PVMFCommandId cmdId = 0;
    const OsclAny* context = 0;

    iWriteResponseQueueLock.Lock();
    while (!iWriteResponseQueue.empty()) {
        status = iWriteResponseQueue[0].iStatus;
        cmdId = iWriteResponseQueue[0].iCmdId;
        context = (OsclAny*)iWriteResponseQueue[0].iContext;
        iWriteResponseQueue.erase(&iWriteResponseQueue[0]);
        iWriteResponseQueueLock.Unlock();
        if (iPeer) {
            LOGP("Return buffer(%d) status(%d) context(%p)", cmdId,status,context);
            iPeer->writeComplete(status, cmdId, (OsclAny*)context);
        }
        iWriteResponseQueueLock.Lock();
    }
    iWriteResponseQueueLock.Unlock();
    //LOGP("ProcessWriteResponseQueue out");
}

PVMFCommandId AndroidAudioMIO::QueryUUID(const PvmfMimeString& aMimeType,
                                        Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                        bool aExactUuidsOnly, const OsclAny* aContext)
{
    LOGP("QueryUUID");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::QueryUUID() called"));
    return QueueCmdResponse(PVMFFailure, aContext);
}

PVMFCommandId AndroidAudioMIO::QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr, const OsclAny* aContext)
{
    LOGP("QueryInterface");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::QueryInterface() called"));
    PVMFStatus status=PVMFFailure;
    if (aUuid == PVMI_CAPABILITY_AND_CONFIG_PVUUID) {
        PvmiCapabilityAndConfig* myInterface = OSCL_STATIC_CAST(PvmiCapabilityAndConfig*, this);
        aInterfacePtr = OSCL_STATIC_CAST(PVInterface*, myInterface);
        status = PVMFSuccess;
    }
    return QueueCmdResponse(status, aContext);
}

PVMFCommandId AndroidAudioMIO::Init(const OsclAny* aContext)
{
    LOGP("Init (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Init() called"));
    iState=STATE_MIO_INITIALIZED;
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::Reset(const OsclAny* aContext)
{
    LOGP("Reset (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Reset() called"));
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::Start(const OsclAny* aContext)
{
    // Start is NO-OP
    LOGP("Start (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Start() called"));
    iState = STATE_MIO_STARTED;
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::Pause(const OsclAny* aContext)
{
    LOGP("Pause (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Pause() called"));
    iState = STATE_MIO_PAUSED;
    return QueueCmdResponse(PVMFSuccess, aContext);
}


PVMFCommandId AndroidAudioMIO::Flush(const OsclAny* aContext)
{
    LOGP("Flush (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Flush() called"));
    iState = STATE_MIO_INITIALIZED;
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::DiscardData(const OsclAny* aContext)
{
    LOGP("DiscardData (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::DiscardData() called"));
    return DiscardData(UINT_MAX, aContext);
}

PVMFCommandId AndroidAudioMIO::DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext)
{
    LOGP("DiscardData (%u, %p)", aTimestamp, aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::DiscardData() called"));
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::Stop(const OsclAny* aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::Stop() called"));
    iState = STATE_MIO_INITIALIZED;
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::CancelAllCommands(const OsclAny* aContext)
{
    LOGP("CancelAllCommands (%p)", aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::CancelAllCommands() called"));

    //commands are executed immediately upon being received, so
    //it isn't really possible to cancel them.
    return QueueCmdResponse(PVMFSuccess, aContext);
}

PVMFCommandId AndroidAudioMIO::CancelCommand(PVMFCommandId aCmdId, const OsclAny* aContext)
{
    LOGP("CancelCommand (%u, %p)", aCmdId, aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::CancelCommand() called"));

    //see if the response is still queued.
    PVMFStatus status = PVMFFailure;
    for (uint32 i = 0; i < iCommandResponseQueue.size(); i++) {
        if (iCommandResponseQueue[i].iCmdId == aCmdId) {
            status = PVMFSuccess;
            break;
        }
    }
    return QueueCmdResponse(PVMFSuccess, aContext);
}

void AndroidAudioMIO::ThreadLogon()
{
    LOGP("ThreadLogon() called ");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::ThreadLogon() called"));
    if (iState == STATE_MIO_IDLE) {
        iLogger = PVLogger::GetLoggerObject("AndroidAudioMIO\n");
        AddToScheduler();
        iState=STATE_MIO_LOGGED_ON;
    }
}

void AndroidAudioMIO::ThreadLogoff()
{
    LOGP("ThreadLogoff() called");
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::ThreadLogoff() called"));
    if (iState!=STATE_MIO_IDLE) {
        RemoveFromScheduler();
        iLogger = NULL;
        iState = STATE_MIO_IDLE;
        ResetData();
    }
}

void AndroidAudioMIO::setPeer(PvmiMediaTransfer* aPeer)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0,"AndroidAudioMIO::setPeer() called"));
    // Set the observer
    iPeer = aPeer;
}

//This routine will determine whether data can be accepted in a writeAsync
//call and if not, will return true;
bool AndroidAudioMIO::CheckWriteBusy(uint32 aSeqNum)
{
    // FIXME: this line screws up video output - why?
    // return (iOSSRequestQueue.size() >= 5);
    return false;
}

PVMFCommandId AndroidAudioMIO::writeAsync(uint8 aFormatType, int32 aFormatIndex, uint8* aData, uint32 aDataLen,
                                        const PvmiMediaXferHeader& data_header_info, OsclAny* aContext)
{
    // Do a leave if MIO is not configured except when it is an EOS
    if (!iAudioThreadCreatedAndMIOConfigured
            &&
            !((PVMI_MEDIAXFER_FMT_TYPE_NOTIFICATION == aFormatType)
              && (PVMI_MEDIAXFER_FMT_INDEX_END_OF_STREAM == aFormatIndex)))
    {
        LOGE("ERROR :: data is pumped in before mio configured");
        OSCL_LEAVE(OsclErrInvalidState);
        return -1;
    }

    uint32 aSeqNum = data_header_info.seq_num;
    PVMFTimestamp aTimestamp = data_header_info.timestamp;
    uint32 flags = data_header_info.flags;

    bool bWriteComplete = true;

    //LOGP("writeAsync() called seqnum %d ts %d flags %d context %d formattype %d formatindex %d",aSeqNum, aTimestamp, flags,aContext,aFormatType,aFormatIndex);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
        (0,"AndroidAudioMIO::writeAsync() seqnum %d ts %d flags %d context %d",
         aSeqNum, aTimestamp, flags,aContext));

    PVMFStatus status=PVMFFailure;

    switch(aFormatType) {
    case PVMI_MEDIAXFER_FMT_TYPE_COMMAND :
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidAudioMIO::writeAsync() called with Command info."));
        //ignore
        LOGP("PVMI_MEDIAXFER_FMT_TYPE_COMMAND");
        status = PVMFSuccess;
        break;

    case PVMI_MEDIAXFER_FMT_TYPE_NOTIFICATION :
        PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
            (0,"AndroidAudioMIO::writeAsync() called with Notification info."));
        switch(aFormatIndex) {
        case PVMI_MEDIAXFER_FMT_INDEX_END_OF_STREAM:
            LOGP("PVMI_MEDIAXFER_FMT_INDEX_END_OF_STREAM");
            bWriteComplete = false; //force an empty buffer through the audio thread
            break;
        default:
            break;
        }
        //ignore
        status = PVMFSuccess;
        break;

    case PVMI_MEDIAXFER_FMT_TYPE_DATA :
        switch(aFormatIndex) {
        case PVMI_MEDIAXFER_FMT_INDEX_FMT_SPECIFIC_INFO:
            status = PVMFSuccess;
            LOGP("PVMI_MEDIAXFER_FMT_INDEX_FMT_SPECIFIC_INFO");
            break;

        case PVMI_MEDIAXFER_FMT_INDEX_DATA:
            LOGP("PVMI_MEDIAXFER_FMT_INDEX_DATA");
            //data contains the media bitstream.

            //Check whether we can accept data now and leave if we can't.
            if (CheckWriteBusy(aSeqNum)) {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                    (0,"AndroidAudioMIO::writeAsync: Entering busy state"));

                //schedule an event to re-start the data flow.
                iWriteBusy = true;
                bWriteComplete = false;

                // Rich:: commenting in this line.
                //        Need some timeout here just in case.
                //        I have no evidence of any problems.
                RunIfNotReady(10 * 1000);
                OSCL_LEAVE(OsclErrBusy);
            } else {
                if (aDataLen > 0) {
                    // this buffer will be queued by the audio output thread to process
                    // this buffer cannot be write completed until
                    // it has been processed by the audio output thread
                    bWriteComplete = false;
                } else {
                       LOGE("writeAsync() called with aDataLen==0");
                       PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_INFO,
                            (0,"AndroidAudioMIO::writeAsync() called aDataLen==0."));
                }
                status = PVMFSuccess;
            }
            break;

        default:
            LOGE("Error unrecognized format index =%u", aFormatIndex);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                (0,"AndroidAudioMIO::writeAsync: Error - unrecognized format index"));
            status = PVMFFailure;
            break;
        }
        break;

    default:
        LOGE("Error unrecognized format type =%u", aFormatType);
        PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
            (0,"AndroidAudioMIO::writeAsync: Error - unrecognized format type"));
        status = PVMFFailure;
        break;
    }

    //Schedule asynchronous response
    PVMFCommandId cmdid=iCommandCounter++;
    if (bWriteComplete) {
        LOGP("write complete (%d)", cmdid);
        WriteResponse resp(status, cmdid, aContext, aTimestamp);
        iWriteResponseQueueLock.Lock();
        iWriteResponseQueue.push_back(resp);
        iWriteResponseQueueLock.Unlock();
        RunIfNotReady();
    } else if (!iWriteBusy) {
        writeAudioBuffer(aData, aDataLen, cmdid, aContext, aTimestamp);
    }
    return cmdid;
}

PVMFStatus AndroidAudioMIO::getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier,
                                              PvmiKvp*& aParameters, int& num_parameter_elements,
                                              PvmiCapabilityContext aContext)
{
    LOGP("getParametersSync in %s",aIdentifier);
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    aParameters=NULL;
    num_parameter_elements=0;
    PVMFStatus status = PVMFFailure;

    if(pv_mime_strcmp(aIdentifier, INPUT_FORMATS_CAP_QUERY) == 0) {
        //This is a query for the list of supported formats.
        aParameters=(PvmiKvp*)oscl_malloc(2 * sizeof(PvmiKvp));
        if (aParameters) {
            aParameters[num_parameter_elements++].value.pChar_value = (char*)PVMF_MIME_PCM16;
            aParameters[num_parameter_elements++].value.pChar_value = (char*)PVMF_MIME_PCM8;
            status = PVMFSuccess;
        }
        else{
            status = PVMFErrNoMemory;
        }
    }
    //other queries are not currently supported.

    //unrecognized key.
    LOGP("getParametersSync out status %d",status);
    return status;
}

PVMFStatus AndroidAudioMIO::releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements)
{
    LOGP("releaseParameters in");
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(num_elements);
    PVMFStatus status = PVMFFailure;
    //release parameters that were allocated by this component.
    if (aParameters) {
        oscl_free(aParameters);
        status = PVMFSuccess;
    }
    LOGP("releaseParameters out status %d",status);
    return status;
}

PVMFStatus AndroidAudioMIO::verifyParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    //LOGP("verifyParametersSync in");

    // Go through each parameter
    for (int32 i=0; i<num_elements; i++) {
        char* compstr = NULL;
        pv_mime_string_extract_type(0, aParameters[i].key, compstr);
        if (pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/media/format-type")) == 0) {
            if ((pv_mime_strcmp(aParameters[i].value.pChar_value, PVMF_MIME_PCM8) == 0) ||
                (pv_mime_strcmp(aParameters[i].value.pChar_value, PVMF_MIME_PCM16) == 0)
               ) {
                return PVMFSuccess;
            }
            else {
                return PVMFErrNotSupported;
            }
        }
    }
    return PVMFSuccess;
    //LOGP("verifyParametersSync out");

}
void AndroidAudioMIO::setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                        int num_elements, PvmiKvp * & aRet_kvp)
{
    OSCL_UNUSED_ARG(aSession);
    //LOGP("setParametersSync in");
    aRet_kvp = NULL;

    for (int32 i=0;i<num_elements;i++) {
        //Check against known audio parameter keys...
        if (pv_mime_strcmp(aParameters[i].key, MOUT_AUDIO_FORMAT_KEY) == 0) {
            LOGP("Audio format: %s", aParameters[i].value.pChar_value);
            iAudioFormat=aParameters[i].value.pChar_value;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidAudioOutput::setParametersSync() Audio Format Key, Value %s",aParameters[i].value.pChar_value));
        } else if (pv_mime_strcmp(aParameters[i].key, MOUT_AUDIO_SAMPLING_RATE_KEY) == 0) {
            iAudioSamplingRate=(int32)aParameters[i].value.uint32_value;
            iAudioSamplingRateValid=true;
            // LOGP("iAudioSamplingRate=%d", iAudioSamplingRate);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidAudioMIO::setParametersSync() Audio Sampling Rate Key, Value %d",iAudioSamplingRate));
        } else if (pv_mime_strcmp(aParameters[i].key, MOUT_AUDIO_NUM_CHANNELS_KEY) == 0) {
            iAudioNumChannels=(int32)aParameters[i].value.uint32_value;
            iAudioNumChannelsValid=true;
            // LOGP("iAudioNumChannels=%d", iAudioNumChannels);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidAudioMIO::setParametersSync() Audio Num Channels Key, Value %d",iAudioNumChannels));
        }
        //All FSI for audio will be set here in one go
#if 0
        else if (pv_mime_strcmp(aParameters[i].key, PVMF_FORMAT_SPECIFIC_INFO_KEY_PCM) == 0)
        {
            uint8* data = (uint8*)aParameters->value.key_specific_value;
            channelSampleInfo* pcm16Info = (channelSampleInfo*)data;

            iAudioSamplingRate = pcm16Info->samplingRate;
            iAudioSamplingRateValid = true;
            iAudioNumChannels = pcm16Info->desiredChannels;
            iAudioNumChannelsValid = true;

            //iFmtSubchunk.sampleRate = iAudioSamplingRate;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                            (0, "PVRefFileOutput::setParametersSync() Audio Sampling Rate, Value %d", iAudioSamplingRate));



            iFmtSubchunk.numChannels = iAudioNumChannels;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                            (0, "PVRefFileOutput::setParametersSync() Audio Num Channels, Value %d", iAudioNumChannels));


            iFmtSubchunk.bitsPerSample = (int32)pcm16Info->bitsPerSample;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                            (0, "PVRefFileOutput::setParametersSync() Audio Bits Per Sample, Value %d", iFmtSubchunk.bitsPerSample));

            iNumberOfBuffers = (int32)pcm16Info->num_buffers;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                            (0, "PVRefFileOutput::setParametersSync() Number of Buffer, Value %d", iNumberOfBuffers));

            iBufferSize = (int32)pcm16Info->buffer_size;
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                            (0, "PVRefFileOutput::setParametersSync() Buffer Size, Value %d", iBufferSize));
        }
#endif
        else {
            //if we get here the key is unrecognized.
            PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                (0,"AndroidAudioMIO::setParametersSync() Error, unrecognized key %s",aParameters[i].key));

            //set the return value to indicate the unrecognized key
            //and return.
            aRet_kvp = &aParameters[i];
            return;
        }
    }

    //LOGP("setParametersSync out");
}

//
// Private section
//

void AndroidAudioMIO::Run()
{
    while (!iCommandResponseQueue.empty()) {
        if (iObserver) {
            iObserver->RequestCompleted(PVMFCmdResp(iCommandResponseQueue[0].iCmdId,
                        iCommandResponseQueue[0].iContext,
                        iCommandResponseQueue[0].iStatus));
        }
        iCommandResponseQueue.erase(&iCommandResponseQueue[0]);
    }

    //send async write completion
    ProcessWriteResponseQueue();

    //Re-start the data transfer if needed.
    if (iWriteBusy) {
        iWriteBusy = false;
        iPeer->statusUpdate(PVMI_MEDIAXFER_STATUS_WRITE);
    }
}

// send response to MIO
void AndroidAudioMIO::sendResponse(PVMFCommandId cmdid, const OsclAny* context, PVMFTimestamp timestamp)
{
    //LOGP("sendResponse :: return buffer (%d) timestamp(%d) context(%p)", cmdid, timestamp, context);
    WriteResponse resp(PVMFSuccess, cmdid, context, timestamp);
    iWriteResponseQueueLock.Lock();
    if (iWriteResponseQueue.size() < iWriteResponseQueue.capacity()) {
        iWriteResponseQueue.push_back(resp);
    } else {
        LOGE("Exceeded response queue capacity");
    }
    iWriteResponseQueueLock.Unlock();

    // Create an event for the threadsafe callback AO
    OsclAny* P = NULL;
    iWriteCompleteAO->ReceiveEvent(P);
}

void AndroidAudioMIO::setAudioSink(const sp<MediaPlayerInterface::AudioSink>& audioSink)
{
    LOGP("setAudioSink in");
    mAudioSink = audioSink;
}

//------------------------------------------------------------------------
// Active timing support
//
AndroidAudioMIOActiveTimingSupport::AndroidAudioMIOActiveTimingSupport(int32 minCorrection, int32 maxCorrection) :
        iClock(NULL),
        iClockNotificationsInf(NULL),
        iClockState(PVMFMediaClock::STOPPED),
        iAudioThreadSem(0),
        iStartTime(0),
        iFrameCount(0),
        iMsecsPerFrame(25),
        iDriverLatency(0),
        iUpdateClock(true),
        iMinCorrection(minCorrection),
        iMaxCorrection(maxCorrection)
{
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_audiooutput");
}

OSCL_EXPORT_REF PVMFStatus AndroidAudioMIOActiveTimingSupport::SetClock(PVMFMediaClock *clockVal)
{
    LOGP("ATS :: SetClock in");
    iClock=clockVal;

    return PVMFSuccess;
}

void AndroidAudioMIOActiveTimingSupport::NotificationsInterfaceDestroyed()
{
    LOGP("ATS :: NotificationsInterfaceDestroyed in");
    iClockNotificationsInf=NULL;
}


OSCL_EXPORT_REF bool AndroidAudioMIOActiveTimingSupport::queryInterface(const PVUuid& aUuid, PVInterface*& aInterface)
{
    aInterface = NULL;
    PVUuid uuid;
    queryUuid(uuid);
    bool status = false;
    if (uuid == aUuid) {
        PvmiClockExtensionInterface* myInterface = OSCL_STATIC_CAST(PvmiClockExtensionInterface*, this);
        aInterface = OSCL_STATIC_CAST(PVInterface*, myInterface);
        status = true;
    }
    LOGP("ATS :: queryInterface out status %d",status);
    return status;
}

void AndroidAudioMIOActiveTimingSupport::queryUuid(PVUuid& uuid)
{
    LOGP("ATS :: queryUuid in");
    uuid = PvmiClockExtensionInterfaceUuid;
}

void AndroidAudioMIOActiveTimingSupport::ClockStateUpdated()
{
    LOGP("ATS :: ClockStateUpdated in");
    if (iClock) {

        PVMFMediaClock::PVMFMediaClockState newClockState = iClock->GetState();
        if (newClockState != iClockState) {
            iClockState = newClockState;
            switch (iClockState) {
            case PVMFMediaClock::STOPPED:
                LOGP("A/V clock stopped");
                break;
            case PVMFMediaClock::RUNNING:
                LOGP("A/V clock running");
                // must be seeking, get new clock offset for A/V sync
                if (iUpdateClock) {
                    iUpdateClock = false;
                    UpdateStartTime();
                }
                if (iAudioThreadSem){
                    LOGP("signal thread to start");
                    iAudioThreadSem->Signal();
                }
                break;
            case PVMFMediaClock::PAUSED:
                //mAudioSink->pause();
                LOGP("A/V clock paused mAudioSink->pause ();");
                break;
            default:
                LOGE("Wrong clock state!");
                break;
            }
        }
    }
    LOGP("ATS :: ClockStateUpdated out");
}

void AndroidAudioMIOActiveTimingSupport::UpdateClock()
{
    if (iClock && (iClockState == PVMFMediaClock::RUNNING)) {
        uint32 clockTime32, timeBaseTime32, updateClock32;
        int32 correction = 0;
        bool overflowFlag = false;
        // get current time
        iClock->GetCurrentTime32(clockTime32, overflowFlag, PVMF_MEDIA_CLOCK_MSEC, timeBaseTime32);
        // calculate sample clock
        updateClock32 = iFrameCount * iMsecsPerFrame;

        correction = updateClock32 - (clockTime32 - iStartTime);
        LOGP("ADJ_CLK iFrameCount= %u iMsecsPerFrame = %f updateClock32 = %u clockTime32 = %u iStartTime = %u",
        iFrameCount,iMsecsPerFrame, updateClock32, clockTime32, iStartTime);

        // do clock correction if drift exceeds threshold
        if (OSCL_ABS(correction) > iMinCorrection) {
            if (correction > iMaxCorrection) {
                correction = iMaxCorrection;
            } else if (correction < -iMaxCorrection) {
                correction = -iMaxCorrection;
            }
            updateClock32 = clockTime32 + correction;
            LOGP("ADJ_CLK old clock %d correction %d new clock %d", clockTime32, correction, updateClock32);

            PVMFMediaClockAdjustTimeStatus adjustmentstatus =
            iClock->AdjustClockTime32(clockTime32, timeBaseTime32, updateClock32,PVMF_MEDIA_CLOCK_MSEC,overflowFlag);

            if ((PVMF_MEDIA_CLOCK_ADJUST_SUCCESS != adjustmentstatus))
            {
                LOGE("Error adjusting clock status = %d",adjustmentstatus);
            }
            if(overflowFlag) {
            LOGE("Adjusting clock caused overflow");
            }
        }
        else{
            LOGP("ATS :: sample clock and pv clock are close enough no need to update");
        }
    }
}
void AndroidAudioMIOActiveTimingSupport::setDriverLatency(uint32 latency)
{
    LOGP("ATS :: setDriverLatency in");
    iDriverLatency = latency;
    if (iClock){
        LOGP("register latency to media clock and set clockobserver");
        PVMFStatus ret = iClock->ConstructMediaClockNotificationsInterface(iClockNotificationsInf,*this,latency);
        if(iClockNotificationsInf && (PVMFSuccess == ret))
        {
            iClockNotificationsInf->SetClockStateObserver(*this);
        }
        else
        {
            LOGE("latency could NOT be set !! set it later ");
        }

// <--- Morris 20100623 for RTSP streaming <--Jun 122346 extend to local playback, so delte if check.
       // if (iClock->IsRtspStreamingClock())
        {
            this->ClockStateUpdated();
        }
// --->

    }
    LOGP("ATS :: setDriverLatency out");
}

void AndroidAudioMIOActiveTimingSupport::setAudioSink(const sp<MediaPlayerInterface::AudioSink>& audioSink)
{
    LOGP("AndroidAudioMIOActiveTimingSupport setAudioSink in");
    mAudioSink = audioSink;
}



void AndroidAudioMIOActiveTimingSupport::UpdateStartTime()
{
    bool overflowFlag = false;
    uint32 currentTimeBase32 = 0;
    iClock->GetCurrentTime32(iStartTime, overflowFlag, PVMF_MEDIA_CLOCK_MSEC, currentTimeBase32);
    iFrameCount = 0;
    LOGP("update iStartTime: %d", iStartTime);
}

