

#include "oscl_scheduler_ao.h"
#include "oscl_socket_method.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_shutdown.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_shutdown.h"

//////////// Method /////////////////////

OsclShutdownMethod *OsclShutdownMethod::NewL(OsclIPSocketI& c)
{
    OsclAny*p = c.Alloc().ALLOCATE(sizeof(OsclShutdownMethod));
    OsclError::LeaveIfNull(p);
    OsclShutdownMethod* self = OSCL_PLACEMENT_NEW(p, OsclShutdownMethod(c));
    OsclError::LeaveIfNull(self);
    OsclError::PushL(self);
    self->ConstructL();
    OsclError::Pop();
    return self;
}

void OsclShutdownMethod::ConstructL()
{
    OsclAny*p = Alloc().ALLOCATE(sizeof(OsclShutdownRequest));
    OsclError::LeaveIfNull(p);
    OsclShutdownRequest* self = OSCL_PLACEMENT_NEW(p, OsclShutdownRequest(*this));
    OsclError::LeaveIfNull(self);
    OsclError::PushL(self);
    self->ConstructL();
    OsclError::Pop();
    iSocketRequestAO = self;
    OsclSocketMethod::ConstructL(iSocketRequestAO);
}

OsclShutdownMethod::~OsclShutdownMethod()
{
    if (ShutdownRequest())
    {
        ShutdownRequest()->~OsclShutdownRequest();
        Alloc().deallocate(ShutdownRequest());
    }
}

TPVSocketEvent OsclShutdownMethod::Shutdown(TPVSocketShutdown aHow,
        int32 aTimeout)
{
    if (!StartMethod(aTimeout))
        return EPVSocketFailure;

    ShutdownRequest()->Shutdown(aHow);

    return EPVSocketPending;
}

//////////// AO /////////////////////

void OsclShutdownRequest::Shutdown(TPVSocketShutdown aHow)
{
    OsclAny *p = NewRequest(sizeof(ShutdownParam));
    if (!p)
        PendComplete(OsclErrNoMemory);
    else
    {
        iParam = OSCL_PLACEMENT_NEW(p, ShutdownParam(aHow));
        if (!iParam)
            PendComplete(OsclErrNoMemory);
        else
            SocketI()->Shutdown(*Param(), *this);
    }
}




