
#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#define PVMF_NODE_INTERFACE_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif
#ifndef PVMF_EVENT_HANDLING_H_INCLUDED
#include "pvmf_event_handling.h"
#endif
#ifndef PVMF_PORT_INTERFACE_H_INCLUDED
#include "pvmf_port_interface.h"
#endif
#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif
#ifndef PV_INTERFACE_H_INCLUDED
#include "pv_interface.h"
#endif
#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

typedef struct
{
    // PVMFPortType iInputCapability;
    Oscl_Vector<PVMFFormatType, OsclMemAllocator> iInputFormatCapability;
    // PVMFPortType iOutputCapability;
    Oscl_Vector<PVMFFormatType, OsclMemAllocator> iOutputFormatCapability;
    bool iCanSupportMultipleOutputPorts;
    bool iCanSupportMultipleInputPorts;
    bool iHasMaxNumberOfPorts;
    int32 iMaxNumberOfPorts;
} PVMFNodeCapability;


#define PVMF_NODE_INFO_EVENT_LAST 4096
#define PVMF_NODE_ERROR_EVENT_LAST 8192


typedef enum
{
    EPVMFNodeCreated
    , EPVMFNodeIdle
    , EPVMFNodeInitialized
    , EPVMFNodePrepared
    , EPVMFNodeStarted
    , EPVMFNodePaused
    , EPVMFNodeError
    , EPVMFNodeLastState // derived nodes can add more states as needed
} TPVMFNodeInterfaceState;

class OsclSharedLibrary;

class PVMFNodeErrorEventObserver
{
    public:
        /**
         * Handle an error event that has been generated.
         *
         * @param "aEvent" "The event to be handled."
         */
        virtual void HandleNodeErrorEvent(const PVMFAsyncEvent& aEvent) = 0;

        virtual ~PVMFNodeErrorEventObserver() {}
};

class PVMFNodeInfoEventObserver
{
    public:
        /**
         * Handle an informational event that has been generated.
         *
         * @param "aEvent" "The event to be handled."
         */
        virtual void HandleNodeInformationalEvent(const PVMFAsyncEvent& aEvent) = 0;

        virtual ~PVMFNodeInfoEventObserver() {}
};


class PVMFNodeCmdStatusObserver
{
    public:
        /**
           Handle an event that has been generated.

           @param "aResponse"   "The response to a previously issued command."
        */
        virtual void NodeCommandCompleted(const PVMFCmdResp& aResponse) = 0;

        virtual ~PVMFNodeCmdStatusObserver() {}
};


class PVMFNodeInterface;



class PVMFPortIter : public HeapBase
{
    public:
        virtual uint16 NumPorts() = 0;
        virtual PVMFPortInterface* GetNext() = 0;
        virtual void Reset() = 0;
        virtual ~PVMFPortIter() {}
};

class PVMFPortFilter
{
    public:
        PVMFPortFilter() {};
};

class PVMFNodeSessionInfo
{
    public:
        PVMFNodeSessionInfo()
                : iCmdStatusObserver(NULL)
                , iInfoObserver(NULL)
                , iErrorObserver(NULL)
                , iInfoContext(NULL)
                , iErrorContext(NULL)
        {}
        PVMFNodeSessionInfo(PVMFNodeCmdStatusObserver*c,
                            PVMFNodeInfoEventObserver*i,
                            OsclAny* ic,
                            PVMFNodeErrorEventObserver*e,
                            OsclAny* ec)
                : iCmdStatusObserver(c)
                , iInfoObserver(i)
                , iErrorObserver(e)
                , iInfoContext(ic)
                , iErrorContext(ec)
        {}
        PVMFNodeCmdStatusObserver* iCmdStatusObserver;
        PVMFNodeInfoEventObserver* iInfoObserver;
        PVMFNodeErrorEventObserver* iErrorObserver;
        OsclAny* iInfoContext;
        OsclAny* iErrorContext;
};

