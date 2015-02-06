

#ifndef OSCL_SOCKET_SEND_TO_H_INCLUDED
#define OSCL_SOCKET_SEND_TO_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"

class OsclSendToRequest;

class OsclSendToMethod : public OsclSocketMethod
{
    public:
        static OsclSendToMethod *NewL(OsclIPSocketI& c);
        ~OsclSendToMethod();

        TPVSocketEvent SendTo(const uint8* &aPtr, uint32 aLen,
                              OsclNetworkAddress& aAddress,
                              int32 aTimeout);

        uint8 *GetSendData(int32 *aLength);

    private:
        OsclSendToMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclsendtomethod", EPVSocketSendTo)
        {};

        void ConstructL();

    public:
        OsclSendToRequest *SendToRequest()
        {
            return (OsclSendToRequest*)iSocketRequestAO;
        }
};

class OsclSendToRequest : public OsclSocketRequestAO
{
    public:
        OsclSendToRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclSendToRequest")
        {}

        void SendTo(const uint8* &aPtr, uint32 aLen, OsclNetworkAddress &aAddress);
        void Success();

        uint8 *GetSendData(int32 *aLength);

    private:

        SendToParam*Param()
        {
            return(SendToParam*)iParam;
        }
};





#endif

