
#ifndef PVMF_ML_EOS_TIMER_H_INCLUDED
#define PVMF_ML_EOS_TIMER_H_INCLUDED

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

class PvmfEOSTimerObserver
{
    public:
        /**
         * A timer event, indicating that the EOS timer has expired.
         */
        virtual void EOSTimerEvent() = 0;
};

class PvmfEOSTimer : public OsclTimerObject
{
    public:
        OSCL_IMPORT_REF PvmfEOSTimer(PvmfEOSTimerObserver* aObserver);

        OSCL_IMPORT_REF virtual ~PvmfEOSTimer();

        /** Start EOS Timer */
        OSCL_IMPORT_REF PVMFStatus Start();

        OSCL_IMPORT_REF PVMFStatus setSessionDuration(uint32 sessionDurationInMS);

        /** Stop Timer events */
        OSCL_IMPORT_REF PVMFStatus Stop();

        bool IsTimerStarted()
        {
            return iStarted;
        }

    private:
        void Run();

        uint32 iSessionDurationInMS;
        PvmfEOSTimerObserver* iObserver;
        PVLogger* iLogger;
        bool iStarted;
};
#endif // PVMF_JB_EOS_TIMER_H_INCLUDED




