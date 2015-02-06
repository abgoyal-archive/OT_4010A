

#ifndef PVMF_MEDIA_INPUT_NODE_EVENTS_H_INCLUDED
#define PVMF_MEDIA_INPUT_NODE_EVENTS_H_INCLUDED

#define PvmfMediaInputNodeEventTypesUUID PVUuid(0x57598870,0xa620,0x11d9,0x96,0x69,0x08,0x00,0x20,0x0c,0x9a,0x66)

typedef enum
{
    //This value is just a placeholder-- the event will not be reported.
    PvmfMediaInputNodeErr_First = 1024

    //This error code indicates that no ports exist, but ports are
    //required to complete the command.
    , PvmfMediaInputNodeErr_NoPorts

    //This error code indicates that a port referenced in a command
    //does not exist.
    , PvmfMediaInputNodeErr_PortNotExist

    //This error code indicates that a Cancel command failed
    //because the indicated command is no longer queueud.
    , PvmfMediaInputNodeErr_CmdNotQueued

    //This error code indicates that the Media I/O component does not exist
    , PvmfMediaInputNodeErr_MediaIONotExist

    //This error code indicates that the Media I/O component was in
    //an unexpected state when the node tried to issue a command.
    , PvmfMediaInputNodeErr_MediaIOWrongState

    //These error codes indicate that a specific command to the
    //Media I/O control component failed.
    , PvmfMediaInputNodeErr_MediaIOConnect
    , PvmfMediaInputNodeErr_MediaIOQueryCapConfigInterface//query for capability & config interface
    , PvmfMediaInputNodeErr_MediaIOInit
    , PvmfMediaInputNodeErr_MediaIOStart
    , PvmfMediaInputNodeErr_MediaIOPause
    , PvmfMediaInputNodeErr_MediaIOStop
    , PvmfMediaInputNodeErr_MediaIOReset
    , PvmfMediaInputNodeErr_MediaIOFlush
    , PvmfMediaInputNodeErr_MediaIOCancelCommand

    //These error codes indicate errors in the data transfer
    , PvmfMediaInputNodeErr_WriteAsync //unexpeced leave in writeAsync
    , PvmfMediaInputNodeErr_WriteComplete //unexpected error reported in writeComplete

    //This value is just a placeholder-- the event will not be reported.
    , PvmfMediaInputNodeErr_Last

    //This value is just a placeholder-- the event will not be reported.
    , PvmfMediaInputNodeInfo_First

    //Notifications of dropped or skipped frames.
    , PvmfMediaInputNodeInfo_FramesDropped
    , PvmfMediaInputNodeInfo_FramesSkipped

    //This value is just a placeholder-- the event will not be reported.
    , PvmfMediaInputNodeInfo_Last

} PvmfMediaInputNodeErrorInfoEventTypes;

#endif // PVMF_MEDIA_INPUT_NODE_EVENTS_H_INCLUDED





