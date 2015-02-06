
#ifndef TSC_NODE_INTERFACE_H_INCLUDED
#define TSC_NODE_INTERFACE_H_INCLUDED

#ifndef PVMF_NODE_UTILS_H_INCLUDED
#include "pvmf_node_utils.h"
#endif


//memory allocator type for this node.
typedef OsclMemAllocator Tsc324mNodeAllocator;

#define Tsc324mNodeCommandBase PVMFGenericNodeCommand<Tsc324mNodeAllocator>  // to remove typedef warning on symbian

class Tsc324mNodeCommand: public Tsc324mNodeCommandBase
{
    public:
        Tsc324mNodeCommand() {}
};

typedef PVMFNodeCommandQueue<Tsc324mNodeCommand, Tsc324mNodeAllocator> Tsc324mNodeCmdQ;

#endif

