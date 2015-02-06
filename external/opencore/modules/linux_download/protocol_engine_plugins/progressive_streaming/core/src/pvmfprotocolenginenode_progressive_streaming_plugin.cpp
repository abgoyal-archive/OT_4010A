
#include "oscl_shared_library.h"
#include "pvmf_protocol_engine_node_shared_lib_interface.h"
#include "pvmf_protocol_engine_node_progressive_streaming_container_factory.h"


class ProgressiveStreamingPluginInterface: public OsclSharedLibraryInterface,
        public ProtocolEngineNodeSharedLibraryInterface
{
    public:
        ProgressiveStreamingPluginInterface() {};

        // From ProtocolEngineNodeSharedLibraryInterface
        OsclAny* QueryLibInterface(const OsclUuid& aInterfaceId)
        {
            if (PENODE_CREATE_LIB_INTERFACE == aInterfaceId)
            {
                return (OsclAny*)(PVMFProtocolEngineNodeProgressiveStreamingContainerFactory::Create);
            }
            else if (PENODE_RELEASE_LIB_INTERFACE == aInterfaceId)
            {
                return (OsclAny*)(PVMFProtocolEngineNodeProgressiveStreamingContainerFactory::Delete);
            }

            return NULL;
        };

        // From OsclSharedLibraryInterface
        OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PENODE_SHARED_LIBRARY_INTERFACE)
            {
                return OSCL_STATIC_CAST(ProtocolEngineNodeSharedLibraryInterface*, this);
            }
            return NULL;
        };
};


extern "C"
{
    OSCL_EXPORT_REF OsclSharedLibraryInterface* PVGetInterface(void)
    {
        return OSCL_NEW(ProgressiveStreamingPluginInterface , ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}




