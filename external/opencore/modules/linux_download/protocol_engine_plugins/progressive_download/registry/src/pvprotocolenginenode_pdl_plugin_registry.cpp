
#include "oscl_shared_library.h"
#include "pvprotocolenginenode_pdl_plugin_registry.h"
#include "pvmf_protocol_engine_node_shared_lib_interface.h"
#include "pvmf_protocol_engine_node_registry_interface.h"
#include "pvmf_protocol_engine_node_registry_populator_interface.h"
#include "pvmf_protocol_engine_node_progressive_download_container_factory.h"
#include "pvmf_protocol_engine_node_common.h"


#define PDL_PLUGIN_LIB_NAME ""

#define LIB_NAME_MAX_LENGTH 64

typedef ProtocolContainer*(* LPFN_LIB_CREATE_FUNC)(PVMFProtocolEngineNode *);
typedef bool (* LPFN_LIB_RELEASE_FUNC)(ProtocolContainer *);

// Factory functions
ProtocolContainer* ProtocolEngineNodeProgressiveDownloadContainerLoader::CreateProgressiveDownloadContainer(PVMFProtocolEngineNode* aNode)
{
    OsclSharedLibrary* aSharedLibrary = NULL;
    OSCL_StackString<LIB_NAME_MAX_LENGTH> libname(PDL_PLUGIN_LIB_NAME);

    // Need to load the library for the node
    aSharedLibrary = OSCL_NEW(OsclSharedLibrary, ());

    OsclLibStatus result = aSharedLibrary->LoadLib(libname);
    if (OsclLibSuccess != result) return NULL;
    aSharedLibrary->AddRef();

    // Query for create function
    OsclAny* interfacePtr = NULL;
    aSharedLibrary->QueryInterface(PENODE_SHARED_LIBRARY_INTERFACE, (OsclAny*&)interfacePtr);
    if (NULL == interfacePtr) return NULL;

    ProtocolEngineNodeSharedLibraryInterface* libIntPtr = OSCL_DYNAMIC_CAST(ProtocolEngineNodeSharedLibraryInterface*, interfacePtr);

    OsclAny* createFuncTemp = libIntPtr->QueryLibInterface(PENODE_CREATE_LIB_INTERFACE);
    if (!createFuncTemp) return NULL;

    LPFN_LIB_CREATE_FUNC libCreateFunc = OSCL_DYNAMIC_CAST(ProtocolContainer * (*)(PVMFProtocolEngineNode *), createFuncTemp);

    if (NULL != libCreateFunc)
    {
        // call the real node factory function
        ProtocolContainer *aProtocolContainer = (*(libCreateFunc))(aNode);
        if (NULL != aProtocolContainer)
        {
            aProtocolContainer->SetSharedLibraryPtr(aSharedLibrary);
            return aProtocolContainer;
        }
    }

    aSharedLibrary->RemoveRef();
    if (OsclLibSuccess == aSharedLibrary->Close())
    {
        // Close will unload the library if refcount is 0
        OSCL_DELETE(aSharedLibrary);
    }
    return NULL;
}

bool ProtocolEngineNodeProgressiveDownloadContainerLoader::DeleteProgressiveDownloadContainer(ProtocolContainer* aContainer)
{
    if (NULL == aContainer) return false;

    // Retrieve shared library pointer
    OsclSharedLibrary* aSharedLibrary = aContainer->GetSharedLibraryPtr();

    bool bStatus = false;
    if (NULL != aSharedLibrary)
    {
        // Query for release function
        OsclAny* interfacePtr = NULL;
        aSharedLibrary->QueryInterface(PENODE_SHARED_LIBRARY_INTERFACE, (OsclAny*&)interfacePtr);

        ProtocolEngineNodeSharedLibraryInterface* libIntPtr = OSCL_DYNAMIC_CAST(ProtocolEngineNodeSharedLibraryInterface*, interfacePtr);

        OsclAny* releaseFuncTemp = libIntPtr->QueryLibInterface(PENODE_RELEASE_LIB_INTERFACE);
        if (!releaseFuncTemp) return false;

        LPFN_LIB_RELEASE_FUNC libReleaseFunc = OSCL_DYNAMIC_CAST(bool (*)(ProtocolContainer*), releaseFuncTemp);

        if (NULL != libReleaseFunc)
        {
            bStatus = (*(libReleaseFunc))(aContainer);
        }

        aSharedLibrary->RemoveRef();

        if (OsclLibSuccess == aSharedLibrary->Close())
        {
            // Close will unload the library if refcount is 0
            OSCL_DELETE(aSharedLibrary);
        }
    }

    return bStatus;
}


class ProtocolEngineNodeProgressiveDownloadContainerRegistryPopulatorInterface: public OsclSharedLibraryInterface,
        public PVMFProtocolEngineNodeRegistryPopulatorInterface
{
    public:
        ProtocolEngineNodeProgressiveDownloadContainerRegistryPopulatorInterface() {};

        // From OsclSharedLibraryInterface
        OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PVMF_PROTOCOL_ENGINE_NODE_REGISTRY_POPULATOR_INTERFACE)
            {
                return OSCL_STATIC_CAST(PVMFProtocolEngineNodeRegistryPopulatorInterface*, this);
            }
            return NULL;
        };

        // From PVMFProtocolEngineNodeRegistryPopulatorInterface
        void Register(PVMFProtocolEngineNodeRegistryInterface* aRegistry)
        {
            PVMFProtocolEngineContainerInfo aContainerInfo;
            aContainerInfo.iSourceType = PVMF_MIME_DATA_SOURCE_HTTP_URL;
            aContainerInfo.iProtocolEngineContainerUUID = KPVMFProtocolEngineNodeProgressiveDownloadUuid;
            aContainerInfo.iProtocolEngineContainerCreateFunc = ProtocolEngineNodeProgressiveDownloadContainerLoader::CreateProgressiveDownloadContainer;
            aContainerInfo.iProtocolEngineContainerReleaseFunc = ProtocolEngineNodeProgressiveDownloadContainerLoader::DeleteProgressiveDownloadContainer;
            if (aRegistry) aRegistry->RegisterProtocolEngineContainer(&aContainerInfo);
        }

        void Unregister(PVMFProtocolEngineNodeRegistryInterface* aRegistry)
        {
            OSCL_UNUSED_ARG(aRegistry);
        }
};

extern "C"
{
    OSCL_EXPORT_REF OsclSharedLibraryInterface* PVGetInterface(void)
    {
        return OSCL_NEW(ProtocolEngineNodeProgressiveDownloadContainerRegistryPopulatorInterface, ());
    }

    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

