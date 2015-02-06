
#include "pvmf_protocol_engine_node.h"
#include "pvmf_protocol_engine_node_progressive_download.h"
#include "pvmf_protocol_engine_node_progressive_download_container_factory.h"


////////////////////////////////////////////////////////////////////////////////////
//////  PVMFProtocolEngineNodeProgressiveDownloadContainerFactory implementation
////////////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF ProtocolContainer* PVMFProtocolEngineNodeProgressiveDownloadContainerFactory::Create(PVMFProtocolEngineNode *aNode)
{
    return OSCL_NEW(ProgressiveDownloadContainer, (aNode));
}

OSCL_EXPORT_REF bool PVMFProtocolEngineNodeProgressiveDownloadContainerFactory::Delete(ProtocolContainer* aContainer)
{
    if (aContainer) OSCL_DELETE(aContainer);
    return true;
}


