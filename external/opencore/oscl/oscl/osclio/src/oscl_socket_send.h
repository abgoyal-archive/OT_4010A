

#ifndef OSCL_SOCKET_SEND_H_INCLUDED
#define OSCL_SOCKET_SEND_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_socket_method.h"

class OsclSendRequest;

class OsclSendMethod : public OsclSocketMethod
{
    public:
        static OsclSendMethod *NewL(OsclIPSocketI& c);

        ~OsclSendMethod();

        TPVSocketEvent Send(const uint8* &aPtr, uint32 aLen,
                            int32 aTimeout);

        uint8 *GetSendData(int32 *aLength);

    private:
        OsclSendMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclsendmethod", EPVSocketSend)
        {};

        void ConstructL();

    public:
        OsclSendRequest *SendRequest()
        {
            return (OsclSendRequest*)iSocketRequestAO;
        }
};

class SendParam;
class OsclSendRequest : public OsclSocketRequestAO
{
    public:
        OsclSendRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclSendRequest")
        {}


        void Send(const uint8* &aPtr, uint32 aLen);
        void Success();
        uint8 *GetSendData(int32 *aLength);

    private:

        SendParam*Param()
        {
            return(SendParam*)iParam;
        }

};




#endif

