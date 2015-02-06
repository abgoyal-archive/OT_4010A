
#ifndef PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_DOWNLOAD_FACTORY_H_INCLUDED
#define PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_DOWNLOAD_FACTORY_H_INCLUDED

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
#define KPVMFProtocolEngineNodeProgressiveDownloadUuid PVUuid(0x94fffdf2,0x0130,0x431e,0x86,0xd7,0x11,0x60,0x28,0x22,0x19,0x00);

class PVMFProtocolEngineNodeProgressiveDownloadContainerFactory
{
    public:
        OSCL_IMPORT_REF static ProtocolContainer* Create(PVMFProtocolEngineNode *aNode);
        OSCL_IMPORT_REF static bool Delete(ProtocolContainer* aContainer);
};

#endif // PVMF_PROTOCOLENGINE_NODE_PROGRESSIVE_DOWNLOAD_FACTORY_H_INCLUDED


