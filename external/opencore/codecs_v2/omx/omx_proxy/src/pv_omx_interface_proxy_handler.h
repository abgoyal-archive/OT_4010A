


#ifndef PV_OMX_INTERFACE_PROXY_HANDLER_H_INCLUDED
#define PV_OMX_INTERFACE_PROXY_HANDLER_H_INCLUDED

#ifndef PV_OMXDEFS_H_INCLUDED
#include "pv_omxdefs.h"
#endif

#ifndef PV_OMX_INTERFACE_PROXY_H_INCLUDED
#include "pv_omx_interface_proxy.h"
#endif

#ifndef PV_INTERFACE_CMD_MESSAGE_H_INCLUDED
#include "pv_interface_cmd_message.h"
#endif

#include "oscl_scheduler_ao.h"

#define VR_PROFILE_OPT 0

#if PROXY_INTERFACE

class CPVInterfaceProxyHandler_OMX :
        public OsclActiveObject
{
    public:
        FILE *fp_getMsgTime;
        CPVInterfaceProxyHandler_OMX(CPVInterfaceProxy_OMX *aProxy, int32 pri):
                OsclActiveObject(pri, "proxyHandler")
                , iProxy(aProxy)
                {        
#if (VR_PROFILE_OPT)
                    fp_getMsgTime = NULL;
                    fp_getMsgTime=fopen("/getMsgTime.txt", "ab");
#endif
                }

        virtual ~CPVInterfaceProxyHandler_OMX()
        {
#if (VR_PROFILE_OPT)
            if(fp_getMsgTime)
                fclose(fp_getMsgTime);
            fp_getMsgTime = NULL;
#endif
        }

    private:
        void Run();

        CPVInterfaceProxy_OMX *iProxy;
};

#endif // PROXY_INTERFACE
#endif //


