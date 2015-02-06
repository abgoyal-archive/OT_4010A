
#ifndef PVMI_MEDIA_IO_OBSERVER_H_INCLUDED
#define PVMI_MEDIA_IO_OBSERVER_H_INCLUDED

#ifndef PVMF_EVENT_HANDLING_H_INCLUDED
#include "pvmf_event_handling.h"
#endif

class PvmiMIOObserver
{
    public:
        virtual ~PvmiMIOObserver() {}

        /**
         * Signal of completion for asynchronous PvmiMIOControl requests.
         * @param aResponse Completion response containing status and other data
         */
        virtual void RequestCompleted(const PVMFCmdResp& aResponse) = 0;
        virtual void ReportErrorEvent(PVMFEventType aEventType, PVInterface* aExtMsg = NULL) = 0;
        virtual void ReportInfoEvent(PVMFEventType aEventType, PVInterface* aExtMsg = NULL) = 0;
};

#endif // PVMI_MEDIA_IO_OBSERVER_H_INCLUDED
