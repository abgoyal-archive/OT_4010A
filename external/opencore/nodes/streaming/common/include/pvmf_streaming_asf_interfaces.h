
///////////////////////////////////////////////////////////////////////////////
//
// pvmf_streaming_asf_interfaces.h
//
// Defines abstract interfaces that will be used by streaming nodes to talk to
// modules providing ASF streaming related functionality.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PVMF_STREAMING_ASF_INTERFACES_INCLUDED
#define PVMF_STREAMING_ASF_INTERFACES_INCLUDED

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif
#ifndef PVMF_MEDIA_PRESENTATION_INFO_H_INCLUDED
#include "pvmf_media_presentation_info.h"
#endif

class StreamAsfHeader
{
    public:
        StreamAsfHeader()
        {
            iASFHeaderObject = NULL;
        };
        ~StreamAsfHeader();
        OsclAny*      iASFHeaderObject;
};

#endif // PVMF_STREAMING_ASF_INTERFACES_INCLUDED


