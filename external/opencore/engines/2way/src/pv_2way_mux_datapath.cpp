
#include "pv_2way_mux_datapath.h"

CPV2WayMuxDatapath *CPV2WayMuxDatapath::NewL(PVLogger *aLogger,
        PVMFFormatType aFormat,
        CPV324m2Way *a2Way)
{
    CPV2WayMuxDatapath *self = OSCL_NEW(CPV2WayMuxDatapath, (aLogger, aFormat, a2Way));
    OsclError::LeaveIfNull(self);

    if (self)
    {
        OSCL_TRAPSTACK_PUSH(self);
        self->ConstructL();
    }

    OSCL_TRAPSTACK_POP();
    return self;
}

void CPV2WayMuxDatapath::OpenComplete()
{
    i2Way->CheckConnect();
}

void CPV2WayMuxDatapath::PauseComplete()
{
    //Mux cannot pause
}

void CPV2WayMuxDatapath::ResumeComplete()
{
    //Mux cannot resume
}

void CPV2WayMuxDatapath::CloseComplete()
{
    i2Way->iIsStackConnected = false;
    i2Way->CheckDisconnect();
}

void CPV2WayMuxDatapath::DatapathError()
{
    i2Way->SetState(EDisconnecting);
    Close();
}





