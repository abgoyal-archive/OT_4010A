
#ifndef PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED
#define PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class PvmiConfigAndCapabilityCmdObserver
{
    public:
        /**
         * This method allows to send a signal of completion for a particular event
         */
        virtual void SignalEvent(int32 req_id) = 0;
        virtual ~PvmiConfigAndCapabilityCmdObserver() {}
};

#endif //PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED
