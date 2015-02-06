

#include "oscl_base.h"

#include "pvmf_omx_audiodec_node.h"
#include "pvmf_omx_audiodec_factory.h"

#include "oscl_error_codes.h"
#include "oscl_exception.h"

// Use default DLL entry point
#include "oscl_dll.h"

OSCL_DLL_ENTRY_POINT_DEFAULT()

/* Audio decoder node factory methods */


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFNodeInterface* PVMFOMXAudioDecNodeFactory::CreatePVMFOMXAudioDecNode(int32 aPriority)
{
    PVMFNodeInterface* node = NULL;
    node = new PVMFOMXAudioDecNode(aPriority);
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF bool PVMFOMXAudioDecNodeFactory::DeletePVMFOMXAudioDecNode(PVMFNodeInterface* aNode)
{
    if (aNode)
    {
        delete aNode;
        return true;
    }

    return false;
}

