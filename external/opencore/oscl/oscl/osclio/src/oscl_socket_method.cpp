

#include "osclconfig_io.h"
#include "oscl_scheduler_ao.h"
#include "oscl_socket_method.h"
#include "oscl_socket_imp.h"
#include "oscl_assert.h"


static const char* const TPVSocketFxnStr[] =
{
    "EPVSocketSend"
    , "EPVSocketSendTo"
    , "EPVSocketRecv"
    , "EPVSocketRecvFrom"
    , "EPVSocketConnect"
    , "EPVSocketAccept"
    , "EPVSocketShutdown"
    , "EPVSocketBindUdp"
    , "EPVSocketBindTcp"
    , "EPVSocketListen"
} ;
static const char* const TPVSocketEventStr[] =
{
    "EPVSocketSuccess"
    , "EPVSocketPending"
    , "EPVSocketTimeout"
    , "EPVSocketFailure"
    , "EPVSocketCancel"
};


inline int OsclSocketRequestAO::GetSocketError()
{
#if  (PV_SOCKET_SERVER)

    //Errors from PV socket server are in
    //iSocketError.
    return iSocketError;

#else

#error No Implementation!

#endif
}

//Stats macros for use with OsclSocketRequestAO
#if(PV_OSCL_SOCKET_STATS_LOGGING)
#define ADD_STATS(a,b) SocketI()->iStats.Add(a,b)
#define LOG_STATS(a) SocketI()->iStats.Log(a)
#define CLEAR_STATS(a) SocketI()->iStats.Clear(a)
#else
#define ADD_STATS(a,b)
#define LOG_STATS(a)
#define CLEAR_STATS(a)
#endif

#define LOGINFOMED(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG,iContainer.iContainer.iLogger,PVLOGMSG_DEBUG,m);

void OsclSocketRequestAO::Run()
{
    //The server has completed the socket request.

    RequestDone();


    // Check the request completion status
    switch (Status())
    {
        case OSCL_REQUEST_ERR_NONE:
            ADD_STATS(iContainer.iSocketFxn, EOsclSocket_RequestAO_Success);
            Success();
            LOGINFOMED((0, "OsclSocket(0x%x): %s %s ", (uint32)SocketI(), TPVSocketFxnStr[iContainer.iSocketFxn], TPVSocketEventStr[EPVSocketSuccess]));
            LOG_STATS(iContainer.iSocketFxn);
            CLEAR_STATS(iContainer.iSocketFxn);
            SocketObserver()->HandleSocketEvent(Id(), iContainer.iSocketFxn, EPVSocketSuccess, 0);
            break;

        case OSCL_REQUEST_ERR_CANCEL:
            ADD_STATS(iContainer.iSocketFxn, EOsclSocket_RequestAO_Canceled);
            //Request was cancelled, either due to an API call or due to the
            //socket server shutting down before completing the operation.
            LOGINFOMED((0, "OsclSocket(0x%x): %s %s ", (uint32)SocketI(), TPVSocketFxnStr[iContainer.iSocketFxn], TPVSocketEventStr[EPVSocketCancel]));
            LOG_STATS(iContainer.iSocketFxn);
            CLEAR_STATS(iContainer.iSocketFxn);
            SocketObserver()->HandleSocketEvent(Id(), iContainer.iSocketFxn, EPVSocketCancel, 0);
            break;

        default:
            ADD_STATS(iContainer.iSocketFxn, EOsclSocket_RequestAO_Error);
            //Some error.
            LOGINFOMED((0, "OsclSocket(0x%x): %s %s %d", (uint32)SocketI(), TPVSocketFxnStr[iContainer.iSocketFxn], TPVSocketEventStr[EPVSocketFailure], GetSocketError()));
            LOG_STATS(iContainer.iSocketFxn);
            CLEAR_STATS(iContainer.iSocketFxn);
            SocketObserver()->HandleSocketEvent(Id(), iContainer.iSocketFxn, EPVSocketFailure, GetSocketError());
            break;
    }
}

