


#ifndef PV_OMX_INTERFACE_PROXY_NOTIFIER_H_INCLUDED
#define PV_OMX_INTERFACE_PROXY_NOTIFIER_H_INCLUDED

#ifndef PV_OMXDEFS_H_INCLUDED
#include "pv_omxdefs.h"
#endif

#ifndef PV_OMX_INTERFACE_PROXY_H_INCLUDED
#include "pv_omx_interface_proxy.h"
#endif

#include "oscl_scheduler_ao.h"
#if PROXY_INTERFACE

class CPVInterfaceProxyNotifier_OMX :
        public OsclActiveObject
{
    public:
        CPVInterfaceProxyNotifier_OMX(CPVInterfaceProxy_OMX *aProxy, int32 pri) :
                OsclActiveObject(pri, "proxyNotifier")
                , iProxy(aProxy)
        {}

        virtual ~CPVInterfaceProxyNotifier_OMX()
        {}

    private:
        void Run();

        CPVInterfaceProxy_OMX *iProxy;
};
#endif //PROXY_INTERFACE
#endif //


