
#ifndef PVMF_PROTOCOL_ENGINE_NODE_REGISTRY_POPULATOR_H_INCLUDED
#define PVMF_PROTOCOL_ENGINE_NODE_REGISTRY_POPULATOR_H_INCLUDED

#ifndef OSCL_UUID_H_INCLUDED
#include "oscl_uuid.h"
#endif


#define PVMF_PROTOCOL_ENGINE_NODE_REGISTRY_POPULATOR_INTERFACE OsclUuid(0x50ddafc9,0x78df,0x47cd,0x90,0x38,0xbe,0xa1,0x6a,0x1f,0xf4,0x29)

class PVMFProtocolEngineNodeRegistryInterface;

class PVMFProtocolEngineNodeRegistryPopulatorInterface
{
    public:
        /*
        ** RegisterAllNodes will register one or more nodes in the registry
        ** @param aRegistry: the registry
        ** @param aContext (out): a returned context value.  The registry must
        **    include this value in the UnregisterAllNodes call.
        */
        virtual void Register(PVMFProtocolEngineNodeRegistryInterface* aRegistry) = 0;

        /*
        ** UnregisterAllNodes will unregister one or more nodes in the registry
        ** @param aRegistry: the registry
        ** @param aContext (in): the context value that was returned in the RegisterAllNodes
        **    call.
        */
        virtual void Unregister(PVMFProtocolEngineNodeRegistryInterface* aRegistry) = 0;
};

#endif // PVMF_PROTOCOL_ENGINE_NODE_REGISTRY_POPULATOR_H_INCLUDED


