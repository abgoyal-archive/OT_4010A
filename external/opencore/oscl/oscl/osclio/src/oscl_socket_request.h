

#ifndef OSCL_SOCKET_REQUEST_H_INCLUDED
#define OSCL_SOCKET_REQUEST_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_vector.h"
#include "oscl_mem.h"

//Internal use buffer types.
class PVSockBufSend
{
    public:
        PVSockBufSend():
                iPtr(NULL),
                iLen(0)
        {}
        PVSockBufSend(const uint8* aPtr, uint32 aLen):
                iPtr(aPtr),
                iLen(aLen)
        {}
        PVSockBufSend(const PVSockBufSend& a):
                iPtr(a.iPtr),
                iLen(a.iLen)
        {}
        const uint8* iPtr;
        uint32 iLen;
};
class PVSockBufRecv
{
    public:
        PVSockBufRecv():
                iPtr(NULL),
                iLen(0),
                iMaxLen(0)
        {}
        PVSockBufRecv(uint8* aPtr, uint32 aLen, uint32 aMax):
                iPtr(aPtr),
                iLen(aLen),
                iMaxLen(aMax)
        {}
        PVSockBufRecv(const PVSockBufRecv& a)
                : iPtr(a.iPtr)
                , iLen(a.iLen)
                , iMaxLen(a.iMaxLen)
        {}
        uint8* iPtr;
        uint32 iLen;
        uint32 iMaxLen;
};

class OsclSocketI;
class OsclSocketRequestAO;

class SocketRequestParam
{
    public:
        SocketRequestParam(TPVSocketFxn aFxn)
                : iFxn(aFxn)
        {}

        TPVSocketFxn iFxn;
};


class SendParam: public SocketRequestParam
{
    public:
        SendParam(const uint8* &aPtr, uint32 aLen, uint32 aFlags)
                : SocketRequestParam(EPVSocketSend)
                , iBufSend(aPtr, aLen)
                , iFlags(aFlags)
                , iXferLen(0)
        {}
        PVSockBufSend iBufSend;
        uint32 iFlags;
        uint32 iXferLen;
} ;

class SendToParam: public SocketRequestParam
{
    public:
        SendToParam(const uint8* &aPtr, uint32 aLen, OsclNetworkAddress& anAddr, uint32 flags)
                : SocketRequestParam(EPVSocketSendTo)
                , iBufSend(aPtr, aLen)
                , iFlags(flags)
                , iAddr(anAddr)
                , iXferLen(0)
        {}
        ~SendToParam()
        {}
        PVSockBufSend iBufSend;
        uint32 iFlags;
        OsclNetworkAddress iAddr;
        uint32 iXferLen;
} ;

class RecvParam: public SocketRequestParam
{
    public:
        RecvParam(uint8* &aPtr, uint32 aMaxLen, uint32 flags)
                : SocketRequestParam(EPVSocketRecv)
                , iBufRecv(aPtr, 0, aMaxLen)
                , iFlags(flags)
        {}
        PVSockBufRecv iBufRecv;
        uint32 iFlags;
} ;

class RecvFromParam: public SocketRequestParam
{
    public:
        RecvFromParam(uint8* &aPtr, uint32 aMaxLen,
                      OsclNetworkAddress& aAddress, uint32 flags, uint32 aMultiMax
                      , Oscl_Vector<uint32, OsclMemAllocator>*aPacketLen
                      , Oscl_Vector<OsclNetworkAddress, OsclMemAllocator>* aPacketSource)
                : SocketRequestParam(EPVSocketRecvFrom)
                , iBufRecv(aPtr, 0, aMaxLen)
                , iFlags(flags)
                , iAddr(aAddress)
                , iMultiMaxLen(aMultiMax)
                , iPacketLen(aPacketLen)
                , iPacketSource(aPacketSource)
        {}
        PVSockBufRecv iBufRecv;
        uint32 iFlags;
        OsclNetworkAddress& iAddr;
        uint32 iMultiMaxLen;
        Oscl_Vector<uint32, OsclMemAllocator>* iPacketLen;
        Oscl_Vector<OsclNetworkAddress, OsclMemAllocator>* iPacketSource;
};

class BindParam: public SocketRequestParam
{
    public:
        BindParam(OsclNetworkAddress& anAddr)
                : SocketRequestParam(EPVSocketBind)
                , iAddr(anAddr)
        {}
        OsclNetworkAddress iAddr;
} ;

class ListenParam: public SocketRequestParam
{
    public:
        ListenParam(uint32 aSize)
                : SocketRequestParam(EPVSocketListen)
                , iQSize(aSize)
        {}
        uint32 iQSize;
} ;

class ConnectParam: public SocketRequestParam
{
    public:
        ConnectParam(OsclNetworkAddress& anAddr)
                : SocketRequestParam(EPVSocketConnect)
                , iAddr(anAddr)
        {}
        OsclNetworkAddress iAddr;
} ;

class AcceptParam: public SocketRequestParam
{
    public:
        AcceptParam(OsclSocketI& aBlankSocket)
                : SocketRequestParam(EPVSocketAccept)
                , iBlankSocket(&aBlankSocket)
        {}
        OsclSocketI *iBlankSocket;
} ;

class ShutdownParam: public SocketRequestParam
{
    public:
        ShutdownParam(TPVSocketShutdown aHow)
                : SocketRequestParam(EPVSocketShutdown)
                , iHow(aHow)
        {}
        TPVSocketShutdown iHow;
} ;

#include "oscl_socket_tuneables.h"
#if PV_SOCKET_SERVER

class OsclSocketServRequestQElem;

class OsclSocketRequest
{
    public:
        OsclSocketRequest()
                : iSocketRequestAO(NULL)
                , iParam(NULL)
                , iSocketI(NULL)
        {}

        TPVSocketFxn Fxn()
        {
            return iParam->iFxn;
        }

        OsclSocketRequestAO *iSocketRequestAO;
        SocketRequestParam *iParam;
        OsclSocketI * iSocketI;

        void CancelRequest();
        void Activate(SocketRequestParam *iParam, OsclSocketRequestAO &a);

        void Complete(OsclSocketServRequestQElem*, int32 aStatus, int32 aSockErr = 0);

};

#endif


#endif

