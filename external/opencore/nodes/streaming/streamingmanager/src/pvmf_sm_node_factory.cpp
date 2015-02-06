
#ifndef OSCL_EXCEPTION_H_INCLUDED
#include "oscl_exception.h"
#endif
#ifndef PVMF_SM_NODE_FACTORY_H_INCLUDED
#include "pvmf_sm_node_factory.h"
#endif
#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif
#ifndef PVMF_STREAMING_MANAGER_NODE_H_INCLUDED
#include "pvmf_streaming_manager_node.h"
#endif


OSCL_EXPORT_REF PVMFNodeInterface*
PVMFStreamingManagerNodeFactory::CreateStreamingManagerNode(int32 aPriority)
{
    PVMFNodeInterface* streamingManagerNode = NULL;

    int32 err;

    OSCL_TRY(err,
             /*
              * Create Streaming Manager Node
              */
             streamingManagerNode = PVMFStreamingManagerNode::New(aPriority);
            );

    if (err != OsclErrNone)
    {
        OSCL_LEAVE(err);
    }
    return (streamingManagerNode);
}

OSCL_EXPORT_REF bool PVMFStreamingManagerNodeFactory::DeleteStreamingManagerNode(PVMFNodeInterface* aNode)
{
    bool retval = false;

    if (aNode)
    {
        OSCL_DELETE(aNode);
        retval = true;
        aNode = NULL;
    }

    /*if(EPVMFNodeIdle == aNode->GetState())
    {
        if(aNode)
        {
            OSCL_DELETE(aNode);
            retval = true;
        }
    }*/
    return retval;
}
