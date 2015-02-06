 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmfrmffparsernodes.cpp
 *
 * Project:
 * --------
 *   YUSU
 *
 * Description:
 * ------------
 *   Implement RMFF parser node core library
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
#include "pvmf_node_shared_lib_interface.h"

#include "pvmf_rmffparser_factory.h"

#include "osclconfig.h"

class RMFFParserNodesInterface: public OsclSharedLibraryInterface,
        public NodeSharedLibraryInterface
{
    public:
        RMFFParserNodesInterface() {};

        // From NodeSharedLibraryInterface
        OsclAny* QueryNodeInterface(const PVUuid& aNodeUuid, const OsclUuid& aInterfaceId)
        {
            if (KPVMFRMFFParserNodeUuid == aNodeUuid)
            {
                if (PV_CREATE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFRMFFParserNodeFactory::CreatePVMFRMFFParserNode));
                }
                else if (PV_RELEASE_NODE_INTERFACE == aInterfaceId)
                {
                    return ((OsclAny*)(PVMFRMFFParserNodeFactory::DeletePVMFRMFFParserNode));
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
        return OSCL_NEW(RMFFParserNodesInterface, ());
    }
    OSCL_EXPORT_REF void PVReleaseInterface(OsclSharedLibraryInterface* aInstance)
    {
        OSCL_DELETE(aInstance);
    }
}

