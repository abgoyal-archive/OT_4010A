
#include "oscl_tickcount.h"
#include "oscl_profile.h"
#include "oscl_file_cfg.h"

#include <utils/Log.h>

// for profiling add by Ryan Lin
// parser node
OSCL_EXPORT_REF ProfileMap OsclProfile::iRetrieveTrackDataInTime;           
OSCL_EXPORT_REF ProfileMap OsclProfile::iRetrieveTrackDataOutTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iSendTrackDataOutTime;
// base decode node
OSCL_EXPORT_REF ProfileMap OsclProfile::iSendInBufToOMXInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iSendOutBufToOMXInTime; 
// OMX mpeg4 decoder in
OSCL_EXPORT_REF ProfileMap OsclProfile::iMp4DecodeVideoInTime;
// OMX mpeg4 decoder out
OSCL_EXPORT_REF ProfileMap OsclProfile::iMp4DecodeVideoOutTime;
// base decode node
OSCL_EXPORT_REF ProfileMap OsclProfile::iCallbackFillBufferDoneInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iProcessCallbackFillBufferDoneInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iFillBufferDoneProcessingInTime;
// video decode nocde
OSCL_EXPORT_REF ProfileMap OsclProfile::iQueueOutputBufferInTime;           
// media output node inport
OSCL_EXPORT_REF ProfileMap OsclProfile::iReceiveYUVDataInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iSendMediaDataInTime;
// android surface output
OSCL_EXPORT_REF ProfileMap OsclProfile::iSurfaceFlingerNotifyInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iPostBufferInTime;
OSCL_EXPORT_REF ProfileMap OsclProfile::iReturnBufferInTime;

#undef LOG_TAG
#define LOG_TAG "LogMap"

#define SHOW_LOG 0
//#define MSG(x, ...) LOGD(__VA_ARGS__)
void OsclProfile::LogMap(ProfileMap& aMap, uint32 aSeqNum)
{
    char* _LogName = NULL;
    
    uint8* ptr = (uint8*)&aMap;
    if (ptr == (uint8*)&iRetrieveTrackDataInTime)
    {
        _LogName = (char*)"RTI";
    }
    else if (ptr == (uint8*)&iRetrieveTrackDataOutTime)
    {
        _LogName = (char*)"RTO";
    }
    else if (ptr == (uint8*)&iSendTrackDataOutTime)
    {
        _LogName = (char*)"STO";
    }
    else if (ptr == (uint8*)&iSendInBufToOMXInTime)
    {
        _LogName = (char*)"SIB";
    }
    else if (ptr == (uint8*)&iSendOutBufToOMXInTime)
    {
        _LogName = (char*)"SOB";
    }
    else if (ptr == (uint8*)&iCallbackFillBufferDoneInTime)
    {
        _LogName = (char*)"CFBD";
    }
    else if (ptr == (uint8*)&iProcessCallbackFillBufferDoneInTime)
    {
        _LogName = (char*)"PCBFBD";
    }
    else if (ptr == (uint8*)&iFillBufferDoneProcessingInTime)
    {
        _LogName = (char*)"FBD";
    }            
    else if (ptr == (uint8*)&iQueueOutputBufferInTime)
    {
        _LogName = (char*)"QOB";
    }
    else if (ptr == (uint8*)&iReceiveYUVDataInTime)
    {
        _LogName = (char*)"RDI";
    }
    else if (ptr == (uint8*)&iSendMediaDataInTime)
    {
        _LogName = (char*)"SMD";
    }
    else if (ptr == (uint8*)&iSurfaceFlingerNotifyInTime)
    {
        _LogName = (char*)"SFN";
    }
    else if (ptr == (uint8*)&iPostBufferInTime)
    {
        _LogName = (char*)"PBI";
    }
    else if (ptr == (uint8*)&iReturnBufferInTime)
    {
        _LogName = (char*)"RBI";
    }

    LOGD("%s: %d", _LogName, aSeqNum);
}

#undef LOG_TAG
#define LOG_TAG "profiling"

static bool bPushToMap = false;

OSCL_EXPORT_REF void OsclProfile::Push(ProfileMap& aMap, uint32 aSeqNum, long long aTime)
{
    if (bPushToMap == false)
    {
        return;
    }
    
    if (aSeqNum == 0 && aMap.size()!=0)
    {
        aMap.clear();
    }
    long long _time = aTime;
    if (aTime == 0)
    {
        _time = OsclTickCount::getTimeMs();
    }

#if SHOW_LOG
    LogMap(aMap, aSeqNum);
#endif

    ProfileMap::value_type value(aSeqNum, _time);
    if (aMap.insert(value).second == false)
    {
        LogMap(aMap, aSeqNum);
        
        LOGE("OsclProfile::Push error - seq = %d", aSeqNum);
        OSCL_ASSERT(false);
    }
}

OSCL_EXPORT_REF long long OsclProfile::Pop(ProfileMap& aMap, uint32 aSeqNum)
{
    long long ret = 0;
    ProfileMap::iterator _iterator = aMap.find(aSeqNum);

    ret = (*_iterator).second;
    aMap.erase(aSeqNum);

    return ret;
}

OSCL_EXPORT_REF void OsclProfile::ClearMap()
{
    iRetrieveTrackDataInTime.clear();
    iRetrieveTrackDataOutTime.clear();
    iSendTrackDataOutTime.clear();
    iSendInBufToOMXInTime.clear();
    iSendOutBufToOMXInTime.clear();
    iQueueOutputBufferInTime.clear();
    iMp4DecodeVideoInTime.clear();
    iMp4DecodeVideoOutTime.clear();
    iCallbackFillBufferDoneInTime.clear();
    iProcessCallbackFillBufferDoneInTime.clear();
    iFillBufferDoneProcessingInTime.clear();
    iReceiveYUVDataInTime.clear();
    iSendMediaDataInTime.clear();
    iSurfaceFlingerNotifyInTime.clear();
    iPostBufferInTime.clear();
    iReturnBufferInTime.clear();
}

OSCL_EXPORT_REF void OsclProfile::Start()
{
    bPushToMap = true;
}

OSCL_EXPORT_REF void OsclProfile::Stop()
{
    bPushToMap = false;
}

