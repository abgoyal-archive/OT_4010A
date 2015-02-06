
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                  OSCL_T H R E A D (T H R E A D  I M P L E M E N T A T I O N)

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




// Implementation file for OSCL threads
#include "oscl_thread.h"



OSCL_EXPORT_REF OsclThread::OsclThread()
{
    bCreated = false;
    iJoined = false;
}



OSCL_EXPORT_REF OsclThread::~OsclThread()
{

    ;

}


OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::Create(TOsclThreadFuncPtr function_name,
        int32 stack_size,
        TOsclThreadFuncArg argument,
        OsclThread_State state,
        bool oIsJoinable)
{
    if (stack_size < 0)
        return OsclProcStatus::INVALID_PARAM_ERROR;

    if (function_name == NULL)
        return OsclProcStatus::INVALID_PARAM_ERROR;

    //Reset thread creation state, since the thread may
    //have exited.
    if (bCreated)
        bCreated = false;


    switch (state)
    {
        case Start_on_creation:
            break;
        case Suspend_on_creation:
            //can't create suspended pthread.
            return OsclProcStatus::INVALID_PARAM_ERROR;
    }
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (stack_size != 0)
        pthread_attr_setstacksize(&attr, stack_size);

    // Default detachstate attribute to PTHREAD_CREATE_DETACHED state
    int detach_ret;
    if (oIsJoinable)
    {
        detach_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        iJoined = true;
    }
    else
    {
        detach_ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    }
    switch (detach_ret)
    {
        case 0: // successful, continue thread creation
            break;
        case EINVAL:
        default:
            return OsclProcStatus::PSHARED_ATTRIBUTE_SETTING_ERROR;
    }

    int result = pthread_create(
                     (pthread_t*) & ObjThread,
                     &attr,
                     function_name,
                     (void*)argument);

    switch (result)
    {
        case 0:
            bCreated = true;
            return OsclProcStatus::SUCCESS_ERROR;
        case EAGAIN:
            return OsclProcStatus::NOT_ENOUGH_RESOURCES_ERROR;
        default:
            return OsclProcStatus::OTHER_ERROR;
    }

}


OSCL_EXPORT_REF void OsclThread::Exit(OsclAny* exitcode)
{
    //intentionally not implemented.
    OSCL_UNUSED_ARG(exitcode);

}

OSCL_EXPORT_REF void OsclThread::EnableKill()
{
    ; //nothing needed

}

OSCL_EXPORT_REF void OsclThread::SleepMillisec(const int32 msec)
{
    struct timespec reqt, remt;
    reqt.tv_sec = msec / 1000;
    reqt.tv_nsec = 1000000 * (msec % 1000);
    nanosleep(&reqt, &remt) ;

}



OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::Terminate(OsclAny* oscl_ExitCode)
{


    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;

    {
        OSCL_UNUSED_ARG(oscl_ExitCode);

        bCreated = false;
        if (iJoined)
        {
            if (pthread_join(ObjThread, NULL) == 0)
            {
                return OsclProcStatus::SUCCESS_ERROR;
            }
            else
            {
                return OsclProcStatus::OTHER_ERROR;
            }
        }
        return OsclProcStatus::NOT_IMPLEMENTED;
    }
}








OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::Suspend()

{

    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;




    {
        //pthread doesn't support suspend/resume
        return OsclProcStatus::NOT_IMPLEMENTED;

    }


}








OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::Resume()

{

    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;




    {

        //pthread doesn't support suspend/resume
        return OsclProcStatus::NOT_IMPLEMENTED;

    }


}






OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::GetPriority(OsclThreadPriority& refThreadPriority)

{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;

    struct sched_param schedparam;
    int sched_policy = 0;
    int pri;
    int result;
    result = pthread_getschedparam(ObjThread, &sched_policy, &schedparam);
    if (result != 0)
    {
        switch (result)
        {
            case ESRCH:
                return OsclProcStatus::INVALID_THREAD_ERROR;
            case EFAULT:
                return OsclProcStatus::INVALID_PARAM_ERROR;
            default:
                return OsclProcStatus::OTHER_ERROR;
        }
    }

    pri = schedparam.sched_priority;
    switch (sched_policy)
    {
        case SCHED_RR:
        case SCHED_FIFO:
        {
            //static priority is 1..99
            if (pri < 1)
                return OsclProcStatus::OTHER_ERROR;
            else if (pri == 1)
                refThreadPriority = ThreadPriorityLowest;
            else if (pri <= 20)
                refThreadPriority = ThreadPriorityLow;
            else if (pri <= 40)
                refThreadPriority = ThreadPriorityBelowNormal;
            else if (pri <= 55)
                refThreadPriority = ThreadPriorityNormal;
            else if (pri <= 75)
                refThreadPriority = ThreadPriorityAboveNormal;
            else if (pri <= 98)
                refThreadPriority = ThreadPriorityHighest;
            else if (pri == 99)
                refThreadPriority = ThreadPriorityTimeCritical;
            else if (pri > 99)
                return OsclProcStatus::OTHER_ERROR;
            return OsclProcStatus::SUCCESS_ERROR;
        }
        break;

        case SCHED_OTHER:
        {
            //static priority is always zero.
            if (pri != 0)
                return OsclProcStatus::OTHER_ERROR;
            refThreadPriority = ThreadPriorityNormal;
            return OsclProcStatus::SUCCESS_ERROR;
        }
        break;

        default:
            return OsclProcStatus::OTHER_ERROR;
    }
}



OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::SetPriority(OsclThreadPriority ePriority)
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;

    //not supported.
    OSCL_UNUSED_ARG(ePriority);
    return OsclProcStatus::NOT_IMPLEMENTED;

}




OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclThread::GetId(TOsclThreadId& refThreadId)

{




    refThreadId = pthread_self();

    return OsclProcStatus::SUCCESS_ERROR;


}


OSCL_EXPORT_REF bool OsclThread::CompareId(TOsclThreadId &t1, TOsclThreadId &t2)
//static routine
{
    return (pthread_equal(t1, t2) != 0);
}












