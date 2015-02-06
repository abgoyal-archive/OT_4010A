
#ifndef TEST_PV_PLAYER_ENGINE_TESTSET5_H_INCLUDED
#define TEST_PV_PLAYER_ENGINE_TESTSET5_H_INCLUDED


#ifndef TEST_PV_PLAYER_ENGINE_H_INCLUDED
#include "test_pv_player_engine.h"
#endif

#ifndef TEST_PV_PLAYER_ENGINE_CONFIG_H_INCLUDED
#include "test_pv_player_engine_config.h"
#endif

#if RUN_FASTTRACK_TESTCASES
#ifndef PVPVXPARSER_H_INCLUDED
#include "pvpvxparser.h"
#endif
#endif

#if !(JANUS_IS_LOADABLE_MODULE)
#ifndef PVMF_CPMPLUGIN_FACTORY_REGISTRY_H_INCLUDED
#include "pvmf_cpmplugin_factory_registry.h"
#endif
#endif

class PVPlayerDataSourceURL;
class PVPlayerDataSinkFilename;
class PVLogger;
class PVMFDownloadDataSourcePVX;
class PVMFDownloadDataSourceHTTP;
class PVMFSourceContextData;
class PvmiCapabilityAndConfig;
class PVMFJanusPluginFactory;
#if RUN_CPMJANUS_TESTCASES
class PVMFJanusPluginConfiguration;
#endif
class WmDrmDeviceInfoFactory;
class PVWmdrmDeviceSystemClockFactory;

class pvplayer_async_test_downloadbase : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_downloadbase(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDownloadContextData(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
                , iProtocolRollOver(false)
                , iRepositionAfterDownloadComplete(false)
                , iPauseAfterDownloadComplete(false)
                , iPlayUntilEOS(false)
                , iPauseResumeAfterUnderflow(false)
                , iProgPlayback(false)
                , iPlayStopPlay(false)
                , iSessionDuration(0)
                , iCancelAllCmdId(0)
        {
            iNumBufferingStart = iNumBufferingComplete = iNumUnderflow = iNumDataReady = iNumEOS = 0;
            iDownloadOnly = iDownloadThenPlay = false;
            iLastBufferingStatusVal = 0;
        }

        virtual ~pvplayer_async_test_downloadbase();

        void StartTest();
        void Run();

        virtual void CommandCompleted(const PVCmdResponse& aResponse);
        virtual void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        virtual void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_QUERYINTERFACE,
            STATE_ADDDATASOURCE,
            STATE_CONFIGPARAMS,
            STATE_INIT,
            STATE_GETMETADATAKEYLIST,
            STATE_GETMETADATAVALUELIST,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_WAIT_FOR_DATAREADY,
            STATE_WAIT_FOR_BUFFCOMPLETE,
            STATE_CANCELALL,
            STATE_WAIT_FOR_CANCELALL,
            STATE_START,
            STATE_SETPLAYBACKRANGE,
            STATE_PAUSE,
            STATE_RESUME,
            STATE_EOSNOTREACHED,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE,
            STATE_PREPARE2,
            STATE_START2,
            STATE_STOP2
        };


        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVMFSourceContextData* iDownloadContextData;
        PVPlayerDataSink* iDataSinkVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeVideo;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutVideo;
        PvmiMIOControl* iMIOFileOutAudio;

        OSCL_wHeapString<OsclMemAllocator> iDownloadURL;
        OSCL_wHeapString<OsclMemAllocator> iDownloadFilename;
        OSCL_HeapString<OsclMemAllocator> iDownloadProxy;
        OSCL_wHeapString<OsclMemAllocator> iDownloadConfigFilename;

        PVCommandId iCurrentCmdId;
        PVLogger* iLogger;

        virtual void CreateDataSource() = 0;
        virtual void CreateDataSinkVideo() = 0;
        virtual void CreateDataSinkAudio() = 0;
        virtual void CleanupData() {}

        void PrintMetadata();
        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        int32 iNumBufferingStart, iNumBufferingComplete, iNumUnderflow, iNumDataReady, iNumEOS;
        bool iDownloadOnly, iDownloadThenPlay;
        bool iContentTooLarge;

        void setProtocolRollOverMode()
        {
            iProtocolRollOver = true;
        }
        bool iProtocolRollOver;

        void enableReposAfterDownloadComplete()
        {
            iRepositionAfterDownloadComplete = true;
        }
        bool iRepositionAfterDownloadComplete;

        void enablePauseAfterDownloadComplete()
        {
            iPauseAfterDownloadComplete = true;
        }
        bool iPauseAfterDownloadComplete;

        void enablePlayUntilEOS()
        {
            iPlayUntilEOS = true;
        }
        bool iPlayUntilEOS;

        bool iPauseResumeAfterUnderflow;

        bool iProgPlayback;
        bool iPlayStopPlay;
        void enablePlayStopPlay()
        {
            iPlayStopPlay = true;
            iTestCaseName += " Play/Stop/Play";
        }
        int32 iLastBufferingStatusVal;

        //for janus drm.
        void PrintJanusError(const PVCmdResponse& aResponse);

        uint32 iSessionDuration;
    protected:
        PVCommandId iCancelAllCmdId;
    private:
        void HandleProtocolEngineNodeErrors(int32 aErr, OsclAny* aEventData);
        void HandleSocketNodeErrors(int32 aErr);
    private:
        PvmiCapabilityAndConfig* iPlayerCapConfigIF;
        PvmiKvp* iErrorKVP;
        PvmiKvp iKVPSetAsync;
        OSCL_StackString<256> iKeyStringSetAsync;
};

