

// ---------------------------------------------------------------------
//
// Ryan Lin !!
#undef LOG_TAG
#define LOG_TAG "OsclReadyQ"

#ifndef _WIN32
#include <utils/Log.h>
#define LOG_PROFILE(...) LOGV(__VA_ARGS__)

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE)\
{\
    if (LEVEL == PVLOGMSG_ERR)\
    {\
        ErrMSG MESSAGE;\
    }\
    else\
    {\
        MSG MESSAGE;\
    }\
}
#define ErrMSG(x, ...) LOGE(__VA_ARGS__)
#define MSG(x, ...) LOGV(__VA_ARGS__)
#else

#define LOGD(...)
#define LOGV(...)
#define LOGE(...)

#endif
// ---------------------------------------------------------------------

#include "oscl_scheduler_readyq.h"


#include "oscl_scheduler_ao.h"
#include "oscl_scheduler.h"
#include "oscl_tickcount.h"
#include "oscl_scheduler_tuneables.h"

////////////////////////////////////////
// Ready Queue & Timer Queue Support
////////////////////////////////////////

//scheduler has an allocator but there's no way to use
//it here since this allocator must be a template argument
//to oscl priority queue.
OsclAny* OsclReadyAlloc::allocate_fl(const uint32 size, const char * file_name, const int line_num)
{
    OsclAny*p = iBasicAlloc.allocate_fl(size, file_name, line_num);
    OsclError::LeaveIfNull(p);
    return p;
}
OsclAny* OsclReadyAlloc::allocate(const uint32 size)
{
    OsclAny*p = iBasicAlloc.ALLOCATE(size);
    OsclError::LeaveIfNull(p);
    return p;
}
void OsclReadyAlloc::deallocate(OsclAny* p)
{
    iBasicAlloc.deallocate(p);
}

//evalute "priority of a is less than priority of b"
int OsclTimerCompare::compare(TOsclReady& a, TOsclReady& b)
{
    //Sort by time to run.  Earlier "time to run" has precedence.
    if (a->iPVReadyQLink.iTimeToRunTicks != b->iPVReadyQLink.iTimeToRunTicks)
    {
        //calculate a>b, taking possible rollover into account.
        uint32 delta = b->iPVReadyQLink.iTimeToRunTicks - a->iPVReadyQLink.iTimeToRunTicks;
        return (delta > OsclExecScheduler::iTimeCompareThreshold);
    }

    //Now sort by priority
    return OsclReadyCompare::compare(a, b);
}

//evalute "priority of a is less than priority of b"
int OsclReadyCompare::compare(TOsclReady& a, TOsclReady& b)
{
    //Sort by AO priority.  Higher priority has precedence.
    if (a->iPVReadyQLink.iAOPriority != b->iPVReadyQLink.iAOPriority)
        return (a->iPVReadyQLink.iAOPriority < b->iPVReadyQLink.iAOPriority);

    //if there was a priority tie, impose a FIFO order.

    //This section allows switching between "fair scheduling" and linear
    //behavior.  We always use fair scheduling, but for testing it can be helpful to
    //swap in the linear behavior.
#if PV_SCHED_FAIR_SCHEDULING
    //Sort by FIFO order, to create fair scheduling.
    //AOs that have been queued the longest have precedence.
    return (a->iPVReadyQLink.iSeqNum >= b->iPVReadyQLink.iSeqNum);
#else
    //Sort by the order when AO was added to scheduler, to simulate
    //Symbian native ActiveScheduler behavior.
    //AOs that were added earlier have precedence.
    return (a->iAddedNum > b->iAddedNum);
#endif

}

////////////////////////////////////////
//OsclReadyQ
////////////////////////////////////////
void OsclReadyQ::Construct(int nreserve)
{
    iSeqNumCounter = 0;
    if (nreserve > 0)
        c.reserve(nreserve);
    iCallback = NULL;
}

void OsclReadyQ::ThreadLogon()
{
    iSem.Create();
    iCrit.Create();
}

void OsclReadyQ::ThreadLogoff()
{
    iSem.Close();
    iCrit.Close();
}

//
//Note: all semaphore errors are fatal, since they can cause
// scheduler to spin or hang.
//

PVActiveBase* OsclReadyQ::WaitAndPopTop()
//block until an AO is ready and pop the highest pri AO.
{
    switch (iSem.Wait())
    {
        case OsclProcStatus::SUCCESS_ERROR:
            return PopTopAfterWait();

        default:
            OsclError::Leave(OsclErrSystemCallFailed);
            return NULL;
    }
}

