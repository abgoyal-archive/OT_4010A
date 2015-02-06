


#ifndef OSCL_SOCKET_TYPES_H_INCLUDED
#define OSCL_SOCKET_TYPES_H_INCLUDED

#include "osclconfig_io.h"
#include "oscl_types.h"
#include "oscl_scheduler_types.h"
#include "oscl_namestring.h"
#include "oscl_stdstring.h"

enum TPVSocketFxn
{
    EPVSocketSend = 0
    , EPVSocketSendTo
    , EPVSocketRecv
    , EPVSocketRecvFrom
    , EPVSocketConnect
    , EPVSocketAccept
    , EPVSocketShutdown
    , EPVSocketBind
    , EPVSocketListen
    , EPVSocket_Last //placeholder
} ;

enum TPVSocketEvent
{
    EPVSocketSuccess
    , EPVSocketPending
    , EPVSocketTimeout
    , EPVSocketFailure
    , EPVSocketCancel
} ;

enum TPVSocketShutdown
{
    EPVSocketSendShutdown
    , EPVSocketRecvShutdown
    , EPVSocketBothShutdown
} ;

#define PVNETWORKADDRESS_LEN 50

class OsclNetworkAddress
{
    public:
        OsclNetworkAddress(): port(0)
        {
        }
        OsclNetworkAddress(const char *addr, int p)
        {
            ipAddr.Set(addr);
            port = p;
        }

        //Network address in dotted decimal string format.
        OsclNameString<PVNETWORKADDRESS_LEN> ipAddr;

        //Port number.
        int port;
        //@cmember equality comparison operator
        bool operator == (const OsclNetworkAddress & rhs) const
        {
            if (port == rhs.port)
            {
                if (0 == oscl_strcmp((const char*)ipAddr.Str(), (const char*)rhs.ipAddr.Str()))
                    return true;
            }
            return false;
        };

} ;

class OsclSocketObserver
{
    public:
        /**
         * Socket Event callback.
         *
         * @param aId: The ID that was supplied when
         *    the socket was created.
         * @param aFxn: Type of socket function call.
         * @param aEvent: Function completion event.  Will be
         *    EPVSocketSuccess, EPVSocketTimeout, or EPVSocketFailure.
         * @param aError: When the event is EPVSocketFailure, this
         *    may contain a platform-specific error code, or zero if
         *    none is available.
         */
        OSCL_IMPORT_REF virtual void HandleSocketEvent(int32 aId, TPVSocketFxn aFxn, TPVSocketEvent aEvent, int32 aError) = 0;
        virtual ~OsclSocketObserver() {}
};


#endif