#if RUN_FASTTRACK_TESTCASES
class pvplayer_async_test_ftdlnormal : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_ftdlnormal(PVPlayerAsyncTestParam aTestParam)
                : pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_ftdlnormal");
            iTestCaseName = _STRLIT_CHAR("FastTrack Download Normal");
        }

        ~pvplayer_async_test_ftdlnormal();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

        uint8 iPVXFileBuf[4096];
        int32 iDownloadMaxfilesize;
        CPVXInfo iDownloadPvxInfo;
};
#endif

class pvplayer_async_test_3gppdlnormal : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_3gppdlnormal(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlnormal");
            iTestCaseName = _STRLIT_CHAR("3GPP Download Play ASAP");
#if RUN_CPMJANUS_TESTCASES && !(JANUS_IS_LOADABLE_MODULE)
            iPluginFactory = NULL;
            iDrmDeviceInfoFactory = NULL;
            iDrmSystemClockFactory = NULL;
#endif
        }

        ~pvplayer_async_test_3gppdlnormal();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

        //for janus DRM
#if RUN_CPMJANUS_TESTCASES && !(JANUS_IS_LOADABLE_MODULE)
        PVMFCPMPluginFactoryRegistryClient iPluginRegistryClient;
        PVMFJanusPluginFactory* iPluginFactory;
        OSCL_HeapString<OsclMemAllocator> iPluginMimeType;
        bool RegisterJanusPlugin(PVMFJanusPluginConfiguration& aConfig);
        void CleanupJanusPlugin();
        WmDrmDeviceInfoFactory* iDrmDeviceInfoFactory;
        PVWmdrmDeviceSystemClockFactory* iDrmSystemClockFactory;
#endif
        void CleanupData();
};

class pvplayer_async_test_ppbnormal : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_ppbnormal(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_ppbnormal");
            iTestCaseName = _STRLIT_CHAR("PPB Normal");
            iProgPlayback = true;
        }

        ~pvplayer_async_test_ppbnormal();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();
};

