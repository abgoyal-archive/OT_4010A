

#ifndef OSCL_SOCKET_RECV_H_INCLUDED
#define OSCL_SOCKET_RECV_H_INCLUDED

#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"


class OsclRecvRequest;

class OsclRecvMethod : public OsclSocketMethod
{
    public:
        static OsclRecvMethod *NewL(OsclIPSocketI& c);

        ~OsclRecvMethod();

        TPVSocketEvent Recv(uint8* &aPtr, uint32 aMaxLen,
                            int32 aTimeout);

        uint8 *GetRecvData(int32 *aLength);

    private:
        OsclRecvMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclrecvmethod", EPVSocketRecv)
        {}

        void ConstructL();

    public:
        OsclRecvRequest *RecvRequest()
        {
            return (OsclRecvRequest*)iSocketRequestAO;
        }
};

class OsclRecvRequest : public OsclSocketRequestAO
{
    public:
        uint8 *GetRecvData(int32 *aLength);

        OsclRecvRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclRecvRequest")
        {}

        void Recv(uint8* &aPtr, uint32 aMaxLen);
        void Success();

    private:

        RecvParam*Param()
        {
            return(RecvParam*)iParam;
        }

};



#endif

