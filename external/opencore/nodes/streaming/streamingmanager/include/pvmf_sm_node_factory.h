
#ifndef PVMF_SM_NODE_FACTORY_H_INCLUDED
#define PVMF_SM_NODE_FACTORY_H_INCLUDED

#ifndef PVMF_SM_NODE_EVENTS_H_INCLUDED
#include "pvmf_sm_node_events.h"
#endif

/* Forward declaration */
class PVMFNodeInterface;

#define KPVMFStreamingManagerNodeUuid PVUuid(0xd4756fd8,0x7bc0,0x417e,0xb3,0xa4,0x13,0xbd,0xbe,0xa0,0x5d,0x49)


class PVMFStreamingManagerNodeFactory
{
    public:
        /**
         * Creates an instance of a PVMFStreamingManagerNode.
         * If the creation fails, this function will leave.
         *
         * @param priority for the underlying node active object
         *
         * @returns A pointer to an instance of PVMFStreamingManagerNode
         * as PVMFNodeInterface reference or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreateStreamingManagerNode(int32 aPriority);

        /**
         * Deletes an instance of PVMFStreamingManagerNode
         * and reclaims all allocated resources.  An instance can be deleted
         * only in the created state. An attempt to delete in any other state
         * will result in a leave with error code OsclErrInvalidState.
         *
         * @param aNode The PVMFStreamingManagerNode instance to be deleted
         * @returns None
         **/
        OSCL_IMPORT_REF static bool DeleteStreamingManagerNode(PVMFNodeInterface* aNode);
};

const int PVMFFSPNotCreated = -1;

#endif


