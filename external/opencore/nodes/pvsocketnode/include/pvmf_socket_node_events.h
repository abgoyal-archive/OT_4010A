
#ifndef PVMF_SOCKET_NODE_EVENTS_H_INCLUDED
#define PVMF_SOCKET_NODE_EVENTS_H_INCLUDED

#define PVMFSocketNodeEventTypeUUID PVUuid(0xbe846567,0xae17,0x44c8,0x9c,0x9a,0x87,0xb8,0x33,0xa0,0xf9,0x1d)

/* Debug macros*/
#define ENABLE_SOCKET_NODE_DEBUG_INFO_EVENT 0

typedef enum
{
    PVMFSocketNodeErrorEventStart = 1024,
    PVMFSocketNodeErrorSocketServerCreateError,
    PVMFSocketNodeErrorSocketServConnectError,
    PVMFSocketNodeErrorUDPSocketRecvError,
    PVMFSocketNodeErrorTCPSocketRecvError,
    PVMFSocketNodeErrorUDPSocketSendError,
    PVMFSocketNodeErrorTCPSocketSendError,
    PVMFSocketNodeError_TCPSocketConnect,
    PVMFSocketNodeError_DNSLookup,
    PVMFSocketNodeErrorSocketFailure,
    PVMFSocketNodeErrorSocketTimeOut,
    PVMFSocketNodeErrorInvalidPortTag,
    PVMFSocketNodeErrorInvalidPortConfig,
    PVMFSocketNodeErrorConnectCancelled,
    PVMFSocketNodeErrorDNSCancelled,
    PVMFSocketNodeErrorEventEnd
} PVMFSocketNodeErrorEventType;

typedef enum
{
    PVMFSocketNodeInfoEventStart = 8192,
    PVMFSocketNodeInfoEventPacketTruncated,

    PVMFSocketNodeInfoEventRequestedDNSResolution,
    PVMFSocketNodeInfoEventConnectRequestPending,

    PVMFSocketNodeInfoEventEnd
} PVMFSocketNodeInfoEventType;

#endif //PVMF_SOCKET_NODE_EVENTS_H_INCLUDED


