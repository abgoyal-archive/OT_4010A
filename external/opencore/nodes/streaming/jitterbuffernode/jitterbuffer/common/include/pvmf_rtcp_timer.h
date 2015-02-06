
#ifndef PVMF_RTCP_TIMER_H_INCLUDED
#define PVMF_RTCP_TIMER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif
#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif
#ifndef PVMF_RETURN_CODES_H_INCLUDED
#include "pvmf_return_codes.h"
#endif
#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif
#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif
#ifndef PVMF_RESIZABLE_SIMPLE_MEDIAMSG_H_INCLUDED
#include "pvmf_resizable_simple_mediamsg.h"
#endif
#ifndef PVMF_SM_TUNABLES_H_INCLUDED
#include "pvmf_sm_tunables.h"
#endif

#define PVMF_MEDIA_DATA_CLASS_SIZE 128

/* RTCP Packet Mem Pool Allocator */
class PVMFRTCPMemPool
{
    public:
        PVMFRTCPMemPool(uint32 aNumRTCPBufs = DEFAULT_RTCP_MEM_POOL_BUFFERS): ipMediaDataMemPool(NULL)
        {
            ipMediaDataMemPool = OSCL_NEW(OsclMemPoolFixedChunkAllocator, (aNumRTCPBufs, PVMF_MEDIA_DATA_CLASS_SIZE));
        }

        ~PVMFRTCPMemPool()
        {
            if (ipMediaDataMemPool)
            {
                ipMediaDataMemPool->removeRef();
            }
        }

        OsclSharedPtr<PVMFMediaDataImpl> GetMediaDataImpl(uint32 size)
        {
            return (ipRTCPRRMsgBufAlloc->allocate(size));
        }

        PVMFResizableSimpleMediaMsgAlloc* ipRTCPRRMsgBufAlloc;
        /* Memory pool for media data objects */
        OsclMemPoolFixedChunkAllocator* ipMediaDataMemPool;
};

class PvmfRtcpTimer;

class PvmfRtcpTimerObserver
{
    public:
        virtual ~PvmfRtcpTimerObserver() {}
        /**
         * A timer event, indicating that the RTCP timer has expired.
         */
        virtual void RtcpTimerEvent() = 0;
};

class PvmfRtcpTimer : public OsclTimerObject
{
    public:
        PvmfRtcpTimer(PvmfRtcpTimerObserver* aObserver);

        virtual ~PvmfRtcpTimer();

        /** Start RTCP Timer */
        PVMFStatus Start();

        /** Reset RTCP Timer */
        PVMFStatus setRTCPInterval(uint32 rtcpTimeIntervalInMicroSecs);

        /** Stop Timer events */
        PVMFStatus Stop();

        PVMFRTCPMemPool* getRTCPBuffAlloc()
        {
            return &iRTCPBufAlloc;
        }

    private:
        void Run();

        PVMFResizableSimpleMediaMsgAlloc* createRTCPRRBufAllocReSize();

        uint32 iRTCPTimeIntervalInMicroSecs;
        PvmfRtcpTimerObserver* iObserver;
        PVLogger* ipLogger;
        bool iStarted;

        PVMFRTCPMemPool iRTCPBufAlloc;
        OsclMemPoolResizableAllocator* iBufAlloc;
        PVMFResizableSimpleMediaMsgAlloc* iImplAlloc;
};
#endif // PVMF_RTCP_TIMER_H_INCLUDED
