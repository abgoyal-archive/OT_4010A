
#include "pvmf_node_shared_lib_interface.h"

#include "pvmf_downloadmanager_factory.h"

#include "osclconfig.h"

class DownloadNodesInterface: public OsclSharedLibraryInterface,
        public NodeSharedLibraryInterface
{
    public:
        DownloadNodesInterface() {};

        // From NodeSharedLibraryInterface
        OsclAny* QueryNodeInterface(const PVUuid& aNodeUuid, const OsclUuid& aInterfaceId)
        {
            if (KPVMFDownloadManagerNodeUuid == aNodeUuid)
            {
                if (PV_CREATE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFDownloadManagerNodeFactory::CreatePVMFDownloadManagerNode));
                }
                else if (PV_RELEASE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFDownloadManagerNodeFactory::DeletePVMFDownloadManagerNode));
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
        return OSCL_NEW(DownloadNodesInterface, ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

