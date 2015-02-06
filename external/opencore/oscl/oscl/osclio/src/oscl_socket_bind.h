

#ifndef OSCL_SOCKET_BIND_H_INCLUDED
#define OSCL_SOCKET_BIND_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"

class OsclBindRequest;

class OsclBindMethod : public OsclSocketMethod
{
    public:
        static OsclBindMethod *NewL(OsclIPSocketI& c);

        ~OsclBindMethod();

        TPVSocketEvent Bind(OsclNetworkAddress& aAddress,
                            int32 aTimeout);

    private:
        OsclBindMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclbindmethod", EPVSocketBind)
        {}

        void ConstructL();

    public:
        OsclBindRequest *BindRequest()
        {
            return (OsclBindRequest*)iSocketRequestAO;
        }
};

class OsclBindRequest : public OsclSocketRequestAO
{
    public:
        OsclBindRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclBindRequest")
        {}

        void Bind(OsclNetworkAddress &aAddress);

    private:

        BindParam*Param()
        {
            return(BindParam*)iParam;
        }

};


#endif



