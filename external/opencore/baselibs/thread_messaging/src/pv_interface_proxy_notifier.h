


#ifndef PV_INTERFACE_PROXY_NOTIFIER_H_INCLUDED
#define PV_INTERFACE_PROXY_NOTIFIER_H_INCLUDED


#ifndef PV_INTERFACE_PROXY_H_INCLUDED
#include "pv_interface_proxy.h"
#endif

#include "oscl_scheduler_ao.h"

class CPVInterfaceProxyNotifier :
        public OsclActiveObject
{
    public:
        CPVInterfaceProxyNotifier(CPVInterfaceProxy *aProxy, int32 pri) :
                OsclActiveObject(pri, "proxyNotifier")
                , iProxy(aProxy)
        {}

        virtual ~CPVInterfaceProxyNotifier()
        {}

    private:
        void Run();

        CPVInterfaceProxy *iProxy;
};

#endif //


