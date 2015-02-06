

#ifndef PV_ENGINE_OBSERVER_H_INCLUDED
#define PV_ENGINE_OBSERVER_H_INCLUDED

#ifndef PV_ENGINE_OBSERVER_MESSAGE_H_INCLUDED
#include "pv_engine_observer_message.h"
#endif

class PVErrorEventObserver
{
    public:
        /**
         * Handle an error event that has been generated.
         *
         * @param "aEvent" "The event to be handled."
         */
        virtual void HandleErrorEvent(const PVAsyncErrorEvent& aEvent) = 0;
        virtual ~PVErrorEventObserver() {}
};

class PVInformationalEventObserver
{
    public:
        /**
         * Handle an informational event that has been generated.
         *
         * @param "aEvent" "The event to be handled."
         */
        virtual void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent) = 0;
        virtual ~PVInformationalEventObserver() {}
};


class PVCommandStatusObserver
{
    public:
        /**
        Handle an event that has been generated.

        @param "aResponse"  "The response to a previously issued command."
        */
        virtual void CommandCompleted(const PVCmdResponse& aResponse) = 0;
        virtual ~PVCommandStatusObserver() {}
};

#endif // PV_ENGINE_OBSERVER_H_INCLUDED



