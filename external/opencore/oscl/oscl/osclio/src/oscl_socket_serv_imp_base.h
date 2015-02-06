

#ifndef OSCL_SOCKET_SERV_IMP_BASE_H_INCLUDED
#define OSCL_SOCKET_SERV_IMP_BASE_H_INCLUDED

#include "oscl_base.h"
#include "oscl_socket_stats.h"

class PVLogger;


class OsclSocketServIBase
{
    public:
        virtual ~OsclSocketServIBase()
        {}

        virtual int32 Connect(uint32 aMessageSlots) = 0;
        virtual void Close(bool) = 0;

    protected:
        OsclSocketServIBase(Oscl_DefAlloc &a)
                : iAlloc(a)
        {
            iServError = 0;
            iServState = ESocketServ_Idle;
            iLogger = NULL;
        }

        Oscl_DefAlloc &iAlloc;

        //server state
        enum TSocketServState
        {
            ESocketServ_Idle
            , ESocketServ_Connected
            , ESocketServ_Error
        };
        TSocketServState iServState;

        TSocketServState State()const
        {
            return iServState;
        }

        bool IsServConnected()const
        {
            return iServState == ESocketServ_Connected;
        }
        int iServError;

#if(PV_OSCL_SOCKET_STATS_LOGGING)
        //server stats
        OsclSocketServStats iServStats;
#endif

    public:
        PVLogger* iLogger;
};



#endif