#define PVMF_NODE_DEFAULT_SESSION_RESERVE 10

class PVMFNodeSession
{
    public:
        PVMFSessionId iId;
        PVMFNodeSessionInfo iInfo;
};

class PVMFPortActivity;
class PVMFPortActivityHandler
{
    public:
        virtual ~PVMFPortActivityHandler() {}
        virtual void HandlePortActivity(const PVMFPortActivity &) = 0;
};

class OSCL_IMPORT_REF PVMFNodeInterface: public PVMFPortActivityHandler
{
    public:

        virtual ~PVMFNodeInterface()
        {
            iSessions.clear();
        }

        virtual PVMFStatus ThreadLogon() = 0;
        virtual PVMFStatus ThreadLogoff() = 0;

        virtual PVMFSessionId Connect(const PVMFNodeSessionInfo &aSession)
        {
            PVMFNodeSession session;
            session.iId = iSessions.size();
            session.iInfo = aSession;
            iSessions.push_back(session);
            return session.iId;
        }

        virtual PVMFStatus Disconnect(PVMFSessionId aSessionId)
        {
            for (uint32 i = 0; i < iSessions.size(); i++)
            {
                if (iSessions[i].iId == aSessionId)
                {
                    iSessions.erase(&iSessions[i]);
                    return PVMFSuccess;
                }
            }
            return PVMFFailure;
        }

        /**
           GetCapability can be invoked only when after a node is initialized
        **/
        virtual PVMFStatus GetCapability(PVMFNodeCapability& aNodeCapability) = 0;


        /**
         * Returns a list of ports currently available in the node that
         * meet the filter criteria We can add fancier iterators and
         * filters as needed.
         * For now we return all the available ports.  If no ports are
         * present, NULL is returned
         **/
        virtual PVMFPortIter* GetPorts(const PVMFPortFilter* aFilter = NULL) = 0;

        /**
         * Retrieves state information of this node
         **/
        virtual TPVMFNodeInterfaceState GetState()
        {
            return iInterfaceState;
        }

        /**
         * This API is to allow for extensibility of the PVMF Node interface.
         * It allows a caller to ask for all UUIDs associated with a
         * particular MIME type.  If interfaces of the requested MIME type
         * are found within the system, they are added to the UUIDs array.
         *
         * Also added to the UUIDs array will be all interfaces which have
         * the requested MIME type as a base MIME type.  This
         * functionality can be turned off.
         *
         * @param aMimeType The MIME type of the desired interfaces
         * @param aUuids A vector to hold the discovered UUIDs
         * @param aExactUuidsOnly Turns on/off the retrival of UUIDs with
         *                        aMimeType as a base type
         * @param aContext Optional opaque data to be passed back to user
         *                 with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVMFCommandId QueryUUID(PVMFSessionId aSession
                                        , const PvmfMimeString& aMimeType
                                        , Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids
                                        , bool aExactUuidsOnly = false
                                                                 , const OsclAny* aContext = NULL) = 0;

        /**
         * This API is to allow for extensibility of the PVMF Node interface.
         * It allows a caller to ask for an instance of a particular
         * interface object to be returned.  The mechanism is analogous to
         * the COM IUnknown method.  The interfaces are identified with an
         * interface ID that is a UUID as in DCE and a pointer to the
         * interface object is returned if it is supported.  Otherwise the
         * returned pointer is NULL.
         *
         * @param aUuid The UUID of the desired interface
         * @param aInterfacePtr The output pointer to the desired interface
         * @param aContext Optional opaque data to be passed back to user
         *                 with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVMFCommandId QueryInterface(PVMFSessionId aSession
                                             , const PVUuid& aUuid
                                             , PVInterface*& aInterfacePtr
                                             , const OsclAny* aContext = NULL) = 0;

        /**
         * Requests the node to return a port meeting certain criteria for
         * format types and buffering capabilities.  The node may return a
         * reference to an already created unused port or it may
         * dynamically create one if it has the capability to do so.
         * Since there might be some port specific initializations that
         * might need to be done for ports created on demand, it will be
         * most flexible to have this as an asynchronous API.
         *
         * A reference to the port interface is returned with the the
         * command completion.  It is passed as an auto ptr carrying
         * opaque data that needs to be cast to PVMFPortInterface*
         * @exception PVMFErrNotSupported leaves if this is not supported.
         **/
        virtual PVMFCommandId RequestPort(PVMFSessionId aSession
                                          , int32 aPortTag
                                          , const PvmfMimeString* aPortConfig = NULL
                                                                                , const OsclAny* aContext = NULL) = 0;

