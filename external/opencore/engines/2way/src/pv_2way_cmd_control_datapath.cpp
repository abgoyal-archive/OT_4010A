
#include "pv_2way_cmd_control_datapath.h"


bool CPV2WayCmdControlDatapath::CloseDatapath(TPV2WayCmdInfo *aCmdInfo)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_NOTICE, (0, "CPV2WayCmdControlDatapath::CloseDatapath state %d cmd %x\n", iState, aCmdInfo));
    if (iState == EClosing)
    {
        if (iCmdInfo == NULL) iCmdInfo = aCmdInfo;
        return false;
    }
    else
    {
        //If command was already in progress when close was initiated.
        if (iCmdInfo)
        {
            if ((aCmdInfo == NULL) && (iState == EOpening))
            {
                Close();
                return false;
            }

            CommandComplete(PVMFErrCancelled);
        }

        iCmdInfo = aCmdInfo;
        Close();

        return true;
    }
}

void CPV2WayCmdControlDatapath::CommandComplete(PVMFStatus aStatus)
{
    iCmdInfo->status = aStatus;
    i2Way->Dispatch(iCmdInfo);
    iCmdInfo = NULL;
}









