
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                  OSCL_MUTEX (M U T E X  I M P L E M E N T A T I O N)

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#include "oscl_mutex.h"


#include "oscl_assert.h"

OSCL_EXPORT_REF OsclMutex::OsclMutex()
{
    bCreated = false;
}

OSCL_EXPORT_REF OsclMutex::~OsclMutex()
{
    //make sure it's closed
    if (bCreated)
        Close();
}

OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclMutex::Create()
{
    if (bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int result = pthread_mutex_init(&ObjMutex, NULL);
    if (result == 0)
    {
        bCreated = true;
        return OsclProcStatus::SUCCESS_ERROR;
    }
    else
        return ErrorMapping(result);


}


OSCL_EXPORT_REF void OsclMutex::Lock()
{
    //verify the mutex is created.
    OSCL_ASSERT(bCreated);


    pthread_mutex_lock(&ObjMutex);

}


OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclMutex::TryLock()
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int result = pthread_mutex_trylock(&ObjMutex);
    switch (result)
    {
        case 0:
            return OsclProcStatus::SUCCESS_ERROR;
        case EBUSY:
            return OsclProcStatus::MUTEX_LOCKED_ERROR;
        default:
            return ErrorMapping(result);
    }

}

OSCL_EXPORT_REF void OsclMutex::Unlock()
{
    //verify the mutex is created.
    OSCL_ASSERT(bCreated);


    pthread_mutex_unlock(&ObjMutex);

}



OSCL_EXPORT_REF OsclProcStatus::eOsclProcError OsclMutex::Close()
{
    if (!bCreated)
        return OsclProcStatus::INVALID_OPERATION_ERROR;


    int result = pthread_mutex_destroy(&ObjMutex);
    if (result == 0)
    {
        bCreated = false;
        return OsclProcStatus::SUCCESS_ERROR;
    }
    else
        return ErrorMapping(result);

}



OsclProcStatus::eOsclProcError OsclMutex::ErrorMapping(int32 Error)
{


    switch (Error)
    {
        case 0:
            return OsclProcStatus::SUCCESS_ERROR;
        case EAGAIN :
            return  OsclProcStatus::SYSTEM_RESOURCES_UNAVAILABLE_ERROR;
        case EINVAL :
            return  OsclProcStatus::INVALID_ARGUMENT_ERROR;
        case ENOMEM :
            return  OsclProcStatus::NOT_ENOUGH_MEMORY_ERROR;
        case EFAULT  :
            return  OsclProcStatus::INVALID_POINTER_ERROR;
        case EBUSY:
            return  OsclProcStatus::MUTEX_LOCKED_ERROR;
        default:
            return OsclProcStatus::OTHER_ERROR;
    }

}

OSCL_EXPORT_REF OsclThreadLock::OsclThreadLock()
{
    iMutex.Create();
}
OSCL_EXPORT_REF OsclThreadLock::~OsclThreadLock()
{
    iMutex.Close();
}
OSCL_EXPORT_REF void OsclThreadLock::Lock()
{
    iMutex.Lock();
}
OSCL_EXPORT_REF void OsclThreadLock::Unlock()
{
    iMutex.Unlock();
}

