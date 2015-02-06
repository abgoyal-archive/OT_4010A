

#ifndef OSCL_SOCKET_CONNECT_H_INCLUDED
#define OSCL_SOCKET_CONNECT_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"

class OsclConnectRequest;

class OsclConnectMethod : public OsclSocketMethod
{
    public:
        static OsclConnectMethod *NewL(OsclIPSocketI& c);

        ~OsclConnectMethod();

        TPVSocketEvent Connect(OsclNetworkAddress& aAddress,
                               int32 aTimeout);

    private:
        OsclConnectMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclconnectmethod", EPVSocketConnect)
        {}

        void ConstructL();

    public:
        OsclConnectRequest *ConnectRequest()
        {
            return (OsclConnectRequest*)iSocketRequestAO;
        }
};

class OsclConnectRequest : public OsclSocketRequestAO
{
    public:
        OsclConnectRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclConnectRequest")
        {}

        void Connect(OsclNetworkAddress &aAddress);

    private:

        ConnectParam*Param()
        {
            return(ConnectParam*)iParam;
        }

};


#endif



