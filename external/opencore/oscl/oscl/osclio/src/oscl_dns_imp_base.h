


#ifndef OSCL_DNS_IMP_BASE_H_INCLUDED
#define OSCL_DNS_IMP_BASE_H_INCLUDED

#include "oscl_socket_imp.h"
#include "oscl_dns_request.h"
#include "oscl_dns.h"

class OsclSocketServI;
class GetHostByNameParam;
class OsclDNSRequestAO;

class OsclDNSIBase
{
    public:
        virtual ~OsclDNSIBase();

        //synchronous methods
        virtual int32 Open(OsclSocketServI& aServer) = 0;
        virtual int32 Close() = 0;

        //asynchronous methods.

        virtual void GetHostByName(GetHostByNameParam& , OsclDNSRequestAO&) = 0;
        virtual void GetHostByNameSuccess(GetHostByNameParam&) = 0;

        void CancelFxn(TPVDNSFxn);

    protected:
        OsclDNSIBase(Oscl_DefAlloc &a);

        Oscl_DefAlloc &iAlloc;

        virtual bool IsReady(OsclDNSRequestAO& aObject) = 0;

        virtual void CancelGetHostByName() = 0;

        //link to socket server.
        OsclSocketServI *iSocketServ;

        friend class OsclDNSRequest;
        friend class OsclGetHostByNameRequest;
};

#endif



