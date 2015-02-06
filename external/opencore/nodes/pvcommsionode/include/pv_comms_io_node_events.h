

#ifndef PV_COMMS_IO_NODE_EVENTS_H_INCLUDED
#define PV_COMMS_IO_NODE_EVENTS_H_INCLUDED

#define PVCommsIONodeEventTypesUUID PVUuid(0x57598870,0xa620,0x11d9,0x96,0x69,0x08,0x00,0x20,0x0c,0x9a,0x66)

typedef enum
{
    //This value is just a placeholder-- the event will not be reported.
    PVCommsIONodeErr_First = 1024

    //This error code indicates that no ports exist, but ports are
    //required to complete the command.
    , PVCommsIONodeErr_NoPorts

    //This error code indicates that a port referenced in a command
    //does not exist.
    , PVCommsIONodeErr_PortNotExist

    //This error code indicates that a Cancel command failed
    //because the indicated command is no longer queueud.
    , PVCommsIONodeErr_CmdNotQueued

    //This error code indicates that the Media I/O component does not exist
    , PVCommsIONodeErr_MediaIONotExist

    //This error code indicates that the Media I/O component was in
    //an unexpected state when the node tried to issue a command.
    , PVCommsIONodeErr_MediaIOWrongState

    //These error codes indicate that a specific command to the
    //Media I/O control component failed.
    , PVCommsIONodeErr_MediaIOConnect
    , PVCommsIONodeErr_MediaIOQueryCapConfigInterface//query for capability & config interface
    , PVCommsIONodeErr_MediaIOInit
    , PVCommsIONodeErr_MediaIOStart
    , PVCommsIONodeErr_MediaIOPause
    , PVCommsIONodeErr_MediaIOStop
    , PVCommsIONodeErr_MediaIOFlush
    , PVCommsIONodeErr_MediaIOCancelCommand

    //These error codes indicate errors in the data transfer
    , PVCommsIONodeErr_WriteAsync //unexpeced leave in writeAsync
    , PVCommsIONodeErr_WriteComplete //unexpected error reported in writeComplete

    //This value is just a placeholder-- the event will not be reported.
    , PVCommsIONodeErr_Last

    //This value is just a placeholder-- the event will not be reported.
    , PVCommsIONodeInfo_First

    //Notifications of dropped or skipped frames.
    , PVCommsIONodeInfo_FramesDropped
    , PVCommsIONodeInfo_FramesSkipped

    //This value is just a placeholder-- the event will not be reported.
    , PVCommsIONodeInfo_Last

} PVCommsIONodeErrorInfoEventTypes;

#endif // PV_COMMS_IO_NODE_EVENTS_H_INCLUDED





