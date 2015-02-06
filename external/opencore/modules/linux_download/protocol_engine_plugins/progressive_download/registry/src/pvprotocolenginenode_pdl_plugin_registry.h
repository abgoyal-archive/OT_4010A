
#ifndef PVMF_PROTOCOL_ENGINE_NODE_PDL_PLUGIN_H_REGISTRY_INCLUDED
#define PVMF_PROTOCOL_ENGINE_NODE_PDL_PLUGIN_H_REGISTRY_INCLUDED

class ProtocolContainer;
class PVMFProtocolEngineNode;

class ProtocolEngineNodeProgressiveDownloadContainerLoader
{
    public:
        static ProtocolContainer* CreateProgressiveDownloadContainer(PVMFProtocolEngineNode* aNode);
        static bool DeleteProgressiveDownloadContainer(ProtocolContainer* aContainer);
};

#endif // PVMF_PROTOCOL_ENGINE_NODE_PDL_PLUGIN_H_REGISTRY_INCLUDED

