
#ifndef PV_OMXDEFS_H_INCLUDED
#include "pv_omxdefs.h"
#endif


#ifndef PV_OMX_INTERFACE_PROXY_H_INCLUDED
#include "pv_omx_interface_proxy.h"
#endif

#ifndef PV_OMX_INTERFACE_PROXY_NOTIFIER_H_INCLUDED
#include "pv_omx_interface_proxy_notifier.h"
#endif

#if PROXY_INTERFACE
void CPVInterfaceProxyNotifier_OMX::Run()
{
    if (Status() != OSCL_REQUEST_ERR_NONE)
        return;

    CPVProxyMsg_OMX notice(0, 0, 0, NULL);

    //Enter critical section...
    iProxy->iNotifierQueueCrit.Lock();

    //check notice queue...
    if (iProxy->iNotificationQueue.size() > 0)
    {
        notice = iProxy->iNotificationQueue[0];
        iProxy->iNotificationQueue.erase(&iProxy->iNotificationQueue[0]);
    }

    PendForExec();

    //if there's another message queued,
    //go ahead and signal request complete.
    if (iProxy->iNotificationQueue.size() > 0)
        PendComplete(OSCL_REQUEST_ERR_NONE);

    //Leave critical section...
    iProxy->iNotifierQueueCrit.Unlock();

    //Process the notice
    if (notice.iMsg)
    {
        //call proxy client message handler.
        CPVProxyInterface_OMX *proxy = iProxy->FindInterface(notice.iProxyId);
        if (proxy)
            proxy->iClient->HandleNotification(notice.iMsgId, notice.iMsg);
        else
            OSCL_ASSERT(NULL != proxy);//debug error.
    }

}
#endif
