
#ifndef PVMF_PROTOCOL_ENGINE_NODE_PS_PLUGIN_H_REGISTRY_INCLUDED
#define PVMF_PROTOCOL_ENGINE_NODE_PS_PLUGIN_H_REGISTRY_INCLUDED

class ProtocolContainer;
class PVMFProtocolEngineNode;

class ProtocolEngineNodeProgressiveStreamingContainerLoader
{
    public:
        static ProtocolContainer* CreateProgressiveStreamingContainer(PVMFProtocolEngineNode* aNode);
        static bool DeleteProgressiveStreamingContainer(ProtocolContainer* aContainer);
};

#endif // PVMF_PROTOCOL_ENGINE_NODE_PS_PLUGIN_H_REGISTRY_INCLUDED

