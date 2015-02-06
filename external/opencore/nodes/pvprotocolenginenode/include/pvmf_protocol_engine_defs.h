
#ifndef PVMF_PROTOCOLENGINE_DEFS_H_INCLUDED
#define PVMF_PROTOCOLENGINE_DEFS_H_INCLUDED


///////////////////////////////////////////////
// Port tags
///////////////////////////////////////////////

typedef enum
{
    PVMF_PROTOCOLENGINENODE_PORT_TYPE_INPUT,
    PVMF_PROTOCOLENGINENODE_PORT_TYPE_OUTPUT,
    PVMF_PROTOCOLENGINENODE_PORT_TYPE_FEEDBACK
} PVMFProtocolEngineNodePortType;


// Capability mime strings
#define PVMF_PROTOCOLENGINE_PORT_INPUT_FORMATS "x-pvmf/port/formattype"
#define PVMF_PROTOCOLENGINE_PORT_INPUT_FORMATS_VALTYPE "x-pvmf/port/formattype;valtype=int32"

#endif

