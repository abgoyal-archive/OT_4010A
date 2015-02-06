
#ifndef PVMF_MP4FFPARSER_NODE_H_INCLUDED
#define PVMF_MP4FFPARSER_NODE_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
#endif

#ifndef OSCL_TIMER_H_INCLUDED
#include "oscl_timer.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#ifndef PVLOGGER_FILE_APPENDER_H_INCLUDED
#include "pvlogger_file_appender.h"
#endif

#ifndef OSCL_BIN_STREAM_H_INCLUDED
#include "oscl_bin_stream.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

#ifndef PVMF_NODE_UTILS_H_INCLUDED
#include "pvmf_node_utils.h"
#endif

#ifndef OSCL_PRIQUEUE_H_INCLUDED
#include "oscl_priqueue.h"
#endif

#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif

#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#include "oscl_mem_mempool.h"
#endif

#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif

#ifndef PVMF_MEDIA_FRAG_GROUP_H_INCLUDED
#include "pvmf_media_frag_group.h"
#endif

#ifndef PVMF_MP4FFPARSER_DEFS_H_INCLUDED
//#include "pvmf_mp4ffparser_defs.h"
#endif

#ifndef PVMF_DATA_SOURCE_PLAYBACK_CONTROL_H_INCLUDED
#include "pvmf_data_source_playback_control.h"
#endif

#ifndef PVMF_META_DATA_EXTENSION_H_INCLUDED
#include "pvmf_meta_data_extension.h"
#endif

#ifndef PVMF_DATA_SOURCE_INIT_EXTENSION_H_INCLUDED
#include "pvmf_data_source_init_extension.h"
#endif

#ifndef PVMF_TRACK_SELECTION_EXTENSION_H_INCLUDED
#include "pvmf_track_selection_extension.h"
#endif

#ifndef PVMF_MEDIA_PRESENTATION_INFO_H_INCLUDED
#include "pvmf_media_presentation_info.h"
#endif

#ifndef PVMF_TRACK_LEVEL_INFO_EXTENSION_H_INCLUDED
#include "pvmf_track_level_info_extension.h"
#endif

#ifndef PVMF_MP4_PROGDOWNLOAD_SUPPORT_EXTENSION_H_INCLUDED
#include "pvmf_mp4_progdownload_support_extension.h"
#endif

#ifndef CPM_H_INCLUDED
#include "cpm.h"
#endif

#ifndef PVMF_CPMPLUGIN_ACCESS_INTERFACE_H_INCLUDED
#include "pvmf_cpmplugin_access_interface.h"
#endif

#ifndef PVMF_LOCAL_DATA_SOURCE_H_INCLUDED
#include "pvmf_local_data_source.h"
#endif

#ifndef PVMF_MEMPOOL_H_INCLUDED
#include "pvmf_mempool.h"
#endif

#ifndef PVMF_TIMEDTEXT_H_INCLUDED
#include "pvmf_timedtext.h"
#endif

#ifndef PV_GAU_H_
#include "pv_gau.h"
#endif

#ifndef PVMF_FORMAT_PROGDOWNLOAD_SUPPORT_EXTENSION_H_INCLUDED
#include "pvmf_format_progdownload_support_extension.h"
#endif

#ifndef PVMF_DOWNLOAD_PROGRESS_EXTENSION_H
#include "pvmf_download_progress_interface.h"
#endif

#ifndef PVMI_DATASTREAMUSER_INTERFACE_H_INCLUDED
#include "pvmi_datastreamuser_interface.h"
#endif

#ifndef PVMF_MP4FFPARSER_OUTPORT_H_INCLUDED
//#include "pvmf_asfffparser_outport.h"
#endif

#ifndef PVMF_SOURCE_CONTEXT_DATA_H_INCLUDED
#include "pvmf_source_context_data.h"
#endif

#ifndef PVMF_CPMPLUGIN_LICENSE_INTERFACE_H_INCLUDED
#include "pvmf_cpmplugin_license_interface.h"
#endif

#ifndef PVMI_KVP_UTIL_H_INCLUDED
#include "pvmi_kvp_util.h"
#endif

#ifndef PVMF_BASIC_ERRORINFOMESSAGE_H_INCLUDED
#include "pvmf_basic_errorinfomessage.h"
#endif

