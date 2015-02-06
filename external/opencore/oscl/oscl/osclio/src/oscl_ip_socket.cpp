

#include "oscl_ip_socket.h"
#include "oscl_error.h"
#include "oscl_socket_imp.h"
#include "pvlogger.h"

void OsclIPSocketI::ConstructL(OsclSocketObserver *aObs, OsclSocketI* aSock, OsclSocketServI* aServ, uint32 aId)
{
    if (!aObs || !aSock || !aServ)
        OsclError::Leave(OsclErrGeneral);
    iObserver = aObs;
    iSocket = aSock;
    iSocketServ = aServ;
    iId = aId;
    iLogger = PVLogger::GetLoggerObject("osclsocket");
}


int32 OsclIPSocketI::Bind(OsclNetworkAddress &aAddress)
{
    if (iSocket)
    {
        iAddress.ipAddr.Set(aAddress.ipAddr.Str());
        iAddress.port = aAddress.port;
        return iSocket->Bind(aAddress);
    }
    else
    {
        return OsclErrGeneral;
    }
}

int32 OsclIPSocketI::Join(OsclNetworkAddress &aAddress)
{
    if (iSocket)
    {
        iAddress.ipAddr.Set(aAddress.ipAddr.Str());
        iAddress.port = aAddress.port;
        return iSocket->Join(aAddress);
    }
    else
    {
        return OsclErrGeneral;
    }
}

int32 OsclIPSocketI::SetRecvBufferSize(uint32 size)
{
    if (iSocket)
    {
        return iSocket->SetRecvBufferSize(size);
    }
    return OsclErrGeneral;
}





