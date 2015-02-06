
#ifndef PV_2WAY_PREVIEW_DATAPATH_H_INCLUDED
#define PV_2WAY_PREVIEW_DATAPATH_H_INCLUDED


#ifndef PV_2WAY_DATAPATH_H_INCLUDED
#include "pv_2way_datapath.h"
#endif

#ifndef PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED
#include "pv_2way_cmd_control_datapath.h"
#endif

class CPV2WayPreviewDatapath : public CPV2WayCmdControlDatapath
{
    public:
        static CPV2WayPreviewDatapath *NewL(PVLogger *aLogger,
                                            TPV2WayMediaType aFormat,
                                            CPV324m2Way *a2Way);

        virtual ~CPV2WayPreviewDatapath() {};

        bool SetCmd(TPV2WayCmdInfo *aCmdInfo);

    private:
        CPV2WayPreviewDatapath(PVLogger *aLogger,
                               TPV2WayMediaType aFormat,
                               CPV324m2Way *a2Way) : CPV2WayCmdControlDatapath(aLogger, EPreviewDatapath, aFormat, a2Way)
        {};

        //Override base virtuals
        void CheckPause();
        void CheckResume();

        void OpenComplete();
        void PauseComplete()
        {
            CommandComplete(PVMFSuccess);
        }
        void ResumeComplete()
        {
            CommandComplete(PVMFSuccess);
        }
        void CloseComplete();

        void DatapathError();

        bool ParentIsClosing();
};


#endif //PV_2WAY_PREVIEW_DATAPATH_H_INCLUDED