        /**
         * Releases a port back to the owning node.
         * @exception PVMFErrArgument leaves if this node is not the owner.
         **/
        virtual PVMFCommandId ReleasePort(PVMFSessionId aSession
                                          , PVMFPortInterface& aPort
                                          , const OsclAny* aContext = NULL) = 0;

        /**
         * Starts initialization of the node.  At the minimum, the node
         * should be ready to advertize its capabilities after
         * initialization is complete
         **/
        virtual PVMFCommandId Init(PVMFSessionId aSession
                                   , const OsclAny* aContext = NULL) = 0;

        /**
         * Starts preparation of the node.
         * Node should be ready to Start after the Prepare is
         * complete.
         **/
        virtual PVMFCommandId Prepare(PVMFSessionId aSession
                                      , const OsclAny* aContext = NULL) = 0;

        /**
         * Causes the node to start servicing all connected ports.
         **/
        virtual PVMFCommandId Start(PVMFSessionId aSession
                                    , const OsclAny* aContext = NULL) = 0;

        /**
         * Causes the node to stop servicing all connected ports and
         * discard any un-processed data.
         **/
        virtual PVMFCommandId Stop(PVMFSessionId aSession
                                   , const OsclAny* aContext = NULL) = 0;

        /**
         * Causes the node to stop servicing all connected ports as
         * soon as current data is processed.
         **/
        virtual PVMFCommandId Flush(PVMFSessionId aSession
                                    , const OsclAny* aContext = NULL) = 0;

        /**
         * Causes the node to pause servicing all connected ports without
         * discarding un-processed data.
         **/
        virtual PVMFCommandId Pause(PVMFSessionId aSession
                                    , const OsclAny* aContext = NULL) = 0;

        /**
         * Resets the node. The node should relinquish all resources that
         * is has acquired as part of the initialization process and
         * should be ready to be deleted when this completes.
         **/
        virtual PVMFCommandId Reset(PVMFSessionId aSession
                                    , const OsclAny* aContext = NULL) = 0;

        /**
         * Cancel all pending requests. The current request being
         * processed, if any, will also be aborted.
         *
         * @param aContextData Optional opaque data that will be passed
         *                     back to the user with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVMFCommandId CancelAllCommands(PVMFSessionId aSession
                                                , const OsclAny* aContextData = NULL) = 0;

        /**
         * Cancels pending command with the specified ID.
         *
         * @param aCmdId Command Id of the command to be cancelled
         * @param aContextData Optional opaque data that will be passed
         *                     back to the user with the command response
         * @returns A unique command id for asynchronous completion
         */
        virtual PVMFCommandId CancelCommand(PVMFSessionId aSession
                                            , PVMFCommandId aCmdId
                                            , const OsclAny* aContextData = NULL) = 0;

        /**
         * Ports call this API to report activity to the node.
         *
         * @param aActivity Information regarding the activity.
         */
        virtual void HandlePortActivity(const PVMFPortActivity& aActivity) = 0;

