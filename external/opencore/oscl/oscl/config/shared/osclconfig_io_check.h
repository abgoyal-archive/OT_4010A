
#ifndef OSCLCONFIG_IO_CHECK_H_INCLUDED
#define OSCLCONFIG_IO_CHECK_H_INCLUDED

#ifndef OSCL_HAS_ANSI_FILE_IO_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_FILE_IO_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_COMPATIBLE_IO_FUNCTION
#error "ERROR: OSCL_HAS_SYMBIAN_COMPATIBLE_IO_FUNCTION has to be defined to either 1 or 0"
#endif

#if (OSCL_HAS_SYMBIAN_COMPATIBLE_IO_FUNCTION)
#ifndef OSCL_HAS_NATIVE_DUPLICATE_FILE_HANDLE
#error "ERROR: OSCL_HAS_NATIVE_DUPLICATE_FILE_HANDLE has to be defined to either 1 or 0"
#endif
#endif


#ifndef OSCL_HAS_NATIVE_FILE_CACHE_ENABLE
#error "ERROR: OSCL_HAS_NATIVE_FILE_CACHE_ENABLE has to be defined to either 1 or 0"
#endif


#ifndef OSCL_HAS_PV_FILE_CACHE
#error "ERROR: OSCL_HAS_PV_FILE_CACHE has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_LARGE_FILE_SUPPORT
#error "ERROR: OSCL_HAS_LARGE_FILE_SUPPORT has to be defined to either 1 or 0"
#endif

typedef TOsclFileOffset __verify__TOsclFileOffset__defined__;


#ifndef OSCL_FILE_BUFFER_MAX_SIZE
#error "ERROR: OSCL_FILE_BUFFER_MAX_SIZE has to be defined to a numeric value"
#endif

#ifndef OSCL_HAS_SOCKET_SUPPORT
#error "ERROR: OSCL_HAS_SOCKET_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_SOCKET_SERVER
#error "ERROR: OSCL_HAS_SYMBIAN_SOCKET_SERVER has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_DNS_SERVER
#error "ERROR: OSCL_HAS_SYMBIAN_DNS_SERVER has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_BERKELEY_SOCKETS
#error "ERROR: OSCL_HAS_BERKELEY_SOCKETS has to be defined to either 1 or 0"
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
typedef TOsclSocket __TOsclSocketCheck___;
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
typedef TOsclSockAddr __TOsclSockAddrCheck___;
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
typedef TOsclSockAddrLen __TOsclSockAddrLenCheck___;
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclBind
#error "ERROR: OsclBind(s,addr,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclListen
#error "ERROR: OsclListen(s,size,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclAccept
#error "ERROR: OsclAccept(s,accept_s,ok,err,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSetNonBlocking
#error "ERROR: OsclSetNonBlocking(s,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclShutdown
#error "ERROR: OsclShutdown(s,how,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSocket
#error "ERROR: OsclSocket(s,fam,type,prot,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSendTo
#error "ERROR: OsclSendTo(s,buf,len,flags,addr,ok,err,nbytes,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSend
#error "ERROR: OsclSend(s,buf,len,ok,err,nbytes,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclCloseSocket
#error "ERROR: OsclCloseSocket(s,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclConnect
#error "ERROR: OsclConnect(s,addr,ok,err,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclConnectComplete
#error "ERROR: OsclConnectComplete(s,wset,eset,success,fail,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclRecv
#error "ERROR: OsclRecv(s,buf,len,ok,err,nbytes,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclRecvFrom
#error "ERROR: OsclRecvFrom(s,buf,len,paddr,paddrlen,ok,err,nbytes,wouldblock) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSocketSelect
#error "ERROR: OsclSocketSelect(nfds,rd,wr,ex,timeout,ok,err,nhandles) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSocketStartup
#error "ERROR: OsclSocketStartup(ok) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclSocketCleanup
#error "ERROR: OsclSocketCleanup(ok) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclGetAsyncSockErr
#error "ERROR: OsclGetAsyncSockErr(s,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
typedef TOsclHostent __TOsclHostentCheck___;
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclGethostbyname
#error "ERROR: OsclGethostbyname(name,hostent,ok,err) has to be defined"
#endif
#endif

#if OSCL_HAS_BERKELEY_SOCKETS
#ifndef OsclGetDottedAddr
#error "ERROR: OsclGetDottedAddr(hostent,dottedaddr,ok) has to be defined"
#endif
#endif



#if OSCL_HAS_SELECTABLE_PIPES
#ifndef OsclPipe
#error "ERROR: OsclPipe(pipearray) has to be defined"
#endif
#endif

#if OSCL_HAS_SELECTABLE_PIPES
#ifndef OsclReadFD
#error "ERROR: OsclReadFD(fd,buffer,cnt) has to be defined"
#endif
#endif

#if OSCL_HAS_SELECTABLE_PIPES
#ifndef OsclWriteFD
#error "ERROR: OsclWriteFD(fd,buffer,cnt) has to be defined"
#endif
#endif

#if OSCL_HAS_SOCKET_SUPPORT
#ifndef OsclValidInetAddr
#error "ERROR: OsclValidInetAddr(addr) must be defined"
#endif
#endif

#if OSCL_HAS_SOCKET_SUPPORT
#ifndef OSCL_SD_RECEIVE
#error "ERROR: OSCL_SD_RECEIVE has to be defined"
#endif
#ifndef OSCL_SD_SEND
#error "ERROR: OSCL_SD_SEND has to be defined"
#endif
#ifndef OSCL_SD_BOTH
#error "ERROR: OSCL_SD_BOTH has to be defined"
#endif
#endif

#if OSCL_HAS_SOCKET_SUPPORT
#ifndef OSCL_AF_INET
#error "ERROR: OSCL_AF_INET has to be defined"
#endif
#endif

#if OSCL_HAS_SOCKET_SUPPORT
#ifndef OSCL_SOCK_STREAM
#error "ERROR: OSCL_SOCK_STREAM has to be defined"
#endif
#ifndef OSCL_SOCK_DATAGRAM
#error "ERROR: OSCL_SOCK_DATAGRAM has to be defined"
#endif
#endif

#if OSCL_HAS_SOCKET_SUPPORT
#ifndef OSCL_IPPROTO_TCP
#error "ERROR: OSCL_IPPROTO_TCP has to be defined"
#endif
#ifndef OSCL_IPPROTO_UDP
#error "ERROR: OSCL_IPPROTO_UDP has to be defined"
#endif
#endif


#endif // OSCLCONFIG_IO_CHECK_H_INCLUDED


