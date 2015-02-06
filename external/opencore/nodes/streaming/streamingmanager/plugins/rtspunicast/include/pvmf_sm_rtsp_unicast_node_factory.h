
#ifndef PVMF_SM_RTSP_UNICAST_NODE_FACTORY_H_INCLUDED
#define PVMF_SM_RTSP_UNICAST_NODE_FACTORY_H_INCLUDED

#ifndef PVMF_EVENT_HANDLING_H_INCLUDED
#include "pvmf_event_handling.h"
#endif

/* Forward declaration */
class PVMFSMFSPBaseNode;

#define KPVMFSMRTSPUnicastNodeUuid PVUuid(0x45ac6dcd,0x2ccf,0x4b0a,0x9c,0xc5,0x8a,0xf7,0x06,0x54,0xde,0x48)


class PVMFSMRTSPUnicastNodeFactory
{
    public:
        /**
         * Creates an instance of a PVMFSMRTSPUnicastNode.
         * If the creation fails, this function will leave.
         *
         * @param priority for the underlying node active object
         *
         * @returns A pointer to an instance of PVMFSMRTSPUnicastNode
         * as PVMFSMFSPBaseNode reference or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFSMFSPBaseNode* CreateSMRTSPUnicastNodeFactory(int32 aPriority);

        /**
         * Deletes an instance of PVMFSMRTSPUnicastNode
         * and reclaims all allocated resources.  An instance can be deleted
         * only in the created state. An attempt to delete in any other state
         * will result returning false
         *
         * @param aNode The PVMFSMFSPBaseNode instance to be deleted
         * @returns bool
         *  true: node was in created state
         *  false: either node was not in created state or deletion failed
         **/
        OSCL_IMPORT_REF static bool DeleteSMRTSPUnicastNodeFactory(PVMFSMFSPBaseNode* aNode);
};

#endif


