
#ifndef PVMF_PROTOCOLENGINE_FACTORY_H_INCLUDED
#define PVMF_PROTOCOLENGINE_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

// Forward declaration
class PVMFNodeInterface;

//PV_DV: this UID needs to reflect WMA DEC
#define KPVMFProtocolEngineNodeUuid PVUuid(0xd6b78c5d,0x2ec2,0x4e9f,0x88,0x12,0xd0,0x92,0x3e,0x81,0x25,0x29);


class PVMFProtocolEngineNodeFactory
{
    public:
        /**
         * Creates an instance of a PVMFProtocolEngineNode. If the creation fails, this function will leave.
         *
         * @param aPriority The active object priority for the node. Default is standard priority if not specified
         * @returns A pointer to an instance of PVMFProtocolEngineNode as PVMFNodeInterface reference or leaves if
         * instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreatePVMFProtocolEngineNode(int32 aPriority = OsclActiveObject::EPriorityNominal);

        /**
         * Deletes an instance of PVMFProtocolEngineNode
         * and reclaims all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aNode The PVMFProtocolEngineNode instance to be deleted
         * @returns A status code indicating success or failure of deletion
         **/
        OSCL_IMPORT_REF static bool DeletePVMFProtocolEngineNode(PVMFNodeInterface* aNode);
};

#endif // PVMF_PROTOCOLENGINE_FACTORY_H_INCLUDED


