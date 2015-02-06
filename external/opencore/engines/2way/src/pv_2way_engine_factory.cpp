
#include "oscl_base.h"
#include "oscl_scheduler.h"
#include "pv_2way_engine_factory.h"
#include "pv_2way_engine.h"

OSCL_EXPORT_REF void CPV2WayEngineFactory::Init()
{
    // initialize Oscl and PVlogger ASAP in each thread
    OsclBase::Init();
    OsclMem::Init();
    PVLogger::Init();
}

OSCL_EXPORT_REF void CPV2WayEngineFactory::Cleanup()
{
    PVLogger::Cleanup();
    OsclMem::Cleanup();
    OsclBase::Cleanup();
}

OSCL_EXPORT_REF CPV2WayInterface *CPV2WayEngineFactory::CreateTerminal(PV2WayTerminalType aTerminalType,
        PVCommandStatusObserver* aCmdStatusObserver,
        PVInformationalEventObserver *aInfoEventObserver,
        PVErrorEventObserver *aErrorEventObserver)
{

    if (aTerminalType == PV_324M ||
            aTerminalType == PV_SIP)
    {
        return CPV324m2Way::NewL(NULL,
                                 aTerminalType,
                                 aCmdStatusObserver,
                                 aInfoEventObserver,
                                 aErrorEventObserver);
    }
    else
    {
        OSCL_LEAVE(PVMFErrNotSupported);
    }
    return NULL;
}

OSCL_EXPORT_REF void CPV2WayEngineFactory::DeleteTerminal(CPV2WayInterface* terminal)
{
    CPV324m2Way::Delete((CPV324m2Way*) terminal);
}
