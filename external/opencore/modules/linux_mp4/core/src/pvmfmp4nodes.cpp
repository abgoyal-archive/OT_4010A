
#include "pvmf_node_shared_lib_interface.h"

#include "pvmf_mp4ffparser_factory.h"

#include "osclconfig.h"

class Mp4NodesInterface: public OsclSharedLibraryInterface,
        public NodeSharedLibraryInterface
{
    public:
        Mp4NodesInterface() {};

        // From NodeSharedLibraryInterface
        OsclAny* QueryNodeInterface(const PVUuid& aNodeUuid, const OsclUuid& aInterfaceId)
        {
            if (KPVMFMP4FFParserNodeUuid == aNodeUuid)
            {
                if (PV_CREATE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFMP4FFParserNodeFactory::CreatePVMFMP4FFParserNode));
                }
                else if (PV_RELEASE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFMP4FFParserNodeFactory::DeletePVMFMP4FFParserNode));
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
    OSCL_EXPORT_REF OsclSharedLibraryInterface *PVGetInterface(void)
    {
        return OSCL_NEW(Mp4NodesInterface, ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

