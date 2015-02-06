
#ifndef PVMP4FFCN_FACTORY_H_INCLUDED
#define PVMP4FFCN_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif
#ifndef PVMP4FFCN_TYPES_H_INCLUDED
#include "pvmp4ffcn_types.h"
#endif

// Forward declaration
class PVMFNodeInterface;

#define KPVMp4FFComposerNodeUuid PVUuid(0x0d270304,0x2c46,0x4fe7,0x92,0xa1,0xa7,0xaa,0x1f,0x95,0xa0,0x7a)

class PVMp4FFComposerNodeFactory
{
    public:
        /**
         * Creates an instance of a PV MPEG4 file format composer. If the creation fails, this function will leave.
         *
         * @param aPriority Priority of the active object of this node
         * @returns A pointer to an author or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreateMp4FFComposer(int32 aPriority = OsclActiveObject::EPriorityNominal);

        /**
         * This function allows the application to delete an instance of a PVMp4FFComposerNode
         * and reclaim all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aComposer The file format composer to be deleted.
         * @returns A status code indicating success or failure.
         **/
        OSCL_IMPORT_REF static bool DeleteMp4FFComposer(PVMFNodeInterface* aComposer);
};

#endif // PVMP4FFCN_FACTORY_H_INCLUDED
