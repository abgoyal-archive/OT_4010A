
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ P R O F I L E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_PROFILE_H_INCLUDED
#define OSCL_PROFILE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

typedef Oscl_Map<uint32, long long, OsclMemAllocator> ProfileMap;

class OsclProfile
{
    public:
        enum SHOW_OPTION
        {
            E_SHOW_NONE         = 0x0000,

            E_SHOW_TICK_COUNT   = 0x0001,
            E_SHOW_SYSTEM_TIME  = 0x0002,

            E_SHOW_MAX
        };

        static void Push(ProfileMap& aMap, uint32 aSeqNum, long long aTime = 0);
        static long long Pop(ProfileMap& aMap, uint32 aSeqNum);
        static void ClearMap();
        static void Start();
        static void Stop();
        
        // parser node
        static ProfileMap iRetrieveTrackDataInTime; 
        static ProfileMap iRetrieveTrackDataOutTime;
        static ProfileMap iSendTrackDataOutTime;
        // base decode node
        static ProfileMap iSendInBufToOMXInTime;
        static ProfileMap iSendOutBufToOMXInTime;
        // OMX mpeg4 decoder in
        static ProfileMap iMp4DecodeVideoInTime;
        // OMX mpeg4 decoder out
        static ProfileMap iMp4DecodeVideoOutTime;
        // base decode node
        static ProfileMap iCallbackFillBufferDoneInTime;
        static ProfileMap iProcessCallbackFillBufferDoneInTime;
        static ProfileMap iFillBufferDoneProcessingInTime;
        // video decode nocde
        static ProfileMap iQueueOutputBufferInTime;
        // media output node inport
        static ProfileMap iReceiveYUVDataInTime;
        static ProfileMap iSendMediaDataInTime;
        // android surface output
        static ProfileMap iSurfaceFlingerNotifyInTime;
        static ProfileMap iPostBufferInTime;
        static ProfileMap iReturnBufferInTime;

    private:
        static void LogMap(ProfileMap& aMap, uint32 aSeqNum);
};

#endif // OSCL_PROFILE_H_INCLUDED

/*! @} */
