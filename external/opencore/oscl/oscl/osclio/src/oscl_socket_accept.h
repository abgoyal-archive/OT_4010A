

#ifndef OSCL_SOCKET_ACCEPT_H_INCLUDED
#define OSCL_SOCKET_ACCEPT_H_INCLUDED


#include "oscl_socket_imp.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_socket_method.h"

class OsclAcceptRequest;

class OsclAcceptMethod : public OsclSocketMethod
{
    public:
        static OsclAcceptMethod *NewL(OsclIPSocketI& c);

        ~OsclAcceptMethod();

        TPVSocketEvent Accept(int32 aTimeout);

        void DiscardAcceptedSocket();
        OsclSocketI *GetAcceptedSocket();

    private:

        OsclAcceptMethod(OsclIPSocketI &c)
                : OsclSocketMethod(c, "osclacceptmethod", EPVSocketAccept)
                , iAcceptedSocket(NULL)
        {}

        void ConstructL();

        void Run();

        OsclSocketServI *SocketServ()
        {
            return iContainer.SocketServ();
        }
    public:
        OsclAcceptRequest *AcceptRequest()
        {
            return (OsclAcceptRequest*)iSocketRequestAO;
        }
    private:
        OsclSocketI *iAcceptedSocket;
};

class AcceptParam;

class OsclAcceptRequest : public OsclSocketRequestAO
{
    public:
        OsclAcceptRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclacceptreq")
        {}

        void Accept(OsclSocketI &aSocket);

    private:
        void Run();

        AcceptParam*Param()
        {
            return(AcceptParam*)iParam;
        }
};




#endif

