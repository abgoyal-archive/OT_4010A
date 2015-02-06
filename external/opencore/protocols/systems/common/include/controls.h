
#if !defined(CPVCONTROLS_H)
#define CPVCONTROLS_H
#include "oscl_types.h"
#include "pvt_params.h"

class CPVControlsObserver
{
    public:
        virtual void HandleControlMessage(PS_ControlMsgHeader msg) = 0;
};

class CPVControls
{
    public:
        CPVControls() {}
        virtual ~CPVControls() {}
        virtual void DispatchControlMessage(PS_ControlMsgHeader msg) = 0;
        virtual void SetControlsObserver(CPVControlsObserver* observer) = 0;
    protected:
        //CPVControlsObserver* iObserver;
};
#endif
