


#ifndef OSCL_DNS_IMP_PV_H_INCLUDED
#define OSCL_DNS_IMP_PV_H_INCLUDED

#include "oscl_socket_imp_base.h"
#include "oscl_dns_request.h"
#include "oscl_dns_imp_base.h"

class OsclSocketServI;

class OsclDNSI: public OsclDNSIBase
{
    public:
        static OsclDNSI* NewL(Oscl_DefAlloc &a);
        ~OsclDNSI();

        //synchronous methods
        int32 Open(OsclSocketServI& aServer);
        int32 Close();

        //asynchronous methods.

        void GetHostByName(GetHostByNameParam& , OsclDNSRequestAO&);
        void GetHostByNameSuccess(GetHostByNameParam&);


    private:
        OsclDNSI(Oscl_DefAlloc &a);

        bool IsReady(OsclDNSRequestAO& aObject);

        void CancelGetHostByName();

        friend class OsclDNSRequest;

        //active requests
        OsclDNSRequest iGetHostByNameRequest;
        static void ProcessDnsRequest(GetHostByNameParam*, int32&, int32&);

        friend class DNSRequestParam;
};

#endif



