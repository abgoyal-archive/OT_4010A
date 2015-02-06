
#include "pvmf_protocol_engine_node.h"
#include "pvmf_protocol_engine_node_progressive_streaming.h"
#include "pvmf_protocol_engine_node_progressive_streaming_container_factory.h"


////////////////////////////////////////////////////////////////////////////////////
//////  PVMFProtocolEngineNodeProgressiveDownloadContainerFactory implementation
////////////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF ProtocolContainer* PVMFProtocolEngineNodeProgressiveStreamingContainerFactory::Create(PVMFProtocolEngineNode *aNode)
{
    return OSCL_NEW(ProgressiveStreamingContainer, (aNode));
}

OSCL_EXPORT_REF bool PVMFProtocolEngineNodeProgressiveStreamingContainerFactory::Delete(ProtocolContainer* aContainer)
{
    if (aContainer) OSCL_DELETE(aContainer);
    return true;
}


