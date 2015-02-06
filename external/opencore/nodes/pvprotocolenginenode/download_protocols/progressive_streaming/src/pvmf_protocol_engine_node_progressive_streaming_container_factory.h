
#ifndef PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_STREAMING_FACTORY_H_INCLUDED
#define PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_STREAMING_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

// Forward declaration
class ProtocolContainer;
class PVMFProtocolEngineNode;

// UUID for the factory
#define KPVMFProtocolEngineNodeProgressiveStreamingUuid PVUuid(0xb6711029,0x9397,0x428b,0xaf,0x75,0x1a,0x1e,0x65,0x1d,0x79,0xa6);

class PVMFProtocolEngineNodeProgressiveStreamingContainerFactory
{
    public:
        OSCL_IMPORT_REF static ProtocolContainer* Create(PVMFProtocolEngineNode *aNode);
        OSCL_IMPORT_REF static bool Delete(ProtocolContainer* aContainer);
};

#endif // PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_STREAMING_FACTORY_H_INCLUDED


