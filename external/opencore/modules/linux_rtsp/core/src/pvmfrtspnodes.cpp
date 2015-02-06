
#include "pvmf_node_shared_lib_interface.h"

#include "pvmfrtspnodereg.h"

#include "pvmf_sm_node_factory.h"

// Need osclconfig.h for the HAS_OSCL_LIB_SUPPORT macro for now
#include "osclconfig.h"

class StreamingNodesInterface: public OsclSharedLibraryInterface,
        public NodeSharedLibraryInterface
{
    public:
        StreamingNodesInterface() {};

        // From NodeSharedLibraryInterface
        OsclAny* QueryNodeInterface(const PVUuid& aNodeUuid, const OsclUuid& aInterfaceId)
        {
            if (KPVMFRTSPStreamingModuleUuid == aNodeUuid)
            {
                if (PV_CREATE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFStreamingManagerNodeFactory::CreateStreamingManagerNode));
                }
                else if (PV_RELEASE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFStreamingManagerNodeFactory::DeleteStreamingManagerNode));
                }
            }

            return NULL;
        };

        // From OsclSharedLibraryInterface
        OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PV_NODE_INTERFACE)
            {
                return OSCL_STATIC_CAST(NodeSharedLibraryInterface*, this);
            }
            return NULL;
        };
};


extern "C"
{
    OSCL_EXPORT_REF OsclSharedLibraryInterface* PVGetInterface(void)
    {
        return OSCL_NEW(StreamingNodesInterface, ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

