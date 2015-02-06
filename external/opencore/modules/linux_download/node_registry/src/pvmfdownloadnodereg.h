
#ifndef PVMFDOWNLOADNODEREG_H_INCLUDED
#define PVMFDOWNLOADNODEREG_H_INCLUDED

#ifndef OSCL_SHARED_LIBRARY_H_INCLUDED
#include "oscl_shared_library.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

class DownloadNodesCoreLibraryLoader
{
    public:
        static PVMFNodeInterface* CreateDownloadManagerNode(int32 aPriority);
        static bool DeleteDownloadManagerNode(PVMFNodeInterface* aNode);
};

#endif // PVMFDOWNLOADNODEREG_H_INCLUDED

