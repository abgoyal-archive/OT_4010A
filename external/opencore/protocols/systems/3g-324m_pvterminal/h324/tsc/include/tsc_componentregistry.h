
#ifndef TSC_COMPONENTREGISTRY_H
#define TSC_COMPONENTREGISTRY_H

#define TSC_COMPONENT_MIME_TYPE "x-pvmf/component"

#include "tsc_node_interface.h"
#include "tsc_component.h"


class TSC_statemanager;
class TSC_capability;
class TSC_lc;
class TSC_blc;
class TSC_clc;
class TSC_mt;

class TSCComponentRegistry
{
    public:
        TSCComponentRegistry(TSC_statemanager& aTSCStateManager,
                             TSC_capability& aTSCcapability,
                             TSC_lc& aTSClc,
                             TSC_blc& aTSCblc,
                             TSC_clc& aTSCclc,
                             TSC_mt& aTSCmt);

        void QueryRegistry(Tsc324mNodeCommand& cmd);

        TSC_component* Create(PVMFSessionId aSession,
                              const PVUuid& aUuid,
                              PVInterface*& aInterfacePtr,
                              const OsclAny* aContext);
    private:
        TSC_statemanager& iTSCstatemanager;
        TSC_capability& iTSCcapability;
        TSC_lc& iTSClc;
        TSC_blc& iTSCblc;
        TSC_clc& iTSCclc;
        TSC_mt& iTSCmt;
};

#endif

