
#include "tsc_componentregistry.h"

TSCComponentRegistry::TSCComponentRegistry(TSC_statemanager& aTSCStateManager,
        TSC_capability& aTSCcapability,
        TSC_lc& aTSClc,
        TSC_blc& aTSCblc,
        TSC_clc& aTSCclc,
        TSC_mt& aTSCmt) :
        iTSCstatemanager(aTSCStateManager),
        iTSCcapability(aTSCcapability),
        iTSClc(aTSClc),
        iTSCblc(aTSCblc),
        iTSCclc(aTSCclc),
        iTSCmt(aTSCmt)
{
}

void TSCComponentRegistry::QueryRegistry(Tsc324mNodeCommand& aCmd)
{

    OSCL_String* mimetype;
    Oscl_Vector<PVUuid, OsclMemAllocator>* uuidvec;
    bool exactMatch;
    aCmd.Parse(mimetype, uuidvec, exactMatch);

//    uuidvec->clear();
    if (*mimetype == (TSC_COMPONENT_MIME_TYPE))
    {
        uuidvec->push_back((PVUuidH324ComponentInterface));
    }
}

TSC_component* TSCComponentRegistry::Create(PVMFSessionId aSession,
        const PVUuid& aUuid,
        PVInterface*& aInterfacePtr,
        const OsclAny* aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    TSC_component* component = NULL;
    if (aUuid == PVUuidH324ComponentInterface)
    {
        component = OSCL_NEW(TSC_component,
                             (iTSCstatemanager, iTSCcapability, iTSClc, iTSCblc, iTSCclc, iTSCmt));
        aInterfacePtr = (PVMFComponentInterface*)component;
    }
    return component;
}


