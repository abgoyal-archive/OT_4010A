
#ifndef PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED
#define PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED

#ifndef PV_2WAY_DATAPATH_H_INCLUDED
#include "pv_2way_datapath.h"
#endif

class CPV2WayCmdControlDatapath : public CPV2WayDatapath
{
    public:
        CPV2WayCmdControlDatapath(PVLogger *aLogger,
                                  TPV2WayDatapathType aType,
                                  PVMFFormatType aFormat,
                                  CPV324m2Way *a2Way) : CPV2WayDatapath(aLogger, aType, aFormat, a2Way),
                iCmdInfo(NULL)

        {};

        virtual ~CPV2WayCmdControlDatapath() {};

        TPV2WayCmdInfo *GetCmdInfo()
        {
            return iCmdInfo;
        }
        virtual bool SetCmd(TPV2WayCmdInfo *aCmdInfo) = 0;

    protected:
        bool CloseDatapath(TPV2WayCmdInfo *aCmdInfo);

        void CommandComplete(PVMFStatus aStatus);

        TPV2WayCmdInfo *iCmdInfo;
};


#endif //PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED


