
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


//                  OSCL_S E M A P H O R E (S E M A P H O R E  I M P L E M E N T A T I O N)


// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


// Implementation file for OSCL Semaphores
#include "oscl_semaphore.h"
#include "oscl_assert.h"



// Class contructor
OSCL_EXPORT_REF OsclSemaphore::OsclSemaphore()
{
    bCreated = false;
}

OSCL_EXPORT_REF OsclSemaphore::~OsclSemaphore()
{
    //make sure it was closed
    OSCL_ASSERT(!bCreated);
}


OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Create(uint32 InitialCount)
{
    if (bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    iCount = InitialCount;
    //add by Chipeng
    MaximunCount = -1;
    if (pthread_cond_init(&ObjCondition, NULL) == 0
            && pthread_mutex_init(&ObjMutex, NULL) == 0)
    {
        bCreated = true;
        return OsclProcStatus::SUCCESS_ERROR;
    }
    return OsclProcStatus::OTHER_ERROR;

}

OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Create(uint32 InitialCount,int32 MaxCount)
{
    if (bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;

    iCount = InitialCount;
    //add by Chipeng
    if(MaxCount >= 0){
        MaximunCount = MaxCount;
        }
    else{
        MaximunCount = -1;
    }
    if (pthread_cond_init(&ObjCondition, NULL) == 0
            && pthread_mutex_init(&ObjMutex, NULL) == 0)
    {
        bCreated = true;
        return OsclProcStatus::SUCCESS_ERROR;
    }
    return OsclProcStatus::OTHER_ERROR;

}



OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Close(void)
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;

    bCreated = false;
    return OsclProcStatus::SUCCESS_ERROR;

}



OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Wait()
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int res = pthread_mutex_lock(&ObjMutex);
    if (res != 0)
        return OsclProcStatus::OTHER_ERROR;
    while (iCount == 0 && res == 0)
    {   // wait till the semaphore is signaled
        // or an error has occurred
        res = pthread_cond_wait(&ObjCondition, &ObjMutex);
    }
    if (res == 0)
    {   // signaled
        iCount --;
    }
    pthread_mutex_unlock(&ObjMutex);
    if (res != 0)
    {   // error occurred
        // invalid condition or mutex
        return OsclProcStatus::OTHER_ERROR;
    }
    return OsclProcStatus::SUCCESS_ERROR;

}

#include "osclconfig_time.h"
static void getAbsTime(struct timespec &abs, uint32 timeout_msec)
{
    //convert timeout msec to sec + usec.
    uint32 timeout_sec = timeout_msec / 1000;
    uint32 timeout_usec = (timeout_msec % 1000) * 1000;

    //compute target time = time now plus timeout.

    struct timeval timenow;
    gettimeofday(&timenow, NULL);

    uint32 targ_sec = timenow.tv_sec + timeout_sec;
    uint32 targ_usec = timenow.tv_usec + timeout_usec;
    while (targ_usec >= 1000000)
    {
        targ_sec++;
        targ_usec -= 1000000;
    }

    //convert target time to timespec.
    abs.tv_sec = targ_sec;
    abs.tv_nsec = targ_usec * 1000;
}

OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Wait(uint32 timeout_msec)
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int res = pthread_mutex_lock(&ObjMutex);
    if (res != 0)
        return OsclProcStatus::OTHER_ERROR;
    if (iCount > 0)
    {
        iCount--;
        pthread_mutex_unlock(&ObjMutex);
        return OsclProcStatus::SUCCESS_ERROR;
    }

    struct timespec abs;
    getAbsTime(abs, timeout_msec);

    while ((iCount == 0) && (res == 0))
    {   // wait till semaphore is signaled
        // or time runs out
        res = pthread_cond_timedwait(&ObjCondition, &ObjMutex, &abs);
    }
    if (res == 0)
    {   // signalied
        iCount--;
    }
    pthread_mutex_unlock(&ObjMutex);
    if (res == ETIMEDOUT)
    {   // timeour occurred
        return OsclProcStatus::WAIT_TIMEOUT_ERROR;
    }
    else if (res != 0)
    {   // error occurred
        return OsclProcStatus::OTHER_ERROR;
    }
    return OsclProcStatus::SUCCESS_ERROR;

}

OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::TryWait()
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int res = pthread_mutex_lock(&ObjMutex);
    if (res != 0)
        return OsclProcStatus::OTHER_ERROR;
    if (iCount > 0)
    {//take it
        iCount--;
        pthread_mutex_unlock(&ObjMutex);
        return OsclProcStatus::SUCCESS_ERROR;
    }
    else
    {//can't take it now so don't block.
        pthread_mutex_unlock(&ObjMutex);
        return OsclProcStatus::SEM_NOT_SIGNALED_ERROR;
    }

}


OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclSemaphore::Signal()
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int res = pthread_mutex_lock(&ObjMutex);
    if (res != 0)
        return OsclProcStatus::OTHER_ERROR;
    iCount++;
    //add by Chipeng
    if(iCount > (uint32)MaximunCount && MaximunCount >= 0){
        iCount= MaximunCount;
        }
    res = pthread_cond_signal(&ObjCondition);
    pthread_mutex_unlock(&ObjMutex);
    if (res != 0)
        return OsclProcStatus::OTHER_ERROR;
    return OsclProcStatus::SUCCESS_ERROR;


}

