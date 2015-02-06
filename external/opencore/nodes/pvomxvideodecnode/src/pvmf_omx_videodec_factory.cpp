

#include "oscl_base.h"

#include "pvmf_omx_videodec_node.h"
#include "pvmf_omx_videodec_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point
#include "oscl_dll.h"

OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFNodeInterface* PVMFOMXVideoDecNodeFactory::CreatePVMFOMXVideoDecNode(int32 aPriority, bool aHwAccelerated)
{
    PVMFNodeInterface* node = NULL;
    node = new PVMFOMXVideoDecNode(aPriority, aHwAccelerated);
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF bool PVMFOMXVideoDecNodeFactory::DeletePVMFOMXVideoDecNode(PVMFNodeInterface* aNode)
{
    if (aNode)
    {
        delete aNode;
        return true;
    }

    return false;
}

