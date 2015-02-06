

#include "oscl_scheduler_ao.h"
#include "oscl_socket_send.h"
#include "oscl_socket_request.h"
#include "oscl_socket_imp.h"

//////////// Method /////////////////////

OsclSendMethod *OsclSendMethod::NewL(OsclIPSocketI& c)
{
    OsclAny*p = c.Alloc().ALLOCATE(sizeof(OsclSendMethod));
    OsclError::LeaveIfNull(p);
    OsclSendMethod* self = OSCL_PLACEMENT_NEW(p, OsclSendMethod(c));
    OsclError::LeaveIfNull(self);
    OsclError::PushL(self);
    self->ConstructL();
    OsclError::Pop();
    return self;
}

void OsclSendMethod::ConstructL()
{
    OsclAny*p = Alloc().ALLOCATE(sizeof(OsclSendRequest));
    OsclError::LeaveIfNull(p);
    OsclSendRequest* self = OSCL_PLACEMENT_NEW(p, OsclSendRequest(*this));
    OsclError::LeaveIfNull(self);
    OsclError::PushL(self);
    self->ConstructL();
    OsclError::Pop();
    iSocketRequestAO = self;
    OsclSocketMethod::ConstructL(iSocketRequestAO);
}

OsclSendMethod::~OsclSendMethod()
{
    if (SendRequest())
    {
        SendRequest()->~OsclSendRequest();
        Alloc().deallocate(SendRequest());
    }
}

TPVSocketEvent OsclSendMethod::Send(const uint8* &aPtr, uint32 aLen,
                                    int32 aTimeout)
{
    if (!StartMethod(aTimeout))
        return EPVSocketFailure;

    SendRequest()->Send(aPtr, aLen);

    return EPVSocketPending;
}

uint8 *OsclSendMethod::GetSendData(int32 *aLength)
{
    return SendRequest()->GetSendData(aLength);
}

//////////// AO /////////////////////

void OsclSendRequest::Send(const uint8* &aPtr, uint32 aLen)
{
    OsclAny *p = NewRequest(sizeof(SendParam));
    if (!p)
        PendComplete(OsclErrNoMemory);
    else
    {
        iParam = OSCL_PLACEMENT_NEW(p, SendParam(aPtr, aLen, 0));
        if (!iParam)
            PendComplete(OsclErrNoMemory);
        else
            SocketI()->Send(*Param(), *this);
    }
}

void OsclSendRequest::Success()
{
    SocketI()->SendSuccess(*Param());
}

uint8 *OsclSendRequest::GetSendData(int32 *aLength)
{
    if (iParam)
    {
        if (aLength)
            *aLength = Param()->iXferLen;
        return (uint8*)Param()->iBufSend.iPtr;
    }
    else
    {
        if (aLength)
            *aLength = 0;
        return NULL;
    }
}




