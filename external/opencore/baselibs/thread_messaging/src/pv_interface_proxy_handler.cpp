

#ifndef PV_INTERFACE_PROXY_H_INCLUDED
#include "pv_interface_proxy.h"
#endif

#ifndef PV_INTERFACE_PROXY_HANDLER_H_INCLUDED
#include "pv_interface_proxy_handler.h"
#endif

void CPVInterfaceProxyHandler::Run()
{
    if (Status() != OSCL_REQUEST_ERR_NONE)
        return;

    CPVProxyMsg command(0, 0, NULL);

    //Enter critical section...
    iProxy->iHandlerQueueCrit.Lock();

    //check command queue...
    if (iProxy->iCommandQueue.size() > 0)
    {
        command = iProxy->iCommandQueue[0];
        iProxy->iCommandQueue.erase(&iProxy->iCommandQueue[0]);
    }

    PendForExec();

    //if there's another message queued,
    //go ahead and signal request complete.
    if (iProxy->iCommandQueue.size() > 0)
        PendComplete(OSCL_REQUEST_ERR_NONE);

    //Leave critical section...
    iProxy->iHandlerQueueCrit.Unlock();

    //Process command
    if (command.iMsg)
    {
        //call proxy server message handler
        CPVProxyInterface *proxy = iProxy->FindInterface(command.iProxyId);
        if (proxy)
            proxy->iServer->HandleCommand(command.iMsgId, command.iMsg);
        else
            OSCL_ASSERT(0);//debug error.
    }

}

