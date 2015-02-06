

#include "oscl_socket_imp_base.h"

OsclSocketIBase::OsclSocketIBase(Oscl_DefAlloc &a): iAlloc(a)
{}

OsclSocketIBase::~OsclSocketIBase()
{
}

void OsclSocketIBase::CancelFxn(TPVSocketFxn aType)
{
    switch (aType)
    {
        case EPVSocketConnect:
            CancelConnect();
            break;
        case EPVSocketAccept:
            CancelAccept();
            break;
        case EPVSocketShutdown:
            CancelShutdown();
            break;
        case EPVSocketSend:
            CancelSend();
            break;
        case EPVSocketRecv:
            CancelRecv();
            break;
        case EPVSocketSendTo:
            CancelSendTo();
            break;
        case EPVSocketRecvFrom:
            CancelRecvFrom();
            break;
        case EPVSocketBind:
            CancelBind();
            break;
        case EPVSocketListen:
            CancelListen();
            break;
        default:
            OSCL_ASSERT(false);
            break;
    }
}

int OsclSocketIBase::GetShutdown(TPVSocketShutdown aOsclVal)
//map socket API value to platform-specific value.
{
    switch (aOsclVal)
    {
        case EPVSocketRecvShutdown:
            return OSCL_SD_RECEIVE;
        case EPVSocketSendShutdown:
            return OSCL_SD_SEND;
        case EPVSocketBothShutdown:
            return OSCL_SD_BOTH;
        default:
            OSCL_ASSERT(0);
    }
    return 0;
}









