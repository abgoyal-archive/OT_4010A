

#ifndef OSCL_DNS_PARAM_H_INCLUDED
#define OSCL_DNS_PARAM_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_dns_tuneables.h"
#include "oscl_namestring.h"
#include "oscl_dns.h"
#include "oscl_mutex.h"
#include "oscl_semaphore.h"

class OsclDNSRequestAO;
class OsclDNSRequest;

#if(PV_DNS_IS_THREAD)
typedef _OsclBasicAllocator TDNSRequestParamAllocator;
#else
typedef OsclMemAllocator TDNSRequestParamAllocator;
#endif

class DNSRequestParam
{
    public:

        virtual ~DNSRequestParam()
        {
#if(PV_DNS_IS_THREAD)
            iLock.Close();
            iStartup.Close();
#endif
        }

        void RemoveRef();

#if(PV_DNS_SERVER)
        void InThread();
#endif

        virtual void Destroy() = 0;

        TPVDNSFxn iFxn;
        OsclDNSRequest *iDNSRequest;

#if(PV_DNS_IS_THREAD)
        bool iThreadKillFlag;
        OsclMutex iLock;
        OsclSemaphore iStartup;
#endif


    protected:

        DNSRequestParam(TPVDNSFxn aFxn);

        uint32 iRefCount;
};


class GetHostByNameParam: public DNSRequestParam
{
    public:

        //Use this routine to create, use DNSRequestParam::RemoveRef when finished.
        static GetHostByNameParam* Create(const char *name, OsclNetworkAddress* &addr);

        //from DNSRequestParam
        void Destroy();

        ~GetHostByNameParam();

        //request params.
        char *iName;
        OsclNetworkAddress *iAddr;

    private:

        GetHostByNameParam(const char *name, OsclNetworkAddress* &addr);


} ;


#endif

