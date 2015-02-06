 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmfrmffparsernodereg.cpp
 *
 * Project:
 * --------
 *   YUSU
 *
 * Description:
 * ------------
 *   Implement RMFF parser node registry
 *
 * Author:
 * -------
 *   Morris Yang (mtk03147)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime $
 * $Log     $  
 *
 * 01 13 2011 donglei.ji
 * [ALPS00139474] [Need Patch] [Volunteer Patch] Android2.3 opencore migration
 * Android2.3 opencore migration.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef PVMF_NODE_SHARED_LIB_INTERFACE_H_INCLUDED
#include "pvmf_node_shared_lib_interface.h"
#endif

#ifndef PVMF_RMFFPARSER_FACTORY_H_INCLUDED
#include "pvmf_rmffparser_factory.h"
#endif

#ifndef PVRMFFREC_FACTORY_H_INCLUDED
#include "pvrmffrec_factory.h"
#endif

#ifndef PVMF_RECOGNIZER_REGISTRY_H_INCLUDED
#include "pvmf_recognizer_registry.h"
#endif

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#ifndef OSCL_SHARED_LIBRARY_H_INCLUDED
#include "oscl_shared_library.h"
#endif

#ifndef PVMFRMFFPARSERNODEREG_H_INCLUDED
#include "pvmfrmffparsernodereg.h"
#endif

#include <cutils/log.h>

#define RMFFPARSERNODE_LIB_NAME "libopencore_rmlocal.so"

#define NODE_REGISTRY_LIB_NAME_MAX_LENGTH 64

typedef PVMFNodeInterface*(* LPFN_NODE_CREATE_FUNC)(int32);

typedef bool (* LPFN_NODE_RELEASE_FUNC)(PVMFNodeInterface *);

// Factory functions
PVMFNodeInterface* RMFFParserNodesCoreLibraryLoader::CreateRMFFParserNode(int32 aPriority)
{
    LOGE ("[Morris] RMFFParserNodesCoreLibraryLoader::CreateRMFFParserNode get called\n");
    OsclSharedLibrary* rmffSharedLibrary = NULL;
    OSCL_StackString<NODE_REGISTRY_LIB_NAME_MAX_LENGTH> libname(RMFFPARSERNODE_LIB_NAME);

    // Need to load the library for the node
    rmffSharedLibrary = OSCL_NEW(OsclSharedLibrary, (libname));
    OsclLibStatus result = rmffSharedLibrary->LoadLib();
    if (OsclLibSuccess != result)
    {
        return NULL;
    }

    rmffSharedLibrary->AddRef();

    // Query for create function
    OsclAny* interfacePtr = NULL;

    rmffSharedLibrary->QueryInterface(PV_NODE_INTERFACE, (OsclAny*&)interfacePtr);

    NodeSharedLibraryInterface* nodeIntPtr = OSCL_DYNAMIC_CAST(NodeSharedLibraryInterface*, interfacePtr);

    OsclAny* createFuncTemp = nodeIntPtr->QueryNodeInterface(KPVMFRMFFParserNodeUuid, PV_CREATE_NODE_INTERFACE);

    LPFN_NODE_CREATE_FUNC nodeCreateFunc = OSCL_DYNAMIC_CAST(PVMFNodeInterface * (*)(int32), createFuncTemp);

    if (NULL != nodeCreateFunc)
    {
        PVMFNodeInterface* node = NULL;
        // call the real node factory function
        node = (*(nodeCreateFunc))(aPriority);
        if (NULL == node)
        {
            rmffSharedLibrary->RemoveRef();

            if (OsclLibSuccess == rmffSharedLibrary->Close())
            {
                // Close will unload the library if refcount is 0
                OSCL_DELETE(rmffSharedLibrary);
            }

            return NULL;
        }
        node->SetSharedLibraryPtr(rmffSharedLibrary);
        return node;
    }
    return NULL;
}


bool RMFFParserNodesCoreLibraryLoader::DeleteRMFFParserNode(PVMFNodeInterface* aNode)
{
    bool bStatus = false;
    OsclSharedLibrary* rmffSharedLibrary = NULL;

    if (NULL == aNode)
    {
        return false;
    }

    // Retrieve shared library pointer
    rmffSharedLibrary = aNode->GetSharedLibraryPtr();

    if (NULL != rmffSharedLibrary)
    {
        // Query for release function
        OsclAny* interfacePtr = NULL;

        rmffSharedLibrary->QueryInterface(PV_NODE_INTERFACE, (OsclAny*&)interfacePtr);

        NodeSharedLibraryInterface* nodeIntPtr = OSCL_DYNAMIC_CAST(NodeSharedLibraryInterface*, interfacePtr);

        OsclAny* releaseFuncTemp = nodeIntPtr->QueryNodeInterface(KPVMFRMFFParserNodeUuid, PV_RELEASE_NODE_INTERFACE);

        LPFN_NODE_RELEASE_FUNC nodeReleaseFunc = OSCL_DYNAMIC_CAST(bool (*)(PVMFNodeInterface*), releaseFuncTemp);

        if (NULL != nodeReleaseFunc)
        {
            bStatus = (*(nodeReleaseFunc))(aNode);
        }

        rmffSharedLibrary->RemoveRef();

        if (OsclLibSuccess == rmffSharedLibrary->Close())
        {
            // Close will unload the library if refcount is 0
            OSCL_DELETE(rmffSharedLibrary);
        }
    }

    return bStatus;
}