OsclAny* OsclSocketRequestAO::NewRequest(const uint32 size)
{
    //Activate the AO.  The socket server will complete the request.
    PendForExec();

    bool reallocate = (!iParam || size != iParamSize);

    //Cleanup any previous parameters.
    CleanupParam(reallocate);

    LOGINFOMED((0, "OsclSocket(0x%x): New Request %s", (uint32)SocketI(), TPVSocketFxnStr[iContainer.iSocketFxn]));

    //Allocate space for new parameters, or recycle current space.
    if (reallocate)
    {
        iParamSize = size;
        return Alloc().ALLOCATE(size);
    }
    else
        return iParam;
}

void OsclSocketRequestAO::CleanupParam(bool aDeallocate)
{
    //cleanup the socket request parameters-- can't use
    //virtual destructor because an allocator was used and
    //destructor is called explicitly.
    if (iParam)
    {
        switch (iContainer.iSocketFxn)
        {
            case EPVSocketRecvFrom:
                ((RecvFromParam*)iParam)->~RecvFromParam();
                break;
            case EPVSocketRecv:
                ((RecvParam*)iParam)->~RecvParam();
                break;
            case EPVSocketSendTo:
                ((SendToParam*)iParam)->~SendToParam();
                break;
            case EPVSocketSend:
                ((SendParam*)iParam)->~SendParam();
                break;
            case EPVSocketAccept:
                ((AcceptParam*)iParam)->~AcceptParam();
                break;
            case EPVSocketConnect:
                ((ConnectParam*)iParam)->~ConnectParam();
                break;
            case EPVSocketShutdown:
                ((ShutdownParam*)iParam)->~ShutdownParam();
                break;
            case EPVSocketBind:
                ((BindParam*)iParam)->~BindParam();
                break;
            case EPVSocketListen:
                ((ListenParam*)iParam)->~ListenParam();
                break;
            default:
                OSCL_ASSERT(false);
                break;
        }
        //free memory if needed.
        if (aDeallocate)
        {
            Alloc().deallocate(iParam);
            iParam = NULL;
            iParamSize = 0;
        }
    }
}



bool OsclSocketMethod::StartMethod(int32 aTimeoutMsec)
{
    //make sure there's not already a request
    //in progress
    if (iSocketRequestAO->IsBusy()
            || IsBusy())
        return false;

    if (!iSocketRequestAO->IsAdded())
        iSocketRequestAO->AddToScheduler();

    //set the timeout if any.  Timeout <= 0 indicates infinite wait.
    if (aTimeoutMsec > 0)
    {
        if (!IsAdded())
            AddToScheduler();
        After(aTimeoutMsec*MSEC_TO_MICROSEC);
    }

    iSocketRequestAO->iSocketError = 0;

    return true;
}

//Stats macros for use with OsclSocketMethod
#undef ADD_STATS
#undef LOG_STATS
#undef CLEAR_STATS
#if(PV_OSCL_SOCKET_STATS_LOGGING)
#define ADD_STATS(a,b) iSocketRequestAO->SocketI()->iStats.Add(a,b)
#define LOG_STATS(a) iSocketRequestAO->SocketI()->iStats.Log(a)
#define CLEAR_STATS(a) iSocketRequestAO->SocketI()->iStats.Clear(a)
#else
#define ADD_STATS(a,b)
#define LOG_STATS(a)
#define CLEAR_STATS(a)
#endif

void OsclSocketMethod::Run()
{
    ADD_STATS(iSocketFxn, EOsclSocket_RequestAO_Timeout);
    //The socket request has timed out.
    MethodDone();
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iContainer.iLogger, PVLOGMSG_DEBUG
                    , (0, "OsclSocket: %s %s ", TPVSocketFxnStr[iSocketFxn], TPVSocketEventStr[EPVSocketTimeout]));
    LOG_STATS(iSocketFxn);
    CLEAR_STATS(iSocketFxn);
    iContainer.iObserver->HandleSocketEvent(iContainer.iId, iSocketFxn, EPVSocketTimeout, 0);
}


