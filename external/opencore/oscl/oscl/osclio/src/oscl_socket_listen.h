

#ifndef OSCL_SOCKET_LISTEN_H_INCLUDED
#define OSCL_SOCKET_LISTEN_H_INCLUDEDd

#include "oscl_socket_types.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"

class OsclListenRequest;

class OsclListenMethod : public OsclSocketMethod
{
    public:
        static OsclListenMethod *NewL(OsclIPSocketI& c);

        ~OsclListenMethod();

        TPVSocketEvent Listen(uint32 qsize,
                              int32 aTimeout);

    private:
        OsclListenMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "oscllistenmethod", EPVSocketListen)
        {}

        void ConstructL();

    public:
        OsclListenRequest *ListenRequest()
        {
            return (OsclListenRequest*)iSocketRequestAO;
        }
};

class OsclListenRequest : public OsclSocketRequestAO
{
    public:
        OsclListenRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclListenRequest")
        {}

        void Listen(uint32 qsize);

    private:

        ListenParam*Param()
        {
            return(ListenParam*)iParam;
        }

};


#endif



