
#ifndef OSCL_EXCEPTION_H_INCLUDED
#include "oscl_exception.h"
#endif
#ifndef PVMF_SM_RTSP_UNICAST_NODE_FACTORY_H_INCLUDED
#include "pvmf_sm_rtsp_unicast_node_factory.h"
#endif

#ifndef PVMF_SM_FSP_RTSP_UNICAST_H
#include "pvmf_sm_fsp_rtsp_unicast.h"
#endif

OSCL_EXPORT_REF PVMFSMFSPBaseNode*
PVMFSMRTSPUnicastNodeFactory::CreateSMRTSPUnicastNodeFactory(int32 aPriority)
{
    PVMFSMFSPBaseNode* smRtspUnicastNode = NULL;
    int32 err;
    OSCL_TRY(err,

             smRtspUnicastNode = PVMFSMRTSPUnicastNode::New(aPriority);
            );

    if (err != OsclErrNone)
    {
        OSCL_LEAVE(err);
    }

    return (smRtspUnicastNode);
}

OSCL_EXPORT_REF bool PVMFSMRTSPUnicastNodeFactory::DeleteSMRTSPUnicastNodeFactory(PVMFSMFSPBaseNode* aSMFSPNode)
{

    bool retval = false;

    if (aSMFSPNode)
    {
        OSCL_DELETE(aSMFSPNode);
        aSMFSPNode = NULL;
        retval = true;
    }

    return retval;
}
