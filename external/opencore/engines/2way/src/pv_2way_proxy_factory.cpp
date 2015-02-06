
#include "pv_2way_proxy_factory.h"
#include "pv_2way_engine_factory.h"
#include "pv_2way_proxy_adapter.h"
#include "pv_2way_engine.h"

OSCL_EXPORT_REF void CPV2WayProxyFactory::Init()
{
    CPV2WayEngineFactory::Init();
}

OSCL_EXPORT_REF void CPV2WayProxyFactory::Cleanup()
{
    CPV2WayEngineFactory::Cleanup();
}

OSCL_EXPORT_REF CPV2WayInterface *CPV2WayProxyFactory::CreateTerminal(TPVTerminalType aTerminalType,
        PVCommandStatusObserver* aCmdStatusObserver,
        PVInformationalEventObserver *aInfoEventObserver,
        PVErrorEventObserver *aErrorEventObserver)
{
    if (aTerminalType == PV_324M)
    {
        return CPV2WayProxyAdapter::New(aTerminalType,
                                        aCmdStatusObserver,
                                        aInfoEventObserver,
                                        aErrorEventObserver);
    }
    else
    {
        return NULL;
    }
}

OSCL_EXPORT_REF void CPV2WayProxyFactory::DeleteTerminal(CPV2WayInterface* terminal)
{
    OSCL_DELETE((CPV2WayProxyAdapter*)terminal);
}
