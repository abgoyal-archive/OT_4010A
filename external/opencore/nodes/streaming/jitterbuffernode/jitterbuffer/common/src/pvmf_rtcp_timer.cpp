
#ifndef PVMF_RTCP_TIMER_H_INCLUDED
#include "pvmf_rtcp_timer.h"
#endif

#ifndef PVMF_JITTER_BUFFER_COMMON_INTERNAL_H
#include "pvmf_jitter_buffer_common_internal.h"
#endif

#define RTCP_HOLD_DATA_SIZE 2

////////////////////////////////////////////////////////////////////////////
PvmfRtcpTimer::PvmfRtcpTimer(PvmfRtcpTimerObserver* aObserver)
        : OsclTimerObject(OsclActiveObject::EPriorityNominal, "PvmfRtcpTimer"),
        iRTCPTimeIntervalInMicroSecs(DEFAULT_RTCP_INTERVAL_USEC),
        iObserver(aObserver),
        iStarted(false)
{
    iBufAlloc = NULL;
    ipLogger = PVLogger::GetLoggerObject("PvmfRtcpTimer");
    AddToScheduler();
    iRTCPBufAlloc.ipRTCPRRMsgBufAlloc = createRTCPRRBufAllocReSize();
}

////////////////////////////////////////////////////////////////////////////
PvmfRtcpTimer::~PvmfRtcpTimer()
{
    Stop();
    if (iBufAlloc != NULL)
    {
        iBufAlloc->removeRef();
        iBufAlloc = NULL;
    }
    if (iImplAlloc != NULL)
    {
        OSCL_DELETE(iImplAlloc);
    }
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PvmfRtcpTimer::Start()
{
    PVMF_JB_LOGINFO((0, "PvmfRtcpTimer::Start iRTCPTimeIntervalInMicroSecs Interval %d", iRTCPTimeIntervalInMicroSecs));
    if (iRTCPTimeIntervalInMicroSecs > 0)
    {
        RunIfNotReady(iRTCPTimeIntervalInMicroSecs);
        iStarted = true;
        return PVMFSuccess;
    }
    else
    {
        return PVMFFailure;
    }
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PvmfRtcpTimer::setRTCPInterval(uint32 rtcpTimeIntervalInMicroSecs)
{
    PVMF_JB_LOGINFO((0, "PvmfRtcpTimer::ResetRTCPInterval"));
    iRTCPTimeIntervalInMicroSecs = rtcpTimeIntervalInMicroSecs;
    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PvmfRtcpTimer::Stop()
{
    PVMF_JB_LOGINFO((0, "PvmfRtcpTimer::Stop"));
    Cancel();
    iStarted = false;
    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
void PvmfRtcpTimer::Run()
{
    PVMF_JB_LOGINFO((0, "PvmfRtcpTimer::Run"));

    if (!iStarted)
        return;

    if (!iObserver)
    {
        PVMF_JB_LOGERROR((0, "PvmfRtcpTimer::Run: Error - Observer not set"));
        return;
    }

    iObserver->RtcpTimerEvent();
    /*
     * Do not reschudule the AO here. Observer would reschedule this AO
     * once it is done processing the timer event.
     */
}

PVMFResizableSimpleMediaMsgAlloc*
PvmfRtcpTimer::createRTCPRRBufAllocReSize()
{
    int32 leavecode = 0;

    OSCL_TRY(leavecode,
             iBufAlloc = OSCL_NEW(OsclMemPoolResizableAllocator, (DEFAULT_RTCP_SOCKET_MEM_POOL_SIZE_IN_BYTES, 1));
             iImplAlloc = OSCL_NEW(PVMFResizableSimpleMediaMsgAlloc, (iBufAlloc));
            );

    if (leavecode || (!iBufAlloc) || (!iImplAlloc))
    {
        PVMF_JB_LOGERROR((0, "PvmfRtcpTimer::createRTCPRRBufAllocReSize: Error - Memory allocation failed"));
        OSCL_LEAVE(OsclErrNoMemory);
    }

    iBufAlloc->enablenullpointerreturn();

    return iImplAlloc;
}
