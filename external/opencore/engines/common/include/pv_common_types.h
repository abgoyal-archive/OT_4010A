
#ifndef PV_COMMON_TYPES_H_INCLUDED
#define PV_COMMON_TYPES_H_INCLUDED

//  INCLUDES
#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

// CONSTANTS

// MACROS
#define PV_COMMON_ASYNC_EVENT_LOCAL_BUF_SIZE 8

// DATA TYPES
/* Temporary definitions */
typedef int32 TPVCmnCommandType;
typedef int32 TPVCmnCommandId;
typedef int32 TPVCmnCommandStatus;
typedef int32 TPVCmnEventType;
typedef void* TPVCmnExclusivePtr;
typedef void* TPVCmnInterfacePtr;
typedef int32 TPVCmnResponseType;
typedef int32 TPVCmnSDKModuleInfo;
typedef uint8* TPVCmnMIMEType;
typedef uint32 TPVCmnUUID;
typedef int32 CPVCmnVideoCaps;
typedef int32 CPVCmnVideoPrefs;
typedef int32 CPVCmnAudioCaps;
typedef int32 CPVCmnAudioPrefs;

struct TPVCmnSDKInfo
{
    TPVCmnSDKInfo()
    {
        iDate = 0x00000000;
    }
    TPVCmnSDKInfo& operator=(const TPVCmnSDKInfo& aSDKInfo)
    {
        iLabel = aSDKInfo.iLabel;
        iDate = aSDKInfo.iDate;
        return *this;
    }

    OSCL_StackString<80> iLabel;
    uint32 iDate; // 0xyyyymmdd
};

// FUNCTION PROTOTYPES


// FORWARD DECLARATIONS


// CLASS DECLARATION
class CPVCmnInterfaceObserverMessage
{
    public:
        CPVCmnInterfaceObserverMessage() {}
        CPVCmnInterfaceObserverMessage(TPVCmnResponseType aResponseType): iResponseType(aResponseType) {}

        virtual ~CPVCmnInterfaceObserverMessage() {};
        TPVCmnResponseType GetResponseType() const
        {
            return iResponseType;
        }
        virtual int GetPriority()const
        {
            return iPriority;
        }
        TPVCmnResponseType iResponseType;
        int iPriority;
        int iOrder;
};

class CPVCmnInterfaceObserverMessageCompare
{
    public:
        int compare(CPVCmnInterfaceObserverMessage *a, CPVCmnInterfaceObserverMessage *b) const
        {
            if (a->GetPriority() < b->GetPriority())
                return 1;//prioritized
            else
                return (a->iOrder > b->iOrder);//fifo
        }
};

class CPVCmnInterfaceCmdMessage;
class CPVCmnCmdResp : public CPVCmnInterfaceObserverMessage
{
    public:
        /**
         * Constructor for CPVCmnCmdResp
         */
        CPVCmnCmdResp(TPVCmnCommandType aType, TPVCmnCommandId aId, void* aContext, TPVCmnCommandStatus aStatus, void* aResponseData = NULL, int aResponseDataSize = 0, TPVCmnResponseType aResponseType = NULL):
                CPVCmnInterfaceObserverMessage(aResponseType), iCmdType(aType), iCmdId(aId), iContext(aContext), iStatus(aStatus), iResponseData(aResponseData), iResponseDataSize(aResponseDataSize) {}

        /**
         * @return Returns the command type that is being completed.
         */
        TPVCmnCommandType GetCmdType()const
        {
            return iCmdType;
        }

        /**
         * @return Returns the unique ID associated with a command of this type.
         */
        TPVCmnCommandId GetCmdId()const
        {
            return iCmdId;
        }

        /**
         * @return Returns the opaque data that was passed in with the command.
         */
        void* GetContext()const
        {
            return iContext;
        }

        /**
         * @return Returns the completion status of the command
         */
        TPVCmnCommandStatus GetCmdStatus()const
        {
            return iStatus;
        }

        /**
         * @return Returns additional data asociated with the command.  This is to be interpreted
                   based on the command type and the return status
         */
        void* GetResponseData()const
        {
            return iResponseData;
        }
        int GetResponseDataSize()const
        {
            return iResponseDataSize;
        }

    protected:
        TPVCmnCommandType iCmdType;
        TPVCmnCommandId iCmdId;
        void* iContext;
        TPVCmnCommandStatus iStatus;
        void* iResponseData;
        int iResponseDataSize;
};


class CPVCmnAsyncEvent : public CPVCmnInterfaceObserverMessage
{
    public:
        CPVCmnAsyncEvent(TPVCmnEventType aEventType, TPVCmnExclusivePtr aExclusivePtr, const uint8 *aLocalBuffer = NULL, uint32 aLocalBufSize = 0, TPVCmnResponseType aResponseType = NULL) :
                CPVCmnInterfaceObserverMessage(aResponseType),  iEventType(aEventType), iExclusivePtr(aExclusivePtr)
        {
            if (aLocalBuffer)
            {
                if (aLocalBufSize > PV_COMMON_ASYNC_EVENT_LOCAL_BUF_SIZE)
                {
                    oscl_memcpy(iLocalBuffer, aLocalBuffer, PV_COMMON_ASYNC_EVENT_LOCAL_BUF_SIZE);
                }
                else
                {
                    oscl_memcpy(iLocalBuffer, aLocalBuffer, aLocalBufSize);
                }
            }
        }

        ~CPVCmnAsyncEvent() {}

        /**
         * @return Returns the Event type that has been received
         */
        TPVCmnEventType GetEventType()const
        {
            return iEventType;
        }

        /**
         * @return Returns the opaque data asociated with the event.
         */
        void GetEventData(TPVCmnExclusivePtr& aPtr)const
        {
            aPtr = iExclusivePtr;
        }

        /**
         * @return Returns the local data asociated with the event.
         */
        uint8 * GetLocalBuffer()
        {
            return &iLocalBuffer[0];
        }

    protected:
        TPVCmnEventType iEventType;
        TPVCmnExclusivePtr iExclusivePtr;
        uint8 iLocalBuffer[PV_COMMON_ASYNC_EVENT_LOCAL_BUF_SIZE];
};

typedef CPVCmnAsyncEvent CPVCmnAsyncInfoEvent;

typedef CPVCmnAsyncEvent CPVCmnAsyncErrorEvent;

class MPVCmnErrorEventObserver
{
    public:
        virtual ~MPVCmnErrorEventObserver() {}
        virtual void HandleErrorEventL(const CPVCmnAsyncErrorEvent& aEvent) = 0;
};

class MPVCmnInfoEventObserver
{
    public:
        virtual ~MPVCmnInfoEventObserver() {}
        virtual void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent) = 0;
};

class MPVCmnCmdStatusObserver
{
    public:
        virtual ~MPVCmnCmdStatusObserver() {}
        virtual void CommandCompletedL(const CPVCmnCmdResp& aResponse) = 0;
};

#endif //


