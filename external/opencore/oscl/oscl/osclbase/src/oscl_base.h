
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                           O S C L _ B A S E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_BASE_H_INCLUDED
#define OSCL_BASE_H_INCLUDED

#include "osclconfig.h"
#include "oscl_base_macros.h"
#include "oscl_types.h"
#include "osclconfig_check.h"

#ifdef USE_CML2_CONFIG
#include "pv_config.h"
#endif

//singleton support derives from global var support.
#define OSCL_HAS_SINGLETON_SUPPORT 1

#ifdef __cplusplus

class OsclLockBase;

class OsclBase
{
    public:
        /**
         * Initializes OsclBase functionality.
         * OsclBase must be initialized before any OsclBase
         * functionality can be used.
         *
         * Note: The first call to OsclBase::Init will initialize
         *  the thread lock that is used to avoid thread contention
         *  for process scope singleton access.  The last call to
         *  OsclBase::Cleanup will cleanup the thread lock.
         *  Case should be taken to avoid possible thread contention
         *  on the first Init and the last Cleanup call.
         *
         * @return 0 on success
         */
        OSCL_IMPORT_REF static int32 Init();

        /**
         * Cleanup OsclBase functionality
         * OsclBase should be cleaned once OsclBase
         * functions are no longer needed
         * @return 0 on success
         */
        OSCL_IMPORT_REF static int32 Cleanup();
};

enum TPVBaseErrorEnum
{
    EPVErrorBaseNotInstalled = 1
    , EPVErrorBaseAlreadyInstalled = 2
    , EPVErrorBaseOutOfMemory = 3
    , EPVErrorBaseSystemCallFailed = 4
    , EPVErrorBaseTooManyThreads = 5
};

#include "oscl_lock_base.h"

#if (OSCL_HAS_BASIC_LOCK)
class _OsclBasicLock : public OsclLockBase
{
    public:

        /**
         * Class constructor.
         */
        OSCL_IMPORT_REF _OsclBasicLock();

        /**
         * Class destructor
         */
        OSCL_IMPORT_REF ~_OsclBasicLock();

        /**
         * Takes the lock
         *
         */
        OSCL_IMPORT_REF void Lock();

        /**
         * Releases the lock
         *
         */
        OSCL_IMPORT_REF void Unlock();


        /**
        * Set to non-zero on error
        */
        int32 iError;

    private:
        TOsclBasicLockObject    ObjLock;

};
#else
typedef OsclNullLock _OsclBasicLock;
#endif

#else

void PVOsclBase_Init();

void PVOsclBase_Cleanup();

#endif

/*! @} */

#endif  // OSCL_BASE_H_INCLUDED
