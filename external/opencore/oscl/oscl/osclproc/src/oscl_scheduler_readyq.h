





#ifndef OSCL_SCHEDULER_READYQ_H_INCLUDED
#define OSCL_SCHEDULER_READYQ_H_INCLUDED

#ifndef OSCL_SCHEDULER_TYPES_H_INCLUDED
#include "oscl_scheduler_types.h"
#endif
#ifndef OSCL_SCHEDULER_TUNEABLES_H_INCLUDED
#include "oscl_scheduler_tuneables.h"
#endif


#ifndef OSCL_PRIQUEUE_H_INCLUDED
#include "oscl_priqueue.h"
#endif
#ifndef OSCL_BASE_ALLOC_H_INCLUDED
#include "oscl_base_alloc.h"
#endif
#ifndef OSCL_SEMAPHORE_H_INCLUDED
#include "oscl_semaphore.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif
#ifndef OSCL_MUTEX_H_INCLUDED
#include "oscl_mutex.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

class PVActiveBase;

class OsclReadyAlloc: public Oscl_DefAlloc
{
    public:
        OsclAny* allocate(const uint32 size) ;
        OsclAny* allocate_fl(const uint32 size, const char * file_name, const int line_num);
        void deallocate(OsclAny* p) ;
    private:
        OsclMemAllocator iBasicAlloc;
};

typedef PVActiveBase* TOsclReady;

class OsclReadyCompare
{
    public:
        static int compare(TOsclReady& a, TOsclReady& b) ;
};
class OsclTimerCompare
{
    public:
        static int compare(TOsclReady& a, TOsclReady& b) ;
};

class PVLogger;
class OsclSchedulerObserver;
class OsclReadyQ
        : public OsclPriorityQueue<TOsclReady, OsclReadyAlloc, Oscl_Vector<TOsclReady, OsclReadyAlloc>, OsclReadyCompare>
{
    public:
        void Construct(int);
        void ThreadLogon();
        void ThreadLogoff();

        void Remove(TOsclReady);

        bool IsIn(TOsclReady);

        uint32 Depth()
        {
            return size();
        }

        TOsclReady PopTop();
        TOsclReady Top();

        TOsclReady WaitAndPopTop();
        TOsclReady WaitAndPopTop(uint32);

        int32 PendComplete(PVActiveBase *pvbase, int32 aReason);
        int32 WaitForRequestComplete(PVActiveBase*);

        //For non-blocking scheduler observer support
        void RegisterForCallback(OsclSchedulerObserver* aCallback, OsclAny* aCallbackContext);
        void TimerCallback(uint32 aDelayMicrosec);
        OsclSchedulerObserver* Callback()
        {
            return iCallback;
        }

    private:
        TOsclReady PopTopAfterWait();

        //mutex for thread protection
        OsclNoYieldMutex iCrit;

        //this semaphore tracks the queue size.  it is used to
        //regulate the scheduling loop when running in blocking mode.
        OsclSemaphore iSem;

        //a sequence number needed to maintain FIFO sorting order in oscl pri queue.
        uint32 iSeqNumCounter;

        //For non-blocking scheduler observer support
        OsclSchedulerObserver* iCallback;
        OsclAny* iCallbackContext;
};

class OsclTimerQ
        : public OsclPriorityQueue<TOsclReady, OsclReadyAlloc, Oscl_Vector<TOsclReady, OsclReadyAlloc>, OsclTimerCompare>
{
    public:
        void Construct(int);
        void Add(TOsclReady);
        void Remove(TOsclReady);
        TOsclReady PopTop();
        TOsclReady Top();
        void Pop(TOsclReady);
        bool IsIn(TOsclReady);
    private:
        //a sequence number needed to maintain FIFO sorting order in oscl pri queue.
        uint32 iSeqNumCounter;
};

class TReadyQueLink
{
    public:
        TReadyQueLink()
        {
            iAOPriority = 0;
            iTimeToRunTicks = 0;
            iSeqNum = 0;
            iIsIn = NULL;
        }

        int32 iAOPriority;//scheduling priority
        uint32 iTimeToRunTicks;//for timers, this is the time to run in ticks.
        uint32 iTimeQueuedTicks;//the time when the AO was queued, in ticks.
        uint32 iSeqNum;//sequence number for oscl pri queue.
        OsclAny* iIsIn;//pointer to the queue we're in, cast as a void*

};

#endif


/*! @} */
