

#include "oscl_base.h"

#include "pvmf_omx_enc_node.h"
#include "pvmf_omx_enc_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point
#include "oscl_dll.h"

OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFNodeInterface* PVMFOMXEncNodeFactory::CreatePVMFOMXEncNode(int32 aPriority)
{
    PVMFNodeInterface* node = NULL;
    node = new PVMFOMXEncNode(aPriority);
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF bool PVMFOMXEncNodeFactory::DeletePVMFOMXEncNode(PVMFNodeInterface* aNode)
{
    if (aNode)
    {
        delete aNode;
        return true;
    }

    return false;
}