PVActiveBase* OsclReadyQ::WaitAndPopTop(uint32 aTimeoutVal)
//block until an AO is ready or timeout is reached.
{
    switch (iSem.Wait(aTimeoutVal))
    {
        case OsclProcStatus::WAIT_TIMEOUT_ERROR:
            //timeout reached, no AO ready.
            return NULL;

        case OsclProcStatus::SUCCESS_ERROR:
            //an AO is ready-- pop it.
            return PopTopAfterWait();

        default:
            //no timeout and no AO.
            OsclError::Leave(OsclErrSystemCallFailed);
            return NULL;
    }
}

bool OsclReadyQ::IsIn(TOsclReady b)
//tell if elemement is in this q
{
    iCrit.Lock();
    bool isin = (b->iPVReadyQLink.iIsIn == this);
    iCrit.Unlock();
    return isin;
}

PVActiveBase* OsclReadyQ::PopTop()
//deque and return highest pri element.
{
    iCrit.Lock();

    PVActiveBase*elem = (size() > 0) ? top() : NULL;
    if (elem)
    {
#if SHOW_DEBUG        
        if (elem->iName.Str() != NULL)
        LOGD("OsclReadyQ::PopTop - %s, %d", elem->iName.Str(), gettid());
#endif        
        elem->iPVReadyQLink.iIsIn = NULL;

        pop();

        //this call won't block-- it will just decrement the sem.
        if (iSem.Wait() != OsclProcStatus::SUCCESS_ERROR)
            OsclError::Leave(OsclErrSystemCallFailed);
    }

    iCrit.Unlock();
    return elem;
}

PVActiveBase* OsclReadyQ::PopTopAfterWait()
//deque and return highest pri element.  used when we have
//already done a sem wait prior to this call, so there is no sem
//decrement here.
{
    iCrit.Lock();

    PVActiveBase*elem = (size() > 0) ? top() : NULL;

    if (elem)
    {
#if SHOW_DEBUG
        if (elem->iName.Str() != NULL)
        LOGD("OsclReadyQ::PopTopAfterWait - %s, %d", elem->iName.Str(), gettid());
#endif        
        elem->iPVReadyQLink.iIsIn = NULL;
        pop();
    }
    else
    {//there should always be an element available after a sem wait.
        OSCL_ASSERT(0);
    }

    iCrit.Unlock();

    return elem;
}

PVActiveBase* OsclReadyQ::Top()
//return highest pri element without removing.
{
    iCrit.Lock();
    PVActiveBase* elem = (size() > 0) ? top() : NULL;
    iCrit.Unlock();
    return elem;
}

void OsclReadyQ::Remove(TOsclReady a)
//remove the given element
{
    iCrit.Lock();
#if SHOW_DEBUG
    if (a != NULL && a->iName.Str()!=NULL)
    LOGD("OsclReadyQ::Remove - %s, %d", a->iName.Str(), gettid());
#endif    
    a->iPVReadyQLink.iIsIn = NULL;

    int32 nfound = remove(a);

    if (nfound > 0)
    {
        //this call won't block-- it will just decrement the sem.
        if (iSem.Wait() != OsclProcStatus::SUCCESS_ERROR)
            OsclError::Leave(OsclErrSystemCallFailed);
    }

    iCrit.Unlock();
}

int32 OsclReadyQ::PendComplete(PVActiveBase *pvbase, int32 aReason)
//Complete an AO request
{
    iCrit.Lock();

    //make sure this AO is not already queued.
    int32 err;
    if (pvbase->IsInAnyQ())
    {
        if (pvbase->iName.Str()!= NULL)
        {
            LOGE("name: %s, id: %d - OsclErrInvalidState", pvbase->iName.Str(), gettid());
        }
        err = OsclErrInvalidState;//EExecAlreadyAdded
    }
    //make sure the AO has a request active
    else if (!pvbase->iBusy
             || pvbase->iStatus != OSCL_REQUEST_PENDING)
    {
        if (pvbase->iName.Str() != NULL)
        {
            LOGE("name: %s, id: %d - OsclErrCorrupt", pvbase->iName.Str(), gettid());
        }
        err = OsclErrCorrupt;//EExecStrayEvent;
    }
    else
    {
        if (pvbase->iName.Str() != NULL)
        {
#if SHOW_DEBUG            
            LOGD("name: %s, id: %d - PendComplete iIsIn", pvbase->iName.Str(), gettid());
#endif
        }
        //Add to pri queue
        pvbase->iPVReadyQLink.iIsIn = this;
        pvbase->iPVReadyQLink.iTimeQueuedTicks = OsclTickCount::TickCount();
        pvbase->iPVReadyQLink.iSeqNum = ++iSeqNumCounter;//for the FIFO sort
        push(pvbase);

        //increment the sem
        if (iSem.Signal() != OsclProcStatus::SUCCESS_ERROR)
        {
            err = OsclErrSystemCallFailed;
        }
        else
        {
            //update the AO status
            pvbase->iStatus = aReason;

            //make scheduler callback if needed.
            //note: this needs to happen under the lock since we're updating
            //the callback pointer.
            if (iCallback)
            {
                iCallback->OsclSchedulerReadyCallback(iCallbackContext);
                iCallback = NULL;
            }
            err = OsclErrNone;
        }
    }

    iCrit.Unlock();
    return err;
}

