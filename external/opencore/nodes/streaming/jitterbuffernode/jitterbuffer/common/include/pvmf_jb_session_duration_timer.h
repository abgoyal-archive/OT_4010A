
#ifndef PVMF_JB_SESSION_DURATION_TIMER_H_INCLUDED
#define PVMF_JB_SESSION_DURATION_TIMER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
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
#ifndef PVMF_SM_TUNABLES_H_INCLUDED
#include "pvmf_sm_tunables.h"
#endif


class PvmfJBSessionDurationTimerObserver
{
    public:
        virtual ~PvmfJBSessionDurationTimerObserver() {}
        /**
         * A timer event, indicating that the timer has expired.
         */
        virtual void PVMFJBSessionDurationTimerEvent() = 0;
};

class PvmfJBSessionDurationTimer : public OsclTimerObject
{
    public:
        PvmfJBSessionDurationTimer(PvmfJBSessionDurationTimerObserver* aObserver);

        virtual ~PvmfJBSessionDurationTimer();

        /** Start Timer */
    // <--- Morris Yang ALPS00225250
        PVMFStatus Start(bool bStartFromSetPlayRange = false);
        bool IsStartFromSetPlayRange() { return iStartFromSetPlayRange; }
    // --->

        PVMFStatus setSessionDurationInMS(uint32 duration);

        uint32 getSessionDurationInMS()
        {
            return iSessionDurationInMS;
        }

        /** Stop Timer events */
        PVMFStatus Stop();

        virtual PVMFStatus Cancel();

        bool IsTimerStarted()
        {
            return iStarted;
        }

        void SetEstimatedServerClock(PVMFMediaClock* aEstimatedServerClock)
        {
            iEstimatedServerClock = aEstimatedServerClock;
        }

        void EstimatedServerClockUpdated();

        uint32 GetExpectedEstimatedServClockValAtSessionEnd()
        {
            return iExpectedEstimatedServClockValAtSessionEnd;
        }

        void setCurrentMonitoringIntervalInMS(uint32 aCurrentMonitoringIntervalInMS)
        {
            iCurrentMonitoringIntervalInMS = aCurrentMonitoringIntervalInMS;
        }

        uint64 GetMonitoringIntervalElapsed()
        {
            return iMonitoringIntervalElapsed;
        }

        void UpdateElapsedSessionDuration(uint32 aElapsedTime)
        {
            iElapsedSessionDurationInMS += aElapsedTime;
        }

        // <--- Morris Yang ALPS00225250
        void SetElapsedSessionDuration(uint32 aElapsedTime)
        {
            iElapsedSessionDurationInMS = aElapsedTime;
        }
        // --->

        uint32 GetElapsedSessionDurationInMS()
        {
            return iElapsedSessionDurationInMS;
        }

        uint32 GetEstimatedServClockValAtLastCancel()
        {
            return iEstimatedServClockValAtLastCancel;
        }

        void ResetEstimatedServClockValAtLastCancel()
        {
            iEstimatedServClockValAtLastCancel = 0;
            if (iEstimatedServerClock != NULL)
            {
                uint32 timebase32 = 0;
                bool overflowFlag = false;
                iEstimatedServerClock->GetCurrentTime32(iEstimatedServClockValAtLastCancel, overflowFlag,
                                                        PVMF_MEDIA_CLOCK_MSEC, timebase32);
            }
        }

    private:
        void Run();

        uint32 iCurrentMonitoringIntervalInMS;
        uint32 iSessionDurationInMS;
        uint32 iElapsedSessionDurationInMS;
        PvmfJBSessionDurationTimerObserver* iObserver;
        PVLogger* ipLogger;
        bool iStarted;

        PVMFMediaClock iClock;
        PVMFTimebase_Tickcount iClockTimeBase;
        uint32 iTimerStartTimeInMS;
        uint64 iMonitoringIntervalElapsed;

        PVMFMediaClock* iEstimatedServerClock;
        uint32 iEstimatedServClockValAtLastCancel;
        uint32 iExpectedEstimatedServClockValAtSessionEnd;

        PVLogger *ipClockLoggerSessionDuration;

    // <--- Morris Yang ALPS00225250
        bool iStartFromSetPlayRange;
    // --->
};


#endif // PVMF_JB_SESSION_DURATION_TIMER_H_INCLUDED




