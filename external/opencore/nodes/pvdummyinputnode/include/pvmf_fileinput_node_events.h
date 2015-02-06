
#ifndef PVMF_FILEINPUT_NODE_EVENTS_H_INCLUDED
#define PVMF_FILEINPUT_NODE_EVENTS_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

///////////////////////////////////////////////
// Error Events
///////////////////////////////////////////////

enum PVMFFileInputNodeErrorEvent
{
    PVMF_FILEINPUT_NODE_ERROR_PORT_PROCESSING_ERROR = PVMF_NODE_ERROR_EVENT_LAST
    , PVMF_FILEINPUT_NODE_ERROR_DATA_GENERATING_ERROR
};

///////////////////////////////////////////////
// Information Events
///////////////////////////////////////////////

// Enumerated list of informational event from PVMFFileInputNode
enum PVMFFileInputNodeInfoEvent
{
    PVMFFIN_EndOfFile = PVMF_NODE_INFO_EVENT_LAST
};

#endif