int32 OsclReadyQ::WaitForRequestComplete(PVActiveBase* pvbase)
//Wait on a particular request to complete
{
    int32 nwait = 0;//count the number of request sem waits.

    for (bool complete = false; !complete;)
    {
        //Wait on any request to complete.
        if (iSem.Wait() != OsclProcStatus::SUCCESS_ERROR)
            return OsclErrSystemCallFailed;

        nwait++;

        //Some request was complete but it might not be the one of interest,
        //so check for completion again.
        complete = IsIn(pvbase);
    }

    //Restore the request semaphore value since we may have decremented it without
    //removing anything from the ReadyQ.
    while (nwait > 0)
    {
        if (iSem.Signal() != OsclProcStatus::SUCCESS_ERROR)
            return OsclErrSystemCallFailed;

        nwait--;
    }

    return OsclErrNone;
}

void OsclReadyQ::RegisterForCallback(OsclSchedulerObserver* aCallback, OsclAny* aCallbackContext)
{
    //Use the Q lock to avoid thread contention over
    //callback pointer.
    iCrit.Lock();

    //Callback right away if ready Q is non-empty.
    if (size() && aCallback)
    {
        iCallback = NULL;
        aCallback->OsclSchedulerReadyCallback(aCallbackContext);
    }
    else
    {
        //save the new pointers.  Callback will happen when timer Q or ready Q is
        //updated.
        iCallback = aCallback;
        iCallbackContext = aCallbackContext;
    }
    iCrit.Unlock();
}

void OsclReadyQ::TimerCallback(uint32 aDelayMicrosec)
//Inform scheduler observer of a change in the shortest timer interval
{
    //lock when updating callback pointer.
    iCrit.Lock();
    OsclSchedulerObserver* callback = iCallback;
    iCallback = NULL;
    iCrit.Unlock();

    //Timer callback happens outside the lock, to allow code
    //to register for additional callback.
    if (callback)
        callback->OsclSchedulerTimerCallback(iCallbackContext, aDelayMicrosec / 1000);
}

////////////////////////////////////////
//OsclTimerQ
////////////////////////////////////////
void OsclTimerQ::Construct(int nreserve)
{
    iSeqNumCounter = 0;
    if (nreserve > 0)
        c.reserve(nreserve);
}

bool OsclTimerQ::IsIn(TOsclReady b)
//tell if element is in this q
{
    return (b->iPVReadyQLink.iIsIn == this);
}

PVActiveBase* OsclTimerQ::PopTop()
//deque and return highest pri element.
{
    PVActiveBase*elem = (size() > 0) ? top() : NULL;
    if (elem)
        Pop(elem);

    return elem;
}

PVActiveBase* OsclTimerQ::Top()
//return highest pri element without removing.
{
    return (size() > 0) ? top() : NULL;
}

void OsclTimerQ::Pop(TOsclReady b)
//remove queue top.
{
#if SHOW_DEBUG    
    if (b!=NULL && b->iName.Str()!=NULL)
    LOGD("OsclReadyQ::Pop - %s, %d", b->iName.Str(), gettid());
#endif

    b->iPVReadyQLink.iIsIn = NULL;
    pop();
}

void OsclTimerQ::Remove(TOsclReady a)
{
#if SHOW_DEBUG    
    if (a!=NULL && a->iName.Str()!=NULL)
    LOGD("OsclReadyQ::Remove - %s, %d", a->iName.Str(), gettid());
#endif

    a->iPVReadyQLink.iIsIn = NULL;
    remove(a);
}

void OsclTimerQ::Add(TOsclReady b)
{
#if SHOW_DEBUG    
    if (b!=NULL && b->iName.Str()!=NULL)
    LOGD("OsclReadyQ::Add - %s, %d", b->iName.Str(), gettid());
#endif

    b->iPVReadyQLink.iIsIn = this;
    b->iPVReadyQLink.iTimeQueuedTicks = OsclTickCount::TickCount();
    b->iPVReadyQLink.iSeqNum = ++iSeqNumCounter;//for the FIFO sort

    push(b);
}








