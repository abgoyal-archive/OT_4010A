

#ifndef OSCL_DNS_METHOD_H_INCLUDED
#define OSCL_DNS_METHOD_H_INCLUDED

#include "osclconfig_io.h"
#include "oscl_socket_types.h"
#include "oscl_scheduler_ao.h"
#include "oscl_dns.h"
#include "pvlogger.h"

class OsclDNSObserver;
class OsclDNSRequestAO;
class OsclSocketServ;
class OsclDNSI;
class OsclDNS;

class OsclDNSMethod : public OsclTimerObject
{
    public:
        OsclDNSMethod(Oscl_DefAlloc &a, const char *name, TPVDNSFxn fxn)
                : OsclTimerObject(OsclActiveObject::EPriorityNominal, name)
                , iDNSObserver(NULL)
                , iId(0)
                , iAlloc(a)
                , iDNSFxn(fxn)
                , iDNSRequestAO(NULL)
        {
            iLogger = PVLogger::GetLoggerObject("oscldns");
        }

        void Abort();
        void AbortAll();
        void CancelMethod();
        void Run();

        OsclDNSObserver *iDNSObserver;
        uint32 iId;
        Oscl_DefAlloc &iAlloc;
        TPVDNSFxn iDNSFxn;
        PVLogger* iLogger;

    protected:
        void ConstructL(
            OsclDNSObserver*aObserver,
            OsclDNSRequestAO *aAO,
            uint32 aId);

        bool StartMethod(int32 aTimeoutMsec);
        void MethodDone();

        OsclDNSRequestAO *iDNSRequestAO;
};


class OsclDNSRequestAO : public OsclActiveObject
{
    protected:

        OsclDNSRequestAO(const char *name)
                : OsclActiveObject(OsclActiveObject::EPriorityNominal, name)
                , iDNSI(NULL)
                , iDNSMethod(NULL)
                , iSocketError(0)
        {
        }

        void ConstructL(
            OsclDNSI *aDNS,
            OsclDNSMethod *aMethod)
        {
            if (!aMethod)
                OsclError::Leave(OsclErrGeneral);
            iDNSMethod = aMethod;
            iLogger = iDNSMethod->iLogger;
            if (!aDNS)
                OsclError::Leave(OsclErrGeneral);
            iDNSI = aDNS;
        }

        void Abort()
        {
            Cancel();
            RemoveFromScheduler();
        }
        void NewRequest();
        void RequestDone();
        int GetSocketError();
        OsclSocketServI *Serv();
        void DoCancel();
        void Run();

        virtual void Success() {}

        OsclDNSI *iDNSI;
        OsclDNSMethod *iDNSMethod;
        int32 iSocketError;
        PVLogger* iLogger;

        friend class OsclDNSI;
        friend class OsclDNSMethod;
        friend class OsclDNSRequest;
        friend class DNSRequestParam;
};

#endif

