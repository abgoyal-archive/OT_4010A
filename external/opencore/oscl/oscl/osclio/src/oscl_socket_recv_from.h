

#ifndef OSCL_SOCKET_RECV_FROM_H_INCLUDED
#define OSCL_SOCKET_RECV_FROM_H_INCLUDED

#include "oscl_socket_serv_imp.h"
#include "oscl_socket_imp.h"
#include "oscl_socket_method.h"

class OsclRecvFromRequest;

class OsclRecvFromMethod : public OsclSocketMethod
{
    public:
        static OsclRecvFromMethod *NewL(OsclIPSocketI& c);

        ~OsclRecvFromMethod();

        TPVSocketEvent RecvFrom(uint8* &aPtr, uint32 aMaxLen,
                                OsclNetworkAddress &aAddress,
                                int32 aTimeout,
                                uint32 aMultiMax,
                                Oscl_Vector<uint32, OsclMemAllocator>* aPacketLen,
                                Oscl_Vector<OsclNetworkAddress, OsclMemAllocator>* aPacketSource);

        uint8 *GetRecvData(int32 *aLength);

    private:
        OsclRecvFromMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclrecvfrommethod", EPVSocketRecvFrom)
        {}

        void ConstructL();

    public:
        OsclRecvFromRequest *RecvFromRequest()
        {
            return (OsclRecvFromRequest*)iSocketRequestAO;
        }
};

class OsclRecvFromRequest : public OsclSocketRequestAO
{
    public:
        uint8 *GetRecvData(int32 *aLength);

        OsclRecvFromRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclRecvFromRequest")
        {}

        void RecvFrom(uint8* &aPtr, uint32 aMaxLen,
                      OsclNetworkAddress& aAddress, uint32 aMultiMax,
                      Oscl_Vector<uint32, OsclMemAllocator>* aPacketLen,
                      Oscl_Vector<OsclNetworkAddress, OsclMemAllocator>* aPacketSource);
        void Success();

    private:
        RecvFromParam*Param()
        {
            return(RecvFromParam*)iParam;
        }
};


#endif