class pvplayer_async_test_cnclall_prtcl_rllovr : public pvplayer_async_test_3gppdlnormal
{
    public:
        pvplayer_async_test_cnclall_prtcl_rllovr(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_3gppdlnormal(aTestParam), iCancelCommandPending(true), iCancellationEventTime(Undetermined), iProtocolRolloverNotificationRecvd(false)
        {
        }
        void CancelAllDuringProtocolRolloverWithNoGetReqAfterRolloverTest()
        {
            iCancellationEventTime = NoGetReqAfterRollover;
        }
        void CancelAllDuringProtocolRolloverWithGetResponseRecvdAfterRolloverTest()
        {
            iCancellationEventTime = GetResponseRecvdAfterRollover;
        }
        void CancelAllDuringProtocolRolloverWithNoGetResponseRecvdAfterRolloverTest()
        {
            iCancellationEventTime = NoGetResponseRecvdAfterRollover;
        }
    protected:
        virtual void CommandCompleted(const PVCmdResponse& aResponse);
        virtual void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);
        bool iCancelCommandPending;

    private:
        enum CancellationEventTime
        {
            Undetermined = 0,
            NoGetReqAfterRollover = 1,
            NoGetResponseRecvdAfterRollover = 2,
            GetResponseRecvdAfterRollover = 3
        };
        CancellationEventTime iCancellationEventTime;
        bool iProtocolRolloverNotificationRecvd;
};

class pvplayer_async_test_3gppdlnormal_dlthenplay : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_3gppdlnormal_dlthenplay(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlnormal_dlthenplay");
            iTestCaseName = _STRLIT_CHAR("3GPP Download Then Play");
        }

        ~pvplayer_async_test_3gppdlnormal_dlthenplay();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

};
class pvplayer_async_test_3gppdlnormal_dlonly : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_3gppdlnormal_dlonly(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlnormal_dlonly");

            iTestCaseName = _STRLIT_CHAR("3GPP Download Only");
        }

        ~pvplayer_async_test_3gppdlnormal_dlonly();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

};


class pvplayer_async_test_3gppdlcancelduringinit : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_3gppdlcancelduringinit(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDownloadHttpContextData(NULL)
                , iCancelCommandExecuted(false)
                , iCurrentCmdId(0)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlcancelduringinit");
            iTestCaseName = _STRLIT_CHAR("3GPP Download Cancel During Init");
        }

        ~pvplayer_async_test_3gppdlcancelduringinit() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_CANCELALL,
            STATE_WAIT_FOR_CANCELALL,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVMFDownloadDataSourceHTTP* iDownloadHttpContextData;
        OSCL_wHeapString<OsclMemAllocator> iDownloadURL;
        OSCL_wHeapString<OsclMemAllocator> iDownloadFilename;
        OSCL_HeapString<OsclMemAllocator> iDownloadProxy;
        OSCL_wHeapString<OsclMemAllocator> iConfigFileName;
        bool iCancelCommandExecuted;
        PVCommandId iCurrentCmdId;
        PVLogger* iLogger;
};

class pvplayer_async_test_3gppdlcancelduringinitdelay : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_3gppdlcancelduringinitdelay(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDownloadHttpContextData(NULL)
                , iCancelCommandExecuted(false)
                , iCurrentCmdId(0)
                , iInitCmdId(-1)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlcancelduringinitdelay");
            iTestCaseName = _STRLIT_CHAR("3GPP Download Cancel During Init - delayed");
        }

        ~pvplayer_async_test_3gppdlcancelduringinitdelay() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_CANCELALL,
            STATE_WAIT_FOR_CANCELALL,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVMFDownloadDataSourceHTTP* iDownloadHttpContextData;
        OSCL_wHeapString<OsclMemAllocator> iDownloadURL;
        OSCL_wHeapString<OsclMemAllocator> iDownloadFilename;
        OSCL_HeapString<OsclMemAllocator> iDownloadProxy;
        OSCL_wHeapString<OsclMemAllocator> iConfigFileName;
        bool iCancelCommandExecuted;
        PVCommandId iCurrentCmdId;
        PVCommandId iInitCmdId;
        PVLogger* iLogger;
};

class pvplayer_async_test_3gppdlcontenttoolarge : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_3gppdlcontenttoolarge(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlcontenttoolarge");
            iTestCaseName = _STRLIT_CHAR("3GPP PVMFErrContentTooLarge during Init");
        }

        ~pvplayer_async_test_3gppdlcontenttoolarge();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

};


