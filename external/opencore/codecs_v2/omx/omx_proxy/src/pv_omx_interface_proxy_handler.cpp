
#ifndef PV_OMXDEFS_H_INCLUDED
#include "pv_omxdefs.h"
#endif


#ifndef PV_OMX_INTERFACE_PROXY_H_INCLUDED
#include "pv_omx_interface_proxy.h"
#endif

#ifndef PV_OMX_INTERFACE_PROXY_HANDLER_H_INCLUDED
#include "pv_omx_interface_proxy_handler.h"
#endif

#include <time.h>
#include <sys/time.h>

#define VR_PROFILE_OPT 0

#if PROXY_INTERFACE
void CPVInterfaceProxyHandler_OMX::Run()
{
    if (Status() != OSCL_REQUEST_ERR_NONE)
        return;

    CPVProxyMsg_OMX command(0, 0, 0, NULL);
#if (VR_PROFILE_OPT)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);        
        if(fp_getMsgTime)
        {
            fprintf(fp_getMsgTime, "Run Run %08X %08X %04d %04d %08X %08X %08X\n", t1.tv_sec, t1.tv_usec, gettid(), iProxy->iCommandQueue.size(), command.iMsg, command.iMsgId, command.iCmdId);    
        }
    }
#endif

    //Enter critical section...
    iProxy->iHandlerQueueCrit.Lock();

#if (VR_PROFILE_OPT)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);        
        if(fp_getMsgTime)
        {
            fprintf(fp_getMsgTime, "Lock ok %08X %08X %04d %04d %08X %08X %08X\n", t1.tv_sec, t1.tv_usec, gettid(), iProxy->iCommandQueue.size(), command.iMsg, command.iMsgId, command.iCmdId);    
        }
    }
#endif
    //check command queue...
    if (iProxy->iCommandQueue.size() > 0)
    {
        command = iProxy->iCommandQueue[0];
        iProxy->iCommandQueue.erase(&iProxy->iCommandQueue[0]);
#if (VR_PROFILE_OPT)
        {
            struct timeval t1;
            gettimeofday(&t1, NULL);        
            if(fp_getMsgTime)
            {
                fprintf(fp_getMsgTime, "dequeue command %08X %08X %04d %04d %08X %08X %08X\n", t1.tv_sec, t1.tv_usec, gettid(), iProxy->iCommandQueue.size(), command.iMsg, command.iMsgId, command.iCmdId);    
            }
        }
#endif
    }

    PendForExec();

    //if there's another message queued,
    //go ahead and signal request complete.
    if (iProxy->iCommandQueue.size() > 0)
        PendComplete(OSCL_REQUEST_ERR_NONE);

    //Leave critical section...
    iProxy->iHandlerQueueCrit.Unlock();

#if (VR_PROFILE_OPT)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);        
        if(fp_getMsgTime)
        {
            fprintf(fp_getMsgTime, "ready Process %08X %08X %04d %04d %08X %08X %08X\n", t1.tv_sec, t1.tv_usec, gettid(), iProxy->iCommandQueue.size(), command.iMsg, command.iMsgId, command.iCmdId);    
        }
    }
#endif

    //Process command
    if (command.iMsg)
    {
        //call proxy server message handler
        CPVProxyInterface_OMX *proxy = iProxy->FindInterface(command.iProxyId);
        if (proxy)
            proxy->iServer->HandleCommand(command.iMsgId, command.iCmdId, command.iMsg);
        else
            OSCL_ASSERT(NULL != proxy);//debug error.
    }

#if (VR_PROFILE_OPT)
    {
        struct timeval t1;
        gettimeofday(&t1, NULL);        
        if(fp_getMsgTime)
        {
            fprintf(fp_getMsgTime, "run end %08X %08X %04d %04d %08X %08X %08X\n", t1.tv_sec, t1.tv_usec, gettid(), iProxy->iCommandQueue.size(), command.iMsg, command.iMsgId, command.iCmdId);    
        }
    }
#endif
}

#endif // PROXY_INTERFACE
