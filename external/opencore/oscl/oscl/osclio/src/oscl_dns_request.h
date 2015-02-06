

#ifndef OSCL_DNS_REQUEST_H_INCLUDED
#define OSCL_DNS_REQUEST_H_INCLUDED

#include "oscl_dns_tuneables.h"
#if PV_DNS_SERVER

#include "oscl_namestring.h"
#include "oscl_dns.h"
#include "oscl_socket_types.h"
#include "oscl_dns_tuneables.h"

#if PV_DNS_IS_THREAD
#include "oscl_thread.h"
#include "oscl_mutex.h"
#endif

class DNSRequestParam;
class OsclDNSRequestAO;

class OsclDNSRequest
{
    public:
        OsclDNSRequest()
                : iDNSRequestAO(NULL)
                , iDNSRequestParam(NULL)
                , iActive(false)
        {
        }
        ~OsclDNSRequest()
        {
        }

        OsclDNSRequestAO *iDNSRequestAO;//the AO that is waiting on completion.
        DNSRequestParam *iDNSRequestParam;
        bool iActive;

#if PV_DNS_IS_THREAD
        static TOsclThreadFuncRet OSCL_THREAD_DECL ThreadFunc(TOsclThreadFuncArg arg);
#endif

        void CancelRequest();
        void Complete(bool, int32 aStatus, int32 aSockErr);
        void Activate(DNSRequestParam *iParam, OsclDNSRequestAO &a);
};

#endif


#endif

