
#ifndef PVMFMP4NODEREG_H_INCLUDED
#define PVMFMP4NODEREG_H_INCLUDED

#ifndef OSCL_SHARED_LIBRARY_H_INCLUDED
#include "oscl_shared_library.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

class Mp4NodesCoreLibraryLoader
{
    public:
        static PVMFNodeInterface* CreateMp4ParserNode(int32 aPriority);
        static bool DeleteMp4ParserNode(PVMFNodeInterface* aNode);
};

#endif // PVMFMP4NODEREG_H_INCLUDED

