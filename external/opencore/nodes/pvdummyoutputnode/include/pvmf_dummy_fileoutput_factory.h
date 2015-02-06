
#ifndef PVMF_DUMMY_FILEOUTPUT_FACTORY_H_INCLUDED
#define PVMF_DUMMY_FILEOUTPUT_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

// Forward declaration
class PVMFNodeInterface;

//Port tags for this node.
typedef enum
{
    PVMF_DUMMY_FILE_OUTPUT_NODE_PORT_TYPE_SINK
} PVMFDummyFileOutputNodePortTag;

#define KPVFileOutputNodeUuid PVUuid(0xa9fb43bb,0xfed4,0x4914,0xa4,0xbf,0x7d,0x04,0x3f,0x45,0x0c,0x24)

class PVMFDummyFileOutputNodeFactory
{
    public:
        /**
         * Creates an instance of a FileOutput. If the creation fails, this function will leave.
         *
         * @param aPriority The active object priority for the node. Default is standard priority if not specified
         * @returns A pointer to an author or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreateDummyFileOutput(int32 aPriority = OsclActiveObject::EPriorityNominal);

        /**
         * Creates an instance of a FileOutput. If the creation fails, this function will leave.
         *
         * @param aFileName pathname of file to write to.
         * @param aFormat media format for file.
         * @param aPriority The active object priority for the node. Default is standard priority if not specified
         * @returns A pointer to an author or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVMFNodeInterface* CreateDummyFileOutput(OSCL_wString &aFileName, PVMFFormatType aFormat, int32 aPriority = OsclActiveObject::EPriorityNominal);

        /**
         * This function allows the application to delete an instance of a PVMFDummyFileOutputNode
         * and reclaim all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aComposer The file format composer to be deleted.
         * @returns A status code indicating success or failure.
         **/
        OSCL_IMPORT_REF static bool DeleteDummyFileOutput(PVMFNodeInterface*& aNode);
};

#endif // PVMF_FILEOUTPUT_FACTORY_H_INCLUDED
