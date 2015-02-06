

#include "oscl_base.h"

#include "pvmf_fileoutput_node.h"

#include "pvmf_fileoutput_factory.h"

#include "pvmf_fileoutput_inport.h"

#include "oscl_error_codes.h"

#include "oscl_exception.h"

#include "oscl_dll.h"

// Define entry point for this DLL
OSCL_DLL_ENTRY_POINT_DEFAULT()

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFNodeInterface* PVFileOutputNodeFactory::CreateFileOutput(int32 aPriority)
{
    PVMFNodeInterface* node = NULL;
    node = OSCL_NEW(PVMFFileOutputNode, (aPriority));
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFNodeInterface* PVFileOutputNodeFactory::CreateFileOutput(OSCL_wString &aFileName, PVMFFormatType aFormat, int32 aPriority)
{
    PVMFNodeInterface* node = NULL;
    node = OSCL_NEW(PVMFFileOutputNode, (aPriority));
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    ((PVMFFileOutputNode *)node)->iFormat = aFormat;
    ((PVMFFileOutputNode *)node)->iOutputFileName = aFileName.get_cstr();
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF bool PVFileOutputNodeFactory::DeleteFileOutput(PVMFNodeInterface* aNode)
{
    if (aNode)
    {
        OSCL_DELETE(aNode);
        return true;
    }

    return false;
}

