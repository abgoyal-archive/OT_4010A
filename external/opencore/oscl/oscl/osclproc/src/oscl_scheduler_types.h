




#ifndef OSCL_SCHEDULER_TYPES_H_INCLUDED
#define OSCL_SCHEDULER_TYPES_H_INCLUDED

#ifndef OSCLCONFIG_PROC_H_INCLUDED
#include "osclconfig_proc.h"
#endif


//Non-Symbian

#ifndef OSCL_AOSTATUS_H_INCLUDED
#include "oscl_aostatus.h"
#endif

#ifndef OSCL_HEAPBASE_H_INCLUDED
#include "oscl_heapbase.h"
#endif


class OsclExecSchedulerBase : public _OsclHeapBase
{
    private:
        virtual void Error(int32 anError) const;
        virtual void OnStarting();
        virtual void OnStopping();
        OsclExecSchedulerBase();
        ~OsclExecSchedulerBase();
        friend class OsclExecScheduler;
        friend class OsclCoeActiveScheduler;
        friend class PVActiveBase;
};




#endif //


/*! @} */
