

#include "oscl_socket_tuneables.h"
#if (PV_SOCKET_SERVER)
//PV Socket server interface

#include "oscl_scheduler_ao.h"
#include "oscl_socket_request.h"
#include "oscl_assert.h"
#include "oscl_error.h"
#include "oscl_socket_method.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_serv_imp.h"

//Stats macros for use with OsclSocketRequest
#if(PV_OSCL_SOCKET_STATS_LOGGING)
#define ADD_STATS(a,b) iSocketRequestAO->SocketI()->iStats.Add(a,b)
#define LOG_STATS(a) iSocketRequestAO->SocketI()->iStats.Log(a)
#define CLEAR_STATS(a) iSocketRequestAO->SocketI()->iStats.Clear(a)
#else
#define ADD_STATS(a,b)
#define LOG_STATS(a)
#define CLEAR_STATS(a)
#endif

void OsclSocketRequest::CancelRequest()
//cancel a request to the socket server.
{
    if (!iSocketRequestAO
            || !iSocketRequestAO->IsBusy())
        return;//unexpected call.

    //issue request to server
    ADD_STATS(iParam->iFxn, EOsclSocket_ServRequestCancelIssued);
    iSocketI->iSocketServ->iSockServRequestList.StartCancel(this);
}

void OsclSocketRequest::Activate(SocketRequestParam* p, OsclSocketRequestAO &a)
//activate a request to the socket server
{
    //save params
    iSocketRequestAO = &a;
    iParam = p;

    //issue request to server.
    ADD_STATS(iParam->iFxn, EOsclSocket_ServRequestIssued);
    iSocketI->iSocketServ->iSockServRequestList.Add(this);
}


void OsclSocketRequest::Complete(OsclSocketServRequestQElem* aElem, int32 aStatus, int32 aSockErr)
//called by socket server when a request is complete.
{
    ADD_STATS(iParam->iFxn, EOsclSocket_ServRequestComplete);

    //remove this request from the server's active request queue.
    //note this just flags it for deletion instead of actually erasing
    //from the queue, since this is generally called while iterating
    //through the queue.

    //Note: It's important to make sure not to access
    //anything in the socket object after completing the request,
    //since at that point the app could delete the socket object
    //at any time.

    OSCL_ASSERT(iSocketI->iSocketServ->IsServerThread());
    iSocketI->iSocketServ->iSockServRequestList.Remove(aElem);

    iSocketRequestAO->iSocketError = aSockErr;
    iSocketRequestAO->PendComplete(aStatus);
}

#endif //PV_SOCKET_SERVER



