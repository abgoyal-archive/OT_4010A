

#ifndef OSCL_TCP_SOCKET_H_INCLUDED
#define OSCL_TCP_SOCKET_H_INCLUDED

#ifndef OSCL_IP_SOCKET_H_INCLUDED
#include "oscl_ip_socket.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class OsclBindMethod;
class OsclListenMethod;
class OsclConnectMethod;
class OsclShutdownMethod;
class OsclAcceptMethod;
class OsclSendMethod;
class OsclRecvMethod;

class OsclTCPSocketI : public OsclIPSocketI
{
    public:
        //Synchronous methods
        static OsclTCPSocketI *NewL(Oscl_DefAlloc &a,
                                    OsclSocketServI *aServ,
                                    OsclSocketObserver *aObserver,
                                    uint32 aId);

        virtual ~OsclTCPSocketI();

        int32 Close();
        inline int32 Listen(int aQueueSize);
        //the returned value is platform-specific

        OsclTCPSocketI *GetAcceptedSocketL(uint32 aId);

        inline uint8 *GetRecvData(int32 *aLength) ;
        inline uint8 *GetSendData(int32 *aLength);

        //Asynchronous methods
        inline TPVSocketEvent BindAsync(OsclNetworkAddress& aAddress,
                                        int32 aTimeoutMsec = -1);
        inline void CancelBind();

        inline TPVSocketEvent ListenAsync(uint32 qsize,
                                          int32 aTimeoutMsec = -1);
        inline void CancelListen();

        inline TPVSocketEvent Connect(OsclNetworkAddress& aAddress,
                                      int32 aTimeoutMsec = -1);
        inline void CancelConnect();

        inline TPVSocketEvent Shutdown(TPVSocketShutdown  aHow,
                                       int32 aTimeoutMsec = -1);
        inline void CancelShutdown();

        inline TPVSocketEvent Accept(int32 aTimeout = -1);
        inline void CancelAccept();

        inline TPVSocketEvent Send(const uint8* &aPtr, uint32 aLen,
                                   int32 aTimeoutMsec = -1);
        inline void CancelSend();

        inline TPVSocketEvent Recv(uint8* &aPtr, uint32 aMaxLen,
                                   int32 aTimeoutMsec = -1);
        inline void CancelRecv();

// <--- Morris Yang dump tcp
        inline void DumpRecv();
// --->

    private:
        static OsclTCPSocketI *NewL(Oscl_DefAlloc &a,
                                    OsclSocketServI *aServ,
                                    OsclSocketI *aSocket,
                                    OsclSocketObserver *aObserver,
                                    uint32 aId);

        OsclTCPSocketI(Oscl_DefAlloc &a) : OsclIPSocketI(a),
                iConnectMethod(NULL),
                iShutdownMethod(NULL),
                iAcceptMethod(NULL),
                iSendMethod(NULL),
                iRecvMethod(NULL)
        {}

        void ConstructL(OsclSocketServI *aServ,
                        OsclSocketObserver *aObserver,
                        uint32 aId);

        void ConstructL(OsclSocketServI *aServ,
                        OsclSocketI *aSocket,
                        OsclSocketObserver *aObserver,
                        uint32 aId);

        OsclBindMethod *iBindMethod;
        OsclListenMethod *iListenMethod;
        OsclConnectMethod *iConnectMethod;
        OsclShutdownMethod *iShutdownMethod;
        OsclAcceptMethod *iAcceptMethod;
        OsclSendMethod *iSendMethod;
        OsclRecvMethod *iRecvMethod;
};

#include "oscl_socket_listen.h"
#include "oscl_socket_recv.h"
#include "oscl_socket_send.h"
#include "oscl_socket_accept.h"
#include "oscl_socket_shutdown.h"
#include "oscl_socket_connect.h"
#include "oscl_socket_bind.h"

//////////////////////////////////////////////////////////////////////////////////
inline int32 OsclTCPSocketI::Listen(int aQueueSize)
{
    return iSocket->Listen(aQueueSize) ;
}

//////////////////////////////////////////////////////////////////////////////////
inline uint8 *OsclTCPSocketI::GetRecvData(int32 *aLength)
{
    return iRecvMethod->GetRecvData(aLength);
}

//////////////////////////////////////////////////////////////////////////////////
inline uint8 *OsclTCPSocketI::GetSendData(int32 *aLength)
{
    return iSendMethod->GetSendData(aLength);
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::BindAsync(OsclNetworkAddress& aAddress,
        int32 aTimeout)
{
    if (!OsclSocketIBase::HasAsyncBind())
        return EPVSocketFailure;//not available.

    iAddress.ipAddr.Set(aAddress.ipAddr.Str());
    iAddress.port = aAddress.port;
    return (iBindMethod->Bind(aAddress, aTimeout));
}

inline void OsclTCPSocketI::CancelBind()
{
    iBindMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::ListenAsync(uint32 qsize,
        int32 aTimeout)
{
    if (!OsclSocketIBase::HasAsyncListen())
        return EPVSocketFailure;//not available

    return (iListenMethod->Listen(qsize, aTimeout));
}

inline void OsclTCPSocketI::CancelListen()
{
    iListenMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::Connect(OsclNetworkAddress& aAddress,
        int32 aTimeout)
{
    return (iConnectMethod->Connect(aAddress, aTimeout));
}

inline void OsclTCPSocketI::CancelConnect()
{
    iConnectMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::Shutdown(TPVSocketShutdown  aHow,
        int32 aTimeout)
{
    return (iShutdownMethod->Shutdown(aHow, aTimeout));
}

inline void OsclTCPSocketI::CancelShutdown()
{
    iShutdownMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::Accept(int32 aTimeout)
{
    return (iAcceptMethod->Accept(aTimeout));
}

inline void OsclTCPSocketI::CancelAccept()
{
    iAcceptMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::Send(const uint8* &aPtr, uint32 aLen,
        int32 aTimeoutMsec)
{
// <--- Morris Yang dump tcp
// --->
    return (iSendMethod->Send(aPtr, aLen, aTimeoutMsec));
}

inline void OsclTCPSocketI::CancelSend()
{
    iSendMethod->CancelMethod();
}

//////////////////////////////////////////////////////////////////////////////////
inline TPVSocketEvent OsclTCPSocketI::Recv(uint8* &aPtr, uint32 aMaxLen,
        int32 aTimeout)
{
    return (iRecvMethod->Recv(aPtr, aMaxLen, aTimeout));
}

inline void OsclTCPSocketI::CancelRecv()
{
    iRecvMethod->CancelMethod();
}

// <--- Morris Yang dump tcp
inline void OsclTCPSocketI::DumpRecv()
{
	//LOGE ("[M] OsclTCPSocketI(0x%x) DumpRecv", this);
	FILE* file = NULL;
	char _path[255];
	//sprintf (_path, "/data/tcpdump/TCP_0x%x_Recv.%d", this, _seq_no++);
	sprintf (_path, "/data/tcp_recv.bin");
	file = fopen(_path, "a");
	if (file != NULL){
	int32 _len = 0;
	 uint8* _ptr = iRecvMethod->GetRecvData(&_len);
	 if (_len > 0)
   	     fwrite(_ptr, 1, _len, file);
	 fclose(file);
	 }
}
// --->
#endif

