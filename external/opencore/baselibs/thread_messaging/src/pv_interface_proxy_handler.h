


#ifndef PV_INTERFACE_PROXY_HANDLER_H_INCLUDED
#define PV_INTERFACE_PROXY_HANDLER_H_INCLUDED

#ifndef PV_INTERFACE_PROXY_H_INCLUDED
#include "pv_interface_proxy.h"
#endif

#include "oscl_scheduler_ao.h"

class CPVInterfaceProxyHandler :
        public OsclActiveObject
{
    public:
        CPVInterfaceProxyHandler(CPVInterfaceProxy *aProxy, int32 pri):
                OsclActiveObject(pri, "proxyHandler")
                , iProxy(aProxy)
        {}

        virtual ~CPVInterfaceProxyHandler()
        {}

    private:
        void Run();

        CPVInterfaceProxy *iProxy;
};

#endif //