#ifndef PVMF_DATA_SOURCE_DIRECTION_CONTROL_H_INCLUDED
#include "pvmf_data_source_direction_control.h"
#endif

#include "pvmf_pmem_alloc.h"

class PVMFASFFFParserNode : public OsclTimerObject,
            public PVMFNodeInterface,
            public PVMFDataSourceInitializationExtensionInterface
#if 0                        
            public PVMFTrackSelectionExtensionInterface
            public PvmfDataSourcePlaybackControlInterface,
            public PVMFMetadataExtensionInterface,
            public PVMFTrackLevelInfoExtensionInterface,
            public PVMFCPMStatusObserver,
            public PvmiDataStreamObserver,
            public PVMIDatastreamuserInterface,
            public PVMFFormatProgDownloadSupportInterface,
            public OsclTimerObserver,
            public PVMFCPMPluginLicenseInterface,
            public PvmiCapabilityAndConfig,
            public PVMFMediaClockStateObserver, // For observing the playback clock states
            public PvmfDataSourceDirectionControlInterface
#endif            
{
    public:
        PVMFASFFFParserNode(int32 aPriority = OsclActiveObject::EPriorityNominal);
        virtual ~PVMFASFFFParserNode();

        // From PVMFNodeInterface
        PVMFStatus ThreadLogon();
        PVMFStatus ThreadLogoff();
        PVMFStatus GetCapability(PVMFNodeCapability& aNodeCapability);
        PVMFPortIter* GetPorts(const PVMFPortFilter* aFilter = NULL);

        PVMFCommandId QueryUUID(PVMFSessionId aSessionId,
                                const PvmfMimeString& aMimeType,
                                Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                bool aExactUuidsOnly = false,
                                const OsclAny* aContext = NULL);
        PVMFCommandId QueryInterface(PVMFSessionId aSessionId,
                                     const PVUuid& aUuid,
                                     PVInterface*& aInterfacePtr,
                                     const OsclAny* aContext = NULL);

        PVMFCommandId RequestPort(PVMFSessionId aSessionId, int32 aPortTag,
                                  const PvmfMimeString* aPortConfig = NULL,
                                  const OsclAny* aContext = NULL);
        PVMFStatus ReleasePort(PVMFSessionId aSessionId, PVMFPortInterface& aPort,
                               const OsclAny* aContext = NULL);

        PVMFCommandId Init(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Prepare(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Start(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Stop(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Flush(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Pause(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);
        PVMFCommandId Reset(PVMFSessionId aSessionId, const OsclAny* aContext = NULL);

        PVMFCommandId CancelAllCommands(PVMFSessionId aSessionId, const OsclAny* aContextData = NULL);
        PVMFCommandId CancelCommand(PVMFSessionId aSessionId, PVMFCommandId aCmdId, const OsclAny* aContextData = NULL);

        PVMFStatus QueryInterfaceSync(PVMFSessionId aSession,
                                      const PVUuid& aUuid,
                                      PVInterface*& aInterfacePtr);
        
        // From PVMFPortActivityHandler
        void HandlePortActivity(const PVMFPortActivity& aActivity); 

        // From PVInterface
        void addRef();
        void removeRef();
        bool queryInterface(const PVUuid& uuid, PVInterface*& iface);

        // From PVMFDataSourceInitializationExtensionInterface
        PVMFStatus SetSourceInitializationData(OSCL_wString& aSourceURL, PVMFFormatType& aSourceFormat, OsclAny* aSourceData);
        PVMFStatus SetClientPlayBackClock(PVMFMediaClock* aClientClock);
        PVMFStatus SetEstimatedServerClock(PVMFMediaClock* aClientClock);

        // From PVMFTrackSelectionExtensionInterface
        PVMFStatus GetMediaPresentationInfo(PVMFMediaPresentationInfo& aInfo);
        PVMFStatus SelectTracks(PVMFMediaPresentationInfo& aInfo);
#if 0        

        /* cap config interface */
        virtual void setObserver(PvmiConfigAndCapabilityCmdObserver* aObserver)
        {
            ciObserver = aObserver;
        }

        virtual PVMFStatus getParametersSync(PvmiMIOSession aSession,
                                             PvmiKeyType aIdentifier,
                                             PvmiKvp*& aParameters,
                                             int& aNumParamElements,
                                             PvmiCapabilityContext aContext);
        virtual PVMFStatus releaseParameters(PvmiMIOSession aSession,
                                             PvmiKvp* aParameters,
                                             int num_elements);
        virtual void createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
        virtual void setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext,
                                          PvmiKvp* aParameters, int num_parameter_elements);
        virtual void DeleteContext(PvmiMIOSession aSession,
                                   PvmiCapabilityContext& aContext);
        virtual void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                       int num_elements, PvmiKvp * & aRet_kvp);
        virtual PVMFCommandId setParametersAsync(PvmiMIOSession aSession,
                PvmiKvp* aParameters,
                int num_elements,
                PvmiKvp*& aRet_kvp,
                OsclAny* context = NULL);
        virtual uint32 getCapabilityMetric(PvmiMIOSession aSession);
        virtual PVMFStatus verifyParametersSync(PvmiMIOSession aSession,
                                                PvmiKvp* aParameters,
                                                int num_elements);

        PVMFStatus GetConfigParameter(PvmiKvp*& aParameters, int& aNumParamElements,
                                      int32 aIndex, PvmiKvpAttr reqattr);
        PVMFStatus VerifyAndSetConfigParameter(int index, PvmiKvp& aParameter, bool set);                

        // From PVMFMetadataExtensionInterface
        uint32 GetNumMetadataKeys(char* aQueryKeyString = NULL);
        uint32 GetNumMetadataValues(PVMFMetadataList& aKeyList);
        uint32 GetNumImotionMetadataValues(PVMFMetadataList& aKeyList);
        PVMFCommandId GetNodeMetadataKeys(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList, uint32 aStartingKeyIndex, int32 aMaxKeyEntries,
                                          char* aQueryKeyString = NULL, const OsclAny* aContextData = NULL);
        PVMFCommandId GetNodeMetadataValues(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList,
                                            Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, int32 aMaxValueEntries, const OsclAny* aContextData = NULL);
        PVMFStatus ReleaseNodeMetadataKeys(PVMFMetadataList& aKeyList, uint32 aStartingKeyIndex, uint32 aEndKeyIndex);
        PVMFStatus ReleaseNodeMetadataValues(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 aStartingValueIndex, uint32 aEndValueIndex);

        // From PvmfDataSourcePlaybackControlInterface
        PVMFCommandId SetDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT,
                                            PVMFTimestamp& aActualMediaDataTS, bool aSeekToSyncPoint = true, uint32 aStreamID = 0, OsclAny* aContext = NULL);
        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT, PVMFTimestamp& aActualNPT,
                                              bool aSeekToSyncPoint = true, OsclAny* aContext = NULL);
        PVMFCommandId QueryDataSourcePosition(PVMFSessionId aSessionId, PVMFTimestamp aTargetNPT,
                                              PVMFTimestamp& aSeekPointBeforeTargetNPT, PVMFTimestamp& aSeekPointAfterTargetNPT,  OsclAny* aContext = NULL, bool aSeekToSyncPoint = true);

        PVMFCommandId SetDataSourceRate(PVMFSessionId aSession, int32 aRate, PVMFTimebase* aTimebase = NULL, OsclAny* aContext = NULL);
        PVMFCommandId SetDataSourceDirection(PVMFSessionId aSessionId, int32 aDirection, PVMFTimestamp& aActualNPT,
                                             PVMFTimestamp& aActualMediaDataTS, PVMFTimebase* aTimebase, OsclAny* aContext);

        // From PVMFTrackLevelInfoExtensionInterface
        PVMFStatus GetAvailableTracks(Oscl_Vector<PVMFTrackInfo, OsclMemAllocator>& aTracks);
        PVMFStatus GetTimestampForSampleNumber(PVMFTrackInfo& aTrackInfo, uint32 aSampleNum, PVMFTimestamp& aTimestamp);
        PVMFStatus GetSampleNumberForTimestamp(PVMFTrackInfo& aTrackInfo, PVMFTimestamp aTimestamp, uint32& aSampleNum);
        PVMFStatus GetNumberOfSyncSamples(PVMFTrackInfo& aTrackInfo, int32& aNumSyncSamples);
        PVMFStatus GetSyncSampleInfo(PVMFTrackInfo& aTrackInfo, PVMFSampleNumTSList& aList, uint32 aStartIndex = 0, int32 aMaxEntries = -1);
        PVMFStatus GetSyncSampleInfo(PVMFSampleNumTSList& aList, PVMFTrackInfo& aTrackInfo, int32 aTargetTimeInMS, uint32 aHowManySamples = 1);
        PVMFStatus GetTimestampForDataPosition(PVMFTrackInfo& aTrackInfo, uint32 aDataPosition, PVMFTimestamp& aTimestamp);
        PVMFStatus GetDataPositionForTimestamp(PVMFTrackInfo& aTrackInfo, PVMFTimestamp aTimestamp, uint32& aDataPosition);        

        // From PVMFCPMStatusObserver
        void CPMCommandCompleted(const PVMFCmdResp& aResponse);

        /* From PVMIDatastreamuserInterface */
        void PassDatastreamFactory(PVMFDataStreamFactory& aFactory,
                                   int32 aFactoryTag,
                                   const PvmfMimeString* aFactoryConfig = NULL);
        void PassDatastreamReadCapacityObserver(PVMFDataStreamReadCapacityObserver* aObserver);


        /* From PVMFFormatProgDownloadSupportInterface */
        int32 convertSizeToTime(uint32 fileSize, uint32& aNPTInMS);
        void setFileSize(const uint32 aFileSize);
        void setDownloadProgressInterface(PVMFDownloadProgressInterface*);
        void playResumeNotification(bool aDownloadComplete);
        void notifyDownloadComplete();

        // From OsclTimer
        void TimeoutOccurred(int32 timerID, int32 timeoutInfo);

        /* From PVMFCPMPluginLicenseInterface */
        PVMFStatus GetLicenseURL(PVMFSessionId aSessionId,
                                 OSCL_wString& aContentName,
                                 OSCL_wString& aLicenseURL)
        {
            OSCL_UNUSED_ARG(aSessionId);
            OSCL_UNUSED_ARG(aContentName);
            OSCL_UNUSED_ARG(aLicenseURL);
            //must use Async method.
            return PVMFErrNotSupported;
        }
        PVMFStatus GetLicenseURL(PVMFSessionId aSessionId,
                                 OSCL_String&  aContentName,
                                 OSCL_String&  aLicenseURL)
        {
            OSCL_UNUSED_ARG(aSessionId);
            OSCL_UNUSED_ARG(aContentName);
            OSCL_UNUSED_ARG(aLicenseURL);
            //must use Async method.
            return PVMFErrNotSupported;
        }

        PVMFCommandId GetLicense(PVMFSessionId aSessionId,
                                 OSCL_wString& aContentName,
                                 OsclAny* aData,
                                 uint32 aDataSize,
                                 int32 aTimeoutMsec,
                                 OsclAny* aContextData) ;

        PVMFCommandId GetLicense(PVMFSessionId aSessionId,
                                 OSCL_String&  aContentName,
                                 OsclAny* aData,
                                 uint32 aDataSize,
                                 int32 aTimeoutMsec,
                                 OsclAny* aContextData);

        PVMFCommandId CancelGetLicense(PVMFSessionId aSessionId
                                       , PVMFCommandId aCmdId
                                       , OsclAny* aContextData);

        PVMFStatus GetLicenseStatus(
            PVMFCPMLicenseStatus& aStatus) ;

        //from PVMFMediaClockStateObserver
        void ClockStateUpdated();
        void NotificationsInterfaceDestroyed();
#endif
    private:
        // from OsclTimerObject
        void Run();
#if 0
        //Command processing
        PVMFCommandId QueueCommandL(PVMFASFFFParserNodeCommand& aCmd);
        void MoveCmdToCurrentQueue(PVMFASFFFParserNodeCommand& aCmd);
        void MoveCmdToCancelQueue(PVMFASFFFParserNodeCommand& aCmd);
        void ProcessCommand();
        void CommandComplete(PVMFASFFFParserNodeCmdQueue& aCmdQueue, PVMFASFFFParserNodeCommand& aCmd, PVMFStatus aStatus, OsclAny* aData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        bool FlushPending();
        PVMFASFFFParserNodeCmdQueue iInputCommands;
        PVMFASFFFParserNodeCmdQueue iCurrentCommand;
        PVMFASFFFParserNodeCmdQueue iCancelCommand;

        // Event reporting
        void ReportASFFFParserErrorEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void ReportASFFFParserInfoEvent(PVMFEventType aEventType, OsclAny* aEventData = NULL, PVUuid* aEventUUID = NULL, int32* aEventCode = NULL);
        void ChangeNodeState(TPVMFNodeInterfaceState aNewState);

        // Node command handlers
        PVMFStatus DoQueryUuid(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoQueryInterface(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoRequestPort(PVMFASFFFParserNodeCommand& aCmd, PVMFPortInterface*&);
        void GetTrackMaxParameters(PVMFFormatType aFormatType, uint32& aMaxDataSize, uint32& aMaxQueueDepth);
        PVMFStatus DoReleasePort(PVMFASFFFParserNodeCommand& aCmd);

        PVMFStatus DoInit(PVMFASFFFParserNodeCommand& aCmd);
        bool ParseASFFile(PVMFASFFFParserNodeCmdQueue& aCmdQ,
                          PVMFASFFFParserNodeCommand& aCmd);
        void PushToAvailableMetadataKeysList(const char* aKeystr, char* aOptionalParam = NULL);
        PVMFStatus InitMetaData();
        PVMFStatus InitImotionMetaData();
        uint32 CountImotionMetaDataKeys();
        int32 CountMetaDataKeys();
        void CompleteInit(PVMFASFFFParserNodeCmdQueue& aCmdQ,
                          PVMFASFFFParserNodeCommand& aCmd);
        void CompleteCancelAfterInit();


        PVMFStatus DoPrepare(PVMFASFFFParserNodeCommand& aCmd);
        void CompletePrepare(PVMFStatus aStatus);
        PVMFStatus DoStart(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoStop(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoFlush(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoPause(PVMFASFFFParserNodeCommand& aCmd);

        PVMFStatus DoReset(PVMFASFFFParserNodeCommand& aCmd);
        void CompleteReset(PVMFASFFFParserNodeCmdQueue& aCmdQ, PVMFASFFFParserNodeCommand& aCmd);

        PVMFStatus DoCancelAllCommands(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoCancelCommand(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoCancelCurrentCommand(PVMFASFFFParserNodeCommand& aCmd);

        // For metadata extention interface
        PVMFStatus DoGetMetadataKeys(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus CompleteGetMetadataKeys(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoGetImotionMetadataValues(PVMFASFFFParserNodeCommand& aCmd, int32 &numentriesadded);
        PVMFStatus DoGetMetadataValues(PVMFASFFFParserNodeCommand& aCmd);
        void CompleteGetMetaDataValues();
        int32 AddToValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, PvmiKvp& aNewValue);
        PVMFStatus GetIndexParamValues(const char* aString, uint32& aStartIndex, uint32& aEndIndex);
        void DeleteAPICStruct(PvmfApicStruct*& aAPICStruct);
        PVMFStatus GetVideoFrameWidth(uint32 aId, int32& aWidth, int32& aDisplayWidth);
        PVMFStatus GetVideoFrameHeight(uint32 aId, int32& aHeight, int32& aDisplayHeight);
        int32 FindVideoWidth(uint32 aId);
        int32 FindVideoHeight(uint32 aId);
        int32 FindVideoDisplayWidth(uint32 aId);
        int32 FindVideoDisplayHeight(uint32 aId);
        PVMFStatus PopulateVideoDimensions(uint32 aId);
        uint32 GetNumAudioChannels(uint32 aId);
        uint32 GetAudioSampleRate(uint32 aId);
        uint32 GetAudioBitsPerSample(uint32 aId);
        PVMFStatus FindBestThumbnailKeyFrame(uint32 aId, uint32& aKeyFrameNum);

        // For data source position extension interface
        PVMFStatus DoSetDataSourcePosition(PVMFASFFFParserNodeCommand& aCmd, PVMFStatus &aEventCode, PVUuid &aEventUuid);
        PVMFStatus DoQueryDataSourcePosition(PVMFASFFFParserNodeCommand& aCmd);
        PVMFStatus DoSetDataSourceRate(PVMFASFFFParserNodeCommand& aCmd);

        PVMFStatus DoSetDataSourceDirection(PVMFASFFFParserNodeCommand& aCmd);

        void HandleTrackState();
        bool RetrieveTrackConfigInfo(uint32 aTrackId,
                                     PVMFFormatType aFormatType,
                                     OsclRefCounterMemFrag &aConfig);
        bool RetrieveTrackConfigInfoAndFirstSample(uint32 aTrackId,
                PVMFFormatType aFormatType,
                OsclRefCounterMemFrag &aConfig);
        bool RetrieveTrackData(PVASFFFNodeTrackPortInfo& aTrackPortInfo);
        bool SendTrackData(PVASFFFNodeTrackPortInfo& aTrackPortInfo);
        bool GenerateAVCNALGroup(PVASFFFNodeTrackPortInfo& aTrackPortInfo, OsclSharedPtr<PVMFMediaDataImpl>& aMediaFragGroup);
        bool GenerateAACFrameFrags(PVASFFFNodeTrackPortInfo& aTrackPortInfo, OsclSharedPtr<PVMFMediaDataImpl>& aMediaFragGroup);
        bool GetAVCNALLength(OsclBinIStreamBigEndian& stream, uint32& lengthSize, int32& len);
        bool UpdateTextSampleEntry(PVASFFFNodeTrackPortInfo& aTrackPortInfo, uint32 aEntryIndex, PVMFTimedTextMediaData& aTextMediaData);
        bool SendEndOfTrackCommand(PVASFFFNodeTrackPortInfo& aTrackPortInfo);
        bool SendBeginOfMediaStreamCommand(PVASFFFNodeTrackPortInfo& aTrackPortInfo);

        // Port processing
        void ProcessPortActivity();
        void QueuePortActivity(const PVMFPortActivity& aActivity);
        PVMFStatus ProcessIncomingMsg(PVMFPortInterface* aPort);
        PVMFStatus ProcessOutgoingMsg(PVMFPortInterface* aPort);
        Oscl_Vector<PVMFPortActivity, OsclMemAllocator> iPortActivityQueue;

        friend class PVMFASFFFParserOutPort;

        PVMFFormatType GetFormatTypeFromMIMEType(PvmfMimeString* aMIMEString);

        void ResetAllTracks();
        bool ReleaseAllPorts();
        void RemoveAllCommands();
        void CleanupFileSource();

        // For comparison with download progress clock
        bool checkTrackPosition(PVASFFFNodeTrackPortInfo& aTrackPortInfo, uint32 numsamples);

        bool GetTrackPortInfoForTrackID(PVASFFFNodeTrackPortInfo*& aInfo,
                                        uint32 aTrackID);

        bool GetTrackPortInfoForPort(PVASFFFNodeTrackPortInfo*& aInfo,
                                     PVMFPortInterface* aPort);

        OSCL_wHeapString<OsclMemAllocator> iFilename;
        PVMFFormatType iSourceFormat;
        PVMFMediaClock* iClientPlayBackClock;
        PVMFMediaClockNotificationsInterface *iClockNotificationsInf;
        bool iUseCPMPluginRegistry;
        PVMFLocalDataSource iCPMSourceData;
        PVMFSourceContextData iSourceContextData;
        OsclFileHandle* iFileHandle;
        Oscl_FileServer iFileServer;
        uint32 iParsingMode;
        bool iProtectedFile;
        IMpeg4File* iASFFileHandle;
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iAvailableMetadataKeys;
        uint32 iASFParserNodeMetadataValueCount;
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iCPMMetadataKeys;
        Oscl_Vector<PVASFFFNodeTrackPortInfo, OsclMemAllocator> iNodeTrackPortList;
        Oscl_Vector<PVMFTrackInfo, OsclMemAllocator> iSelectedTrackInfoList;
        Oscl_Vector<VideoTrackDimensionInfo, OsclMemAllocator> iVideoDimensionInfoVec;

        PVMFASFFFPortIter* iPortIter;

        // stream id
        uint32 iStreamID;

        PVMFNodeCapability iCapability;
        PVLogger* iLogger;
        PVLogger* iDataPathLogger;
        PVLogger* iAVCDataPathLogger;
        PVLogger* iClockLogger;
        PVLogger* iDiagnosticsLogger;
        // Reference counter for extension
        uint32 iExtensionRefCount;

        // variables to support download autopause
        OsclSharedPtr<PVMFMediaClock> download_progress_clock;
        PVMFDownloadProgressInterface* download_progress_interface;
        uint32 iDownloadFileSize;
        bool autopaused;

        void DataStreamCommandCompleted(const PVMFCmdResp& aResponse);
        void DataStreamInformationalEvent(const PVMFAsyncEvent& aEvent);
        void DataStreamErrorEvent(const PVMFAsyncEvent& aEvent);

        bool MapASFErrorCodeToEventCode(int32 aASFErrCode, PVUuid& aEventUUID, int32& aEventCode);

        GAU iGau;

        bool iThumbNailMode;

        // Content Policy Manager related
        bool iSourceContextDataValid;
        bool iPreviewMode;
        PVMFCPM* iCPM;
        PVMFSessionId iCPMSessionID;
        PVMFCPMContentType iCPMContentType;
        PVMFCPMPluginAccessInterfaceFactory* iCPMContentAccessFactory;
        PVMFMetadataExtensionInterface* iCPMMetaDataExtensionInterface;
        PVMFCPMPluginLicenseInterface* iCPMLicenseInterface;
        PVInterface* iCPMLicenseInterfacePVI;
        PVMFCPMPluginAccessUnitDecryptionInterface* iDecryptionInterface;
        PvmiKvp iRequestedUsage;
        PvmiKvp iApprovedUsage;
        PvmiKvp iAuthorizationDataKvp;
        PVMFCPMUsageID iUsageID;
        bool oWaitingOnLicense;
        bool iPoorlyInterleavedContentEventSent;

        PVMFCommandId iCPMInitCmdId;
        PVMFCommandId iCPMOpenSessionCmdId;
        PVMFCommandId iCPMRegisterContentCmdId;
        PVMFCommandId iCPMRequestUsageId;
        PVMFCommandId iCPMUsageCompleteCmdId;
        PVMFCommandId iCPMCloseSessionCmdId;
        PVMFCommandId iCPMResetCmdId;
        PVMFCommandId iCPMGetMetaDataKeysCmdId;
        PVMFCommandId iCPMGetMetaDataValuesCmdId;
        PVMFCommandId iCPMGetLicenseInterfaceCmdId;
        PVMFCommandId iCPMGetLicenseCmdId;
        PVMFCommandId iCPMCancelGetLicenseCmdId;
        void InitCPM();
        void OpenCPMSession();
        void CPMRegisterContent();
        bool GetCPMContentAccessFactory();
        void GetCPMContentType();
        bool GetCPMMetaDataExtensionInterface();
        void GetCPMLicenseInterface();
        void RequestUsage(PVASFFFNodeTrackOMA2DRMInfo* aInfo);
        void SendUsageComplete();
        void CloseCPMSession();
        void ResetCPM();
        void GetCPMMetaDataKeys();
        PVMFStatus CheckCPMCommandCompleteStatus(PVMFCommandId, PVMFStatus);
        PVMFStatus iCPMRequestUsageCommandStatus;

        PVMFStatus DoGetLicense(PVMFASFFFParserNodeCommand& aCmd,
                                bool aWideCharVersion = false);
        PVMFStatus DoCancelGetLicense(PVMFASFFFParserNodeCommand& aCmd);
        void CompleteGetLicense();

        void PopulateOMA1DRMInfo();
        /*
         * OMA2 DRM Related Methods
         */
        Oscl_Vector<PVASFFFNodeTrackOMA2DRMInfo, OsclMemAllocator> iOMA2DRMInfoVec;
        PVASFFFNodeTrackOMA2DRMInfo* LookUpOMA2TrackInfoForTrack(uint32 aTrackID);
        PVMFStatus InitOMA2DRMInfo();
        void PopulateOMA2DRMInfo(PVASFFFNodeTrackOMA2DRMInfo* aInfo);
        PVMFStatus CheckForOMA2AuthorizationComplete(PVASFFFNodeTrackOMA2DRMInfo*& aInfo);
        void OMA2TrackAuthorizationComplete();
        bool CheckForOMA2UsageApproval();
        void ResetOMA2Flags();
        uint8* iOMA2DecryptionBuffer;

        PVMFStatus GetFileOffsetForAutoResume(uint32& aOffset, bool aPortsAvailable = true);
        PVMFStatus GetFileOffsetForAutoResume(uint32& aOffset, PVASFFFNodeTrackPortInfo* aInfo);

        PVMFStatus CheckForUnderFlow(PVASFFFNodeTrackPortInfo* aInfo);

        void getLanguageCode(uint16 langcode, int8 *LangCode);
        void getBrand(uint32 langcode, char *LangCode);

        PVMFStatus CheckForASFHeaderAvailability();
        int32 CreateErrorInfoMsg(PVMFBasicErrorInfoMessage** aErrorMsg, PVUuid aEventUUID, int32 aEventCode);
        void CreateDurationInfoMsg(uint32 adurationms);
        PVMFStatus PushKVPToMetadataValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>* aVecPtr, PvmiKvp& aKvpVal);
        PVMFStatus CreateNewArray(uint32** aTrackidList, uint32 aNumTracks);
        PVMFStatus PushValueToList(Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> &aRefMetadataKeys,
                                   PVMFMetadataList *&aKeyListPtr,
                                   uint32 aLcv);
        PVMIDataStreamSyncInterface* iDataStreamInterface;
        PVMFDataStreamFactory* iDataStreamFactory;
        PVMFDataStreamReadCapacityObserver* iDataStreamReadCapacityObserver;
        PvmiDataStreamSession iDataStreamSessionID;
        PvmiDataStreamCommandId iRequestReadCapacityNotificationID;
        uint32 iASFHeaderSize;
        bool iDownloadComplete;
        bool iProgressivelyDownlodable;
        uint32 iLastNPTCalcInConvertSizeToTime;
        uint32 iFileSizeLastConvertedToTime;
        bool iFastTrackSession;

        /* External PseudoStreaming related */
        bool iExternalDownload;

        bool iUnderFlowEventReported;
        PVMFStatus ReportUnderFlow();
        OsclTimer<OsclMemAllocator> *iUnderFlowCheckTimer;

        /* bitstream logging */
        void LogMediaData(PVMFSharedMediaDataPtr data,
                          PVMFPortInterface* aPort);
        bool iPortDataLog;
        char iLogFileIndex;
        OSCL_HeapString<OsclMemAllocator> portLogPath;

        uint32 minTime;
        uint32 avgTime;
        uint32 maxTime;
        uint32 sumTime;
        bool iDiagnosticsLogged;
        void LogDiagnostics();
        uint32 iTimeTakenInReadASFFile;
        bool iBackwardReposFlag; /* To avoid backwardlooping :: A flag to remember backward repositioning */
        bool iForwardReposFlag;
        uint32 iCurPos;
        bool iEOTForTextSentToMIO;

        bool iSetTextSampleDurationZero;

        /* To take into account if we get negative TS for text track after repositionings*/
        bool iTextInvalidTSAfterReposition;

        uint32 iDelayAddToNextTextSample;

        uint32 iCacheSize;
        uint32 iAsyncReadBuffSize;
        bool iPVLoggerEnableFlag;
        bool iPVLoggerStateEnableFlag;
        uint32 iNativeAccessMode;

        BaseKeys_SelectionType iBaseKey;
        uint32 iJitterBufferDurationInMs;
        bool iDataStreamRequestPending;
        bool iCPMSequenceInProgress;
        bool oIsAACFramesFragmented;

        int32 iPlayBackDirection;
        int32 iStartForNextTSSearch;
        int32 iPrevSampleTS;
        bool iParseAudioDuringFF;
        bool iParseAudioDuringREW;
        bool iParseVideoOnly;
        int32 iDataRate;

        int32 minFileOffsetTrackID;

        // Ryan Lin !!
        // For Pmem  
        PVMFPmemMemAlloc iInBufMemoryAlloc;
#endif        
};

#endif // PVMF_ASFFFPARSER_NODE_H_INCLUDED

