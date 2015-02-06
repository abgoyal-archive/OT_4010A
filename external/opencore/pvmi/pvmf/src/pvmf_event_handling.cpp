

#include "pvmf_event_handling.h"

// TODO: header file is bloated with inlined code. Move it here.

// ----------------------------------------------------------------------
// PVMFCmdResp

OSCL_EXPORT_REF PVMFEventCategory PVMFCmdResp::IsA() const
{
    return PVMFCmdRespEvent;
}


// ----------------------------------------------------------------------
// PVMFAsyncEvent

OSCL_EXPORT_REF PVMFEventCategory PVMFAsyncEvent::IsA() const
{
    return iEventCategory;
}
