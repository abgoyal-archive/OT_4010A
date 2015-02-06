


#ifndef OSCL_SOCKET_STATS_H_INCLUDED
#define OSCL_SOCKET_STATS_H_INCLUDED

#include "oscl_base.h"
#include "oscl_vector.h"
#include "oscl_mem.h"
#include "oscl_mutex.h"

enum TOsclSocketStatEvent
{
    //Request/Method AO events
    EOsclSocket_RequestAO_Success
    , EOsclSocket_RequestAO_Canceled
    , EOsclSocket_RequestAO_Error
    , EOsclSocket_RequestAO_Timeout
    //Server events
    , EOsclSocket_ServRequestIssued
    , EOsclSocket_ServPoll
    , EOsclSocket_OS
    , EOsclSocket_Readable
    , EOsclSocket_Writable
    , EOsclSocket_Except
    , EOsclSocket_DataRecv
    , EOsclSocket_DataSent
    , EOsclSocket_ServRequestComplete
    , EOsclSocket_ServRequestCancelIssued
    //loop socket events
    , EOsclSocketServ_LoopsockOk
    , EOsclSocketServ_LoopsockError
};
static const char* const TPVSocketStatStr[] =
{
    //Request/Method AO events
    "AO_Success"
    , "AO_Canceled"
    , "AO_Error"
    , "AO_Timeout"
    //Server events
    , "Serv_ReqIssued"
    , "Serv_Poll"
    , "Serv_OS_Op"
    , "Serv_Readable"
    , "Serv_Writeable"
    , "Serv_Except!"
    , "Serv_DataRecv'd"
    , "Serv_DataSent"
    , "Serv_ReqComplete"
    , "Serv_ReqCancel"
    //loop socket events
    , "Loopsock_OK"
    , "Loopsock_Err"
} ;

enum TOsclSocketServStatEvent
{
    //select loop events
    EOsclSocketServ_SelectNoActivity = 0
    , EOsclSocketServ_SelectActivity
    , EOsclSocketServ_SelectRescheduleAsap
    , EOsclSocketServ_SelectReschedulePoll
    , EOsclSocketServ_LastEvent
};
static const char* const TPVSocketServStatStr[] =
{
    "Select_No"
    , "Select_Yes"
    , "Select_Resched_Asap"
    , "Select_Resched_Poll"
} ;

#include "oscl_socket_tuneables.h"
#if(PV_OSCL_SOCKET_STATS_LOGGING)

class PVLogger;
class OsclSocketStatEventEntry
{
    public:
        OsclSocketStatEventEntry(TOsclSocketStatEvent aEvent, uint32 aTime, int32 aParam)
                : iEvent(aEvent)
                , iTicks(aTime)
                , iParam(aParam)
        {}
        TOsclSocketStatEvent iEvent;
        uint32 iTicks;
        int32 iParam;
};

#include "oscl_socket_types.h"

#define OsclSocketStats_NUM_ACTIVE_REQUESTS EPVSocket_Last

//Define the maximum number of polling events that will be logged.
//Additional poll events will be counted but not logged individually.
#define OsclSocketStats_MAX_POLL_EVENTS 100

class OsclSocketStats
{
    public:
        OsclSocketStats();
        ~OsclSocketStats();
        void ShowTuneables();
        void Add(TPVSocketFxn, TOsclSocketStatEvent, int32 aParam = (-1));
        void Log(TPVSocketFxn);
        void Log();
        void Clear(TPVSocketFxn);
        void Construct(OsclAny* aContainer, OsclAny*aServ);
        void LogAndDump();
    private:
        Oscl_Vector<OsclSocketStatEventEntry, OsclMemAllocator> iEvents[OsclSocketStats_NUM_ACTIVE_REQUESTS];
        Oscl_Vector<OsclSocketStatEventEntry, OsclMemAllocator> iPollEvents[OsclSocketStats_NUM_ACTIVE_REQUESTS];
        int32 iExcessPollEvents[OsclSocketStats_NUM_ACTIVE_REQUESTS];
        int32 iLastCompletionTime[OsclSocketStats_NUM_ACTIVE_REQUESTS];
        OsclThreadLock iLock;
        char* iLogStr;
        PVLogger* iLogger;
        OsclAny* iSock;
        OsclAny* iServ;
};

class OsclSocketServStats
{
    public:
        OsclSocketServStats();
        ~OsclSocketServStats();
        void Add(TOsclSocketServStatEvent);
        void Construct(OsclAny*aServ);
        void LogAndDump();
    private:
        void Log(TOsclSocketServStatEvent);
        void Clear();
        void ShowTuneables();
        uint32 iEvents[EOsclSocketServ_LastEvent];
        PVLogger* iLogger;
        OsclAny* iServ;
};

#endif
#endif



