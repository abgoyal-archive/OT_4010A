





#ifndef OSCL_INIT_H_INCLUDED
#define OSCL_INIT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

//For logging memory leaks, this module relies directly on fprintf to FILE.
#include <stdio.h>

class Oscl_DefAlloc;

class OsclSelect
{
    public:
        OsclSelect()
                : iOsclBase(true)
                , iOsclMemory(true)
                , iOsclErrorTrap(true)
                , iOsclLogger(true)
                , iOsclScheduler(true)
                , iErrAlloc(NULL)
                , iSchedulerAlloc(NULL)
                , iSchedulerName(NULL)
                , iSchedulerReserve(10)
                , iHeapCheck(false)
                , iOutputFile(NULL)
        {}

        //this constructor is mainly for back-compatibility with the old OsclInit argument list.
        OsclSelect(Oscl_DefAlloc* erralloc
                   , Oscl_DefAlloc* schedalloc
                   , const char*name
                   , int32 reserve = 10
                                     , bool heapcheck = false
                                                        , FILE* output = NULL)
                : iOsclBase(true)
                , iOsclMemory(true)
                , iOsclErrorTrap(true)
                , iOsclLogger(true)
                , iOsclScheduler(true)
                , iErrAlloc(erralloc)
                , iSchedulerAlloc(schedalloc)
                , iSchedulerName(name)
                , iSchedulerReserve(reserve)
                , iHeapCheck(heapcheck)
                , iOutputFile(output)
        {}

        bool iOsclBase;         //Init/Cleanup OsclBase?
        bool iOsclMemory;       //Init/Cleanup OsclMemory?
        bool iOsclErrorTrap;    //Init/Cleanup OsclErrorTrap?
        bool iOsclLogger;       //Init/Cleanup PVLogger?
        bool iOsclScheduler;    //Init/Cleanup OsclScheduler?

        Oscl_DefAlloc *iErrAlloc; //Allocator for OsclErrorTrap::Init
        Oscl_DefAlloc *iSchedulerAlloc; //Allocator for OsclScheduler::Init
        const char *iSchedulerName;     //Name for OsclScheduler::Init
        int32 iSchedulerReserve;        //Queue reserve for OsclScheduler::Init

        bool iHeapCheck; //Do Symbian heap checks?

        FILE* iOutputFile; //Output file for memory leak report.
};

class OsclInit
{
    public:
        /**
         * This routine initializes the Oscl modules in the calling thread.
         *
         * @param err: (output) error code of any leave that occurs in
         *   initialization.
         *
         * @param config: (input param) optional set of initialization parameters.
         *   If null, then full initialization with default parameters will be performed.
         *
         */
        OSCL_IMPORT_REF static void Init(
            int32& aError
            , const OsclSelect *aSelect = NULL
        );

        /**
         * This routine cleans up the Oscl modules in the calling thread.
         *
         * @param err: (output) error code of any leave that occurs in
         *   initialization.
         * @param config: (input param) optional set of initialization parameters.
         *   If null, then full initialization with default parameters will be performed.
         *   For proper cleanup, the parameters should match the ones used during
         *   the Init call.
         *
         */
        OSCL_IMPORT_REF static void Cleanup(
            int32& aError
            , const OsclSelect *aSelect = NULL
        );
};


#endif


/*! @} */