class RMFFParserNodesRegistryInterface: public OsclSharedLibraryInterface,
        public NodeRegistryPopulatorInterface,
        public RecognizerPopulatorInterface
{
    public:
        RMFFParserNodesRegistryInterface() {};

        // From NodeRegistryPopulatorInterface
        void RegisterAllNodes(PVPlayerNodeRegistryInterface* aRegistry, OsclAny*& aContext)
        {
            PVPlayerNodeInfo nodeinfo;

            OSCL_StackString<NODE_REGISTRY_LIB_NAME_MAX_LENGTH> libname = RMFFPARSERNODE_LIB_NAME;

            Oscl_Vector<PVPlayerNodeInfo, OsclMemAllocator>* nodeList = new Oscl_Vector<PVPlayerNodeInfo, OsclMemAllocator>;

            //For PVMFRMFFParserNode
            nodeinfo.iInputTypes.clear();
            nodeinfo.iInputTypes.push_back(PVMF_MIME_RMFF);
            nodeinfo.iNodeUUID = KPVMFRMFFParserNodeUuid;
            nodeinfo.iOutputType.clear();
            nodeinfo.iOutputType.push_back(PVMF_MIME_FORMAT_UNKNOWN);
            nodeinfo.iNodeCreateFunc = (RMFFParserNodesCoreLibraryLoader::CreateRMFFParserNode);
            nodeinfo.iNodeReleaseFunc = (RMFFParserNodesCoreLibraryLoader::DeleteRMFFParserNode);

            aRegistry->RegisterNode(nodeinfo);

            nodeList->push_back(nodeinfo);

            aContext = (OsclAny *)nodeList;

        };

        // From NodeRegistryPopulatorInterface
        void UnregisterAllNodes(PVPlayerNodeRegistryInterface* aRegistry, OsclAny* aContext)
        {
            if (NULL != aContext)
            {
                Oscl_Vector<PVPlayerNodeInfo, OsclMemAllocator>* nodeList = (Oscl_Vector<PVPlayerNodeInfo, OsclMemAllocator> *)aContext;

                while (!nodeList->empty())
                {
                    PVPlayerNodeInfo tmpnode = nodeList->front();
                    aRegistry->UnregisterNode(tmpnode);
                    nodeList->erase(nodeList->begin());
                }

                delete nodeList;
            }
        };


        // From RecognizerPopulatorInterface
        void RegisterAllRecognizers(PVPlayerRecognizerRegistryInterface* aRegistry, OsclAny*& aContext)
        {
            PVMFRecognizerPluginFactory* tmpfac = NULL;

            Oscl_Vector<PVMFRecognizerPluginFactory*, OsclMemAllocator>* pluginList =
                new Oscl_Vector<PVMFRecognizerPluginFactory*, OsclMemAllocator>;

            // morris yang 0415 to to change the RecognizerFactory
            tmpfac =
                OSCL_STATIC_CAST(PVMFRecognizerPluginFactory*, OSCL_NEW(PVRMFFRecognizerFactory, ()));
            aRegistry->RegisterRecognizer(tmpfac);

            pluginList->push_back(tmpfac);

            aContext = (OsclAny *)pluginList;
        };


        void UnregisterAllRecognizers(PVPlayerRecognizerRegistryInterface* aRegistry, OsclAny* aContext)
        {
            if (NULL != aContext)
            {
                Oscl_Vector<PVMFRecognizerPluginFactory*, OsclMemAllocator>* pluginList = (Oscl_Vector<PVMFRecognizerPluginFactory*, OsclMemAllocator>*)aContext;

                while (!pluginList->empty())
                {
                    PVMFRecognizerPluginFactory* tmpfac = pluginList->front();

                    aRegistry->UnregisterRecognizer(tmpfac);

                    pluginList->erase(pluginList->begin());

                    OSCL_DELETE(tmpfac);
                }

                delete pluginList;
            }
        };

        // From OsclSharedLibraryInterface
        OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == PV_NODE_REGISTRY_POPULATOR_INTERFACE)
            {
                return OSCL_STATIC_CAST(NodeRegistryPopulatorInterface*, this);
            }
            else if (aInterfaceId == PV_RECOGNIZER_POPULATOR_INTERFACE)
            {
                return OSCL_STATIC_CAST(RecognizerPopulatorInterface*, this);
            }
            return NULL;
        };
};


extern "C"
{
    OSCL_EXPORT_REF OsclSharedLibraryInterface *PVGetInterface(void)
    {
        return OSCL_NEW(RMFFParserNodesRegistryInterface, ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

