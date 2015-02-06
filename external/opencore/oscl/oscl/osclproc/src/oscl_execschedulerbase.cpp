

#include "osclconfig_proc.h"

//The OsclExecSchedulerBase class provides a subset of Symbian
//CActiveScheduler functionality.

#include "oscl_scheduler.h"
#include "oscl_error.h"
#include "oscl_error_imp.h"

OsclExecSchedulerBase::OsclExecSchedulerBase()
//constructor
{
}

OsclExecSchedulerBase::~OsclExecSchedulerBase()
//destructor
{
}

void OsclExecSchedulerBase::Error(int32 anError) const
//virtual method--default error handler.
{
    OSCL_UNUSED_ARG(anError);
    //"The default method raises E32User Cbase 47 panic
    OsclError::Leave(OsclErrNoHandler);//EExecObjectLeave
}

void OsclExecSchedulerBase::OnStarting()
//virtual method
{
    //"The default method does nothing"
}

void OsclExecSchedulerBase::OnStopping()
//virtual method
{
    //"The default method does nothing"
}








