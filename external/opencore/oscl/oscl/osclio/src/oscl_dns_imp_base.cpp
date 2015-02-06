

#include "osclconfig_io.h"
#include "oscl_scheduler_ao.h"
#include "oscl_dns_imp_base.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_error.h"
#include "oscl_assert.h"
#include "oscl_dns_method.h"

//Common section...

OsclDNSIBase::OsclDNSIBase(Oscl_DefAlloc &a) : iAlloc(a)
{
    iSocketServ = NULL;
}

OsclDNSIBase::~OsclDNSIBase()
{
}

void OsclDNSIBase::CancelFxn(TPVDNSFxn aFxn)
{
    switch (aFxn)
    {
        case EPVDNSGetHostByName:
            CancelGetHostByName();
            break;
        default:
            OSCL_ASSERT(false);
            break;
    }
}









