
#ifndef PVMF_ASFFFPARSER_FACTORY_H_INCLUDED
#define PVMF_ASFFFPARSER_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

// Forward declaration
class PVMFNodeInterface;

#define KPVMFASFFFParserNodeUuid PVUuid(0x96589b44, 0x912c, 0x4964, 0xb5, 0x4f, 0x39, 0xcb, 0xa7, 0xe5, 0x62, 0xb2)

class PVMFASFFFParserNodeFactory
{
    public:
        /**
         * Creates an instance of a PVMFASFFFParserNode. If the creation fails, this function will leave.
         *
         * @param aPriority The active object priority for the node. Default is standard priority if not specified
         * @returns A pointer to an instance of PVMFASFFFParserNode as PVMFNodeInterface reference or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreatePVMFASFFFParserNode(int32 aPriority = OsclActiveObject::EPriorityNominal);

        /**
         * Deletes an instance of PVMFASFFFParserNode
         * and reclaims all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aNode The PVMFASFFFParserNode instance to be deleted
         * @returns A status code indicating success or failure of deletion
         **/
        OSCL_IMPORT_REF static bool DeletePVMFASFFFParserNode(PVMFNodeInterface* aNode);
};

#endif // PVMF_ASFFFPARSER_FACTORY_H_INCLUDED