        /**
         * This API is a synchronous version of QueryInterface.
         * The mechanism is analogous to the COM IUnknown method.  The
         * interfaces are identified with an interface ID that is a UUID
         * as in DCE and a pointer to the interface object is returned if
         * it is supported.  Otherwise the returned pointer is NULL.
         *
         * @param aUuid The UUID of the desired interface
         * @param aInterfacePtr The output pointer to the desired interface
         * @returns PVMFSuccess or PVMFErrNotSupported
         */
        virtual PVMFStatus QueryInterfaceSync(PVMFSessionId aSession
                                              , const PVUuid& aUuid
                                              , PVInterface*& aInterfacePtr)
        {
            return PVMFErrNotImplemented;
        }


        /**
         * Set shared library pointer
         * @param aPtr Pointer to the shared library.
         **/
        virtual void SetSharedLibraryPtr(OsclSharedLibrary* aPtr)
        {
            iOsclSharedLibrary = aPtr;
        }

        /**
         * Retrieves shared library pointer
         * @returns Pointer to the shared library.
         **/
        virtual OsclSharedLibrary* GetSharedLibraryPtr()
        {
            return iOsclSharedLibrary;
        }

    protected:
        PVMFNodeInterface(int32 aSessionReserve = PVMF_NODE_DEFAULT_SESSION_RESERVE):
                iInterfaceState(EPVMFNodeCreated)
                , iOsclSharedLibrary(NULL)
                , iPlayerDriverUserData(NULL)
        {
            iSessions.reserve(aSessionReserve);
        }

        Oscl_Vector<PVMFNodeSession, OsclMemAllocator> iSessions;
        TPVMFNodeInterfaceState iInterfaceState;

        OsclSharedLibrary* iOsclSharedLibrary;

        /** This method can be used to update the state and
         ** notify observers of the state change event.
         */
        OSCL_IMPORT_REF virtual void SetState(TPVMFNodeInterfaceState);

        /* For the given session id, forward the command response if an
         * observer exists. No-op if the session id is bad or no command
         * complete observer exists on that session.
         *
         * @param session_id Created when the user who should receive this
         *                   event connected to that node.
         * @param resp Command complete event.
         */
        OSCL_IMPORT_REF virtual void ReportCmdCompleteEvent(PVMFSessionId session_id,
                const PVMFCmdResp &resp);

        /* For each session handled by the node, if an appropriate
         * observer exists (info, error) a copy of the event is
         * dispatched with a copy of the session info/error context.
         * No-op if the event category is wrong.
         *
         * @param event To be reported to the session(s) observer(s) for
         *              the event's category.
         */
        OSCL_IMPORT_REF virtual void ReportErrorEvent(const PVMFAsyncEvent& aEvent);
        OSCL_IMPORT_REF virtual void ReportInfoEvent(const PVMFAsyncEvent& aEvent);

        /* Similar to the above except the event is built first.
         * TODO: Get rid of these. Callers should build the event object
         * including the pointer to their interfaces.
         */
        OSCL_IMPORT_REF virtual void ReportErrorEvent(PVMFEventType aEventType,
                void* aEventData = NULL,
                PVInterface*aExtMsg = NULL);
        OSCL_IMPORT_REF virtual void ReportInfoEvent(PVMFEventType aEventType,
                void* aEventData = NULL,
                PVInterface*aExtMsg = NULL);

protected:		
        PVMFNodeInterface* mSinkNode;

        uint32* iPlayerDriverUserData;
     
public:
        virtual PVMFStatus ResetNodeParam()
        {
            return ClearBuffer();
        };

        virtual PVMFStatus ClearBuffer()
        {
            return PVMFErrNotSupported;
        };

        virtual void SetAVSyncCtrl(PVMFAVSyncbase* aAVSyncCtrl){};
        virtual void SetSinkNode(PVMFNodeInterface* aSinkNode){}

        virtual void SetPlayerDriverUserData(uint32* pUserData) { iPlayerDriverUserData = pUserData; }
        virtual uint32* GetPlayerDriverUserData() { return iPlayerDriverUserData; }
};

#endif