class pvplayer_async_test_3gppdlContentTruncated : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_3gppdlContentTruncated(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_3gppdlContentTruncated");
            iTestCaseName = _STRLIT_CHAR("3GPP PVMFInfoContentTruncated during download");
        }

        ~pvplayer_async_test_3gppdlContentTruncated();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();

};


class pvplayer_async_test_ppb_base : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_ppb_base(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iDownloadContextData(NULL)
                , iCurrentCmdId(0)
                , iRepositionAfterDownloadComplete(false)
                , iPlayUntilEOS(false)
                , iShortPauseResume(false)
                , iLongPauseResume(false)
                , iSeekAfterStart(false)
                , iSeekBeforeStart(false)
                , iTwice(false)
                , iLoop(0)
                , iLoopReq(0)
                , iStepInterval(0)
                , iBeginTime(0)
                , iEndTime(0)
                , iForwardStep(false)
                , iForwardSeekStep(0)
                , iForwardSeekTime(0)
                , iBackwardStep(false)
                , iBackwardSeekStep(0)
                , iBackwardSeekTime(0)
                , iSeekAfterDownloadComplete(false)
                , iSeekToBOC(false)
                , iSeekInCache(false)
                , iEOSStopPlay(false)
                , iShoutcastSession(false)
                , iSCListenTime(0)
                , iShoutcastPlayStopPlay(false)
                , iSessionDuration(0)
        {
            iNumBufferingStart = iNumBufferingComplete = iNumUnderflow = iNumDataReady = iNumEOS = 0;
        }

        virtual ~pvplayer_async_test_ppb_base() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_QUERYINTERFACE,
            STATE_ADDDATASOURCE,
            STATE_CONFIGPARAMS,
            STATE_INIT,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_SETPLAYBACKRANGE_BEFORESTART,
            STATE_PREPARE,
            STATE_PREPARE_AFTERSTOP,
            STATE_GETMETADATAKEYLIST,
            STATE_GETMETADATAVALUELIST,
            STATE_WAIT_FOR_DATAREADY,
            STATE_WAIT_FOR_BUFFCOMPLETE,
            STATE_START,
            STATE_START_AFTERSTOP,
            STATE_SETPLAYBACKRANGE,
            STATE_PAUSE,
            STATE_SETPLAYBACKRANGE_AFTERSTART,
            STATE_RESUME,
            STATE_PAUSE_TWICE,
            STATE_SETPLAYBACKRANGE_AFTERSTART_TWICE,
            STATE_RESUME_TWICE,
            STATE_PAUSE_AFTERDOWNLOADCOMPLETE,
            STATE_SETPLAYBACKRANGE_AFTERDOWNLOADCOMPLETE,
            STATE_RESUME_AFTERDOWNLOADCOMPLETE,
            STATE_STOP,
            STATE_STOP_TWICE,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeVideo;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutVideo;
        PvmiMIOControl* iMIOFileOutAudio;
        PVMFSourceContextData* iDownloadContextData;

        OSCL_wHeapString<OsclMemAllocator> iDownloadURL;
        OSCL_wHeapString<OsclMemAllocator> iDownloadFilename;
        OSCL_HeapString<OsclMemAllocator> iDownloadProxy;
        OSCL_wHeapString<OsclMemAllocator> iDownloadConfigFilename;

        PVCommandId iCurrentCmdId;
        PVLogger* iLogger;

        virtual void CreateDataSource() = 0;
        virtual void CreateDataSinkVideo() = 0;
        virtual void CreateDataSinkAudio() = 0;
        virtual void CleanupData() {}

        void PrintMetadata();
        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        int32 iNumBufferingStart;
        int32 iNumBufferingComplete;
        int32 iNumUnderflow;
        int32 iNumDataReady;
        int32 iNumEOS;

        bool iContentTooLarge;

        void enableReposAfterDownloadComplete()
        {
            iRepositionAfterDownloadComplete = true;
        }
        bool iRepositionAfterDownloadComplete;

        void enablePlayUntilEOS()
        {
            iPlayUntilEOS = true;
        }
        bool iPlayUntilEOS;

        void enableShortPauseResume()
        {
            iShortPauseResume = true;
        }
        bool iShortPauseResume;

        void enableLongPauseResume()
        {
            iLongPauseResume = true;
        }
        bool iLongPauseResume;

        void enableSeekAfterStart()
        {
            iSeekAfterStart = true;
        }
        bool iSeekAfterStart;

        void enableSeekBeforeStart()
        {
            iSeekBeforeStart = true;
        }
        bool iSeekBeforeStart;

        void enableTwice()
        {
            iTwice = true;
        }
        bool iTwice;

        void enableLoop()
        {
            iLoop = iLoopReq = 10;
        }
        int32 iLoop;
        int32 iLoopReq;

        void enableForwardStep()
        {
            iForwardStep = true;
            // the following value are in millisecs
            iBeginTime = 0;
            iEndTime = 250 * 1000;
            iForwardSeekStep = 2 * 1000;
            iForwardSeekTime = 60 * 1000;
            iStepInterval = 5 * 1000;
        }
        int32 iStepInterval;
        int32 iBeginTime;
        int32 iEndTime;
        bool iForwardStep;
        int32 iForwardSeekStep;
        int32 iForwardSeekTime;
        bool iBackwardStep;
        int32 iBackwardSeekStep;
        int32 iBackwardSeekTime;

        void enableSeekAfterDownloadComplete()
        {
            iSeekAfterDownloadComplete = true;
        }
        bool iSeekAfterDownloadComplete;

        void enableSeekToBOC()
        {
            iSeekToBOC = true;
        }
        bool iSeekToBOC;

        void enableSeekInCache()
        {
            iSeekInCache = true;
        }
        bool iSeekInCache;

        void enableEOSStopPlay()
        {
            iEOSStopPlay = true;
        }
        bool iEOSStopPlay;

        void setShoutcastSessionDuration()
        {
            iSCListenTime = 5 * 60 * 1000 * 1000;
        }
        bool iShoutcastSession;
        int32 iSCListenTime;
        bool iShoutcastPlayStopPlay;

        void enableShoutcastPauseResume()
        {
            iSCListenTime = 1 * 60 * 1000 * 1000;
            enableShortPauseResume();
        }

        void enableShoutcastPlayStopPlay()
        {
            iSCListenTime = 30 * 1000 * 1000;
            iShoutcastPlayStopPlay = true;
        }

        //for janus drm.
        void PrintJanusError(const PVCmdResponse& aResponse);

        uint32 iSessionDuration;

    private:
        void HandleProtocolEngineNodeErrors(int32 aErr);
        void HandleSocketNodeErrors(int32 aErr);
    private:
        PvmiCapabilityAndConfig* iPlayerCapConfigIF;
        PvmiKvp* iErrorKVP;
        PvmiKvp iKVPSetAsync;
        OSCL_StackString<256> iKeyStringSetAsync;
};


class pvplayer_async_test_ppb_normal : public pvplayer_async_test_ppb_base
{
    public:
        pvplayer_async_test_ppb_normal(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_ppb_base(aTestParam)
        {
            iDownloadContextData = NULL;
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_ppb_normal");
            iTestCaseName = _STRLIT_CHAR("Progressive Playback Normal");
        }

        ~pvplayer_async_test_ppb_normal();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();
};

class pvplayer_async_test_PDLPauseResumeAfterUnderFlow : public pvplayer_async_test_downloadbase
{
    public:
        pvplayer_async_test_PDLPauseResumeAfterUnderFlow(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_downloadbase(aTestParam)
        {
            iLogger = PVLogger::GetLoggerObject("pvplayer_async_test_PDLPauseResumeAfterUnderFlow");
            iTestCaseName = _STRLIT_CHAR("PDL Pause Resume After UnderFlow");
        }

        ~pvplayer_async_test_PDLPauseResumeAfterUnderFlow();

        void CreateDataSource();
        void CreateDataSinkVideo();
        void CreateDataSinkAudio();
        void CleanupData();
};

#endif  // TEST_PV_PLAYER_ENGINE_TESTSET5_H_INCLUDED

