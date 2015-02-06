
#ifndef TEST_PV_PLAYER_ENGINE_TESTSET1_H_INCLUDED
#define TEST_PV_PLAYER_ENGINE_TESTSET1_H_INCLUDED


#ifndef TEST_PV_PLAYER_ENGINE_H_INCLUDED
#include "test_pv_player_engine.h"
#endif

#ifndef PV_PLAYER_DATASOURCEURL_H_INCLUDED
#include "pv_player_datasourceurl.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

#ifndef PV_ENGINE_TYPES_H_INCLUDED
#include "pv_engine_types.h"
#endif

#ifndef TEST_PV_PLAYER_ENGINE_CONFIG_H_INCLUDED
#include "test_pv_player_engine_config.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED
#include "pvmi_config_and_capability_observer.h"
#endif

#ifndef PVMF_CPMPLUGIN_FACTORY_REGISTRY_H_INCLUDED
#include "pvmf_cpmplugin_factory_registry.h"
#endif

#ifndef PVMF_STREAMING_DATA_SOURCE_H_INCLUDED
#include "pvmf_streaming_data_source.h"
#endif

#ifndef PVMF_SOURCE_CONTEXT_DATA_H_INCLUDED
#include "pvmf_source_context_data.h"
#endif



#define AMR_MPEG4_RTSP_URL "rtsp://pvserveroha.pv.com/public/Interop/3GPP/pv2/pv-amr-475_mpeg4-20.3gp"


class PVPlayerDataSink;
class PVPlayerDataSinkFilename;
class PvmfFileOutputNodeConfigInterface;
class PvmiCapabilityAndConfig;
class PVRefFileOutput;


class pvplayer_async_test_newdelete : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_newdelete(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
        {
            iTestCaseName = _STRLIT_CHAR("New-Delete");
        }

        ~pvplayer_async_test_newdelete() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);
};


class pvplayer_async_test_openplaystopreset : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_openplaystopreset(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Open-Play-Stop-Reset");
        }

        ~pvplayer_async_test_openplaystopreset() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class PVMFLocalDataSource;
class PVMFOma1PassthruPluginFactory;
class PVMFCPMPluginAccessInterfaceFactory;
class PVMFSourceContextData;

class pvplayer_async_test_cpmopenplaystopreset : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_cpmopenplaystopreset(PVPlayerAsyncTestParam aTestParam, bool aUsingDataStreamInput = false):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
                , iDataStreamFactory(NULL)
                , iSourceContextData(NULL)
                , iUsingDataStreamInput(aUsingDataStreamInput)
        {
            if (aUsingDataStreamInput)
            {
                iTestCaseName = _STRLIT_CHAR("CPM Open-Play-Stop-Reset Using DataStream Input");
            }
            else
            {
                iTestCaseName = _STRLIT_CHAR("CPM Open-Play-Stop-Reset");
            }
            iLocalDataSource = NULL;
            iPluginFactory = NULL;
        }

        ~pvplayer_async_test_cpmopenplaystopreset() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;

        PVMFLocalDataSource* iLocalDataSource;
        PVMFCPMPluginFactoryRegistryClient iPluginRegistryClient;
        PVMFOma1PassthruPluginFactory* iPluginFactory;
        OSCL_HeapString<OsclMemAllocator> iPluginMimeType;
        PVMFCPMPluginAccessInterfaceFactory* iDataStreamFactory;
        PVMFSourceContextData* iSourceContextData;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
        bool iUsingDataStreamInput;
};


class pvplayer_async_test_metadata : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_metadata(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Metadata");
        }

        ~pvplayer_async_test_metadata() {}

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
            STATE_GETMETADATAKEYS1,
            STATE_GETMETADATAVALUES1,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_GETMETADATAKEYS2,
            STATE_GETMETADATAVALUES2,
            STATE_GETMETADATAKEYSSEG,
            STATE_GETMETADATAVALUESSEG1,
            STATE_GETMETADATAVALUESSEG2,
            STATE_GETMETADATAVALUESSEG3,
            STATE_START,
            STATE_STOP,
            STATE_GETMETADATAKEYS3,
            STATE_GETMETADATAVALUES3,
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

        PVCommandId iCurrentCmdId;

    private:
        PVPMetadataList iKeyList;
        int32 iNumAvailableValues;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iValueList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iValueListSeg1;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iValueListSeg2;
        OSCL_HeapString<OsclMemAllocator> iKeyQueryString;

        int32 CheckMetadataValue(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList);
};


class pvplayer_async_test_timing : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_timing(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
                , iStartTime(0)
        {
            iTestCaseName = _STRLIT_CHAR("Timing");
        }

        ~pvplayer_async_test_timing() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE,
            STATE_WAIT
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
        PVCommandId iCurrentCmdId;
        PVPPlaybackPosition aPos;
        uint32 iStartTime;
};



class pvplayer_async_test_invalidstate : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_invalidstate(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Invalid State");
        }

        ~pvplayer_async_test_invalidstate() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_IDLE_INVALID_START,
            STATE_IDLE_INVALID_STOP,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_INITIALIZED_INVALID_ADDDATASOURCE,
            STATE_INITIALIZED_INVALID_START,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_PREPARED_INVALID_INIT,
            STATE_PREPARED_INVALID_PAUSE,
            STATE_START,
            STATE_STARTED_INVALID_RESUME,
            STATE_STARTED_INVALID_PREPARE,
            STATE_PAUSE,
            STATE_PAUSED_INVALID_INIT,
            STATE_PAUSED_INVALID_PREPARE,
            STATE_STOP,
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
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_preparedstop : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_preparedstop(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Stop When Prepared");
        }

        ~pvplayer_async_test_preparedstop() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_STOP,
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
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_videoonlyplay7seconds: public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_videoonlyplay7seconds(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Video-only Play 7 Sec");
        }

        ~pvplayer_async_test_videoonlyplay7seconds() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_play5stopplay10stopreset: public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_play5stopplay10stopreset(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play 5 Sec-Stop-Play 10 Sec-Stop-Reset");
        }

        ~pvplayer_async_test_play5stopplay10stopreset() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE_FIRST,
            STATE_START_FIRST,
            STATE_STOP_FIRST,
            STATE_PREPARE_SECOND,
            STATE_START_SECOND,
            STATE_STOP_SECOND,
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
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_pauseresume : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_pauseresume(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Pause-Resume");
        }

        ~pvplayer_async_test_pauseresume() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_PAUSE,
            STATE_RESUME,
            STATE_STOP,
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
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_playpausestop : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_playpausestop(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutVideo(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-Pause-Stop");
        }

        ~pvplayer_async_test_playpausestop() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_PAUSE,
            STATE_STOP,
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
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_outsidenodeforvideosink : public pvplayer_async_test_base,
        public PVMFNodeCmdStatusObserver,
        public PVMFNodeInfoEventObserver,
        public PVMFNodeErrorEventObserver
{
    public:
        pvplayer_async_test_outsidenodeforvideosink(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iMOutVideo(NULL)
                , iMOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Outside Node for Video Sink");
        }

        ~pvplayer_async_test_outsidenodeforvideosink() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void NodeCommandCompleted(const PVMFCmdResp& aResponse);
        void HandleNodeInformationalEvent(const PVMFAsyncEvent& /*aEvent*/) {}
        void HandleNodeErrorEvent(const PVMFAsyncEvent& /*aEvent*/) {}

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
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
        PvmiMIOControl* iMOutVideo;
        PvmiMIOControl* iMOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_getplayerstate : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_getplayerstate(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("GetPlayerState");
        }

        ~pvplayer_async_test_getplayerstate() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_PLAYER_STATE_FIRST,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_PLAYER_STATE_SECOND,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_PLAYER_STATE_THIRD,
            STATE_RESET,
            STATE_PLAYER_STATE_FOURTH,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;
        PVPlayerState aState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_getcurrentposition : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_getcurrentposition(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
                , iCount(0)
        {
            iTestCaseName = _STRLIT_CHAR("GetCurrentPosition");
        }

        ~pvplayer_async_test_getcurrentposition() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_GETCURRENT_POSITION_SYNC,
            STATE_GETCURRENT_POSITION,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
        PVMFStatus Status;
        PVPPlaybackPosition Position ;
        uint32 iCount;
};


class pvplayer_async_test_playsetstopposition : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_playsetstopposition(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-Set End Time");
        }

        ~pvplayer_async_test_playsetstopposition() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_SETPLAYBACKRANGE,
            STATE_STOPTIMENOTREACHED,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_playsetstoppositionvidframenum : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_playsetstoppositionvidframenum(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-Set End Time by VidFrameNum");
        }

        ~pvplayer_async_test_playsetstoppositionvidframenum() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_SETPLAYBACKRANGE,
            STATE_STOPTIMENOTREACHED,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_setstartpositionplaystop : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setstartpositionplaystop(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
                , iSourceContextData(NULL)
        {
            iTestCaseName = _STRLIT_CHAR("Set Begin Position-Play-Stop");
            iTargetNumSeek = 1;
            iNumSeek = 0;
        }

        ~pvplayer_async_test_setstartpositionplaystop() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_SETPLAYBACKRANGE,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;

        void setMultipleSeekMode(uint32 aNum)
        {
            iTargetNumSeek = aNum;
        }

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
        uint32 iTargetNumSeek;
        uint32 iNumSeek;

        PVMFSourceContextData* iSourceContextData;
};


class pvplayer_async_test_setplayrangeplay : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setplayrangeplay(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("SetPlaybackRange-Play");
        }

        ~pvplayer_async_test_setplayrangeplay() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_SETPLAYBACKRANGE,
            STATE_PREPARE,
            STATE_START,
            STATE_STOPTIMENOTREACHED,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_setplayrangevidframenumplay : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setplayrangevidframenumplay(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("SetPlaybackRange by VidFrameNum-Play");
        }

        ~pvplayer_async_test_setplayrangevidframenumplay() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_SETPLAYBACKRANGE,
            STATE_PREPARE,
            STATE_START,
            STATE_STOPTIMENOTREACHED,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_playsetplayrangestop : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_playsetplayrangestop(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-SetPlaybackRange-Stop");
        }

        ~pvplayer_async_test_playsetplayrangestop() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_SETPLAYBACKRANGE,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_playsetplayrangevidframenumstop : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_playsetplayrangevidframenumstop(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-SetPlaybackRange by VidFrameNum-Stop");
        }

        ~pvplayer_async_test_playsetplayrangevidframenumstop() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_SETPLAYBACKRANGE,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class PVMFTrackLevelInfoExtensionInterface;

class pvplayer_async_test_tracklevelinfo : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_tracklevelinfo(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iTrackLevelInfoIF(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Track-Level Info");
        }

        ~pvplayer_async_test_tracklevelinfo() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_INVALIDQUERYINTERFACE1,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_QUERYINTERFACE,
            STATE_TRACKLEVELINFOTEST,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_INVALIDQUERYINTERFACE2,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVMFTrackLevelInfoExtensionInterface* iTrackLevelInfoIF;
        PVCommandId iCurrentCmdId;
    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_setplaybackrate2X : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setplaybackrate2X(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("SetPlaybackRate 2X");
        }

        ~pvplayer_async_test_setplaybackrate2X() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_SETPLAYBACKRATE,
            STATE_START,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_setplaybackratefifth : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setplaybackratefifth(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("SetPlaybackRate 1/5X");
        }

        ~pvplayer_async_test_setplaybackratefifth() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_SETPLAYBACKRATE,
            STATE_START,
            STATE_STOP,
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
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;
};


class pvplayer_async_test_queuedcommands : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_queuedcommands(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Queued Commands");
        }

        ~pvplayer_async_test_queuedcommands() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_QUEUEDCMD1,
            STATE_QUEUEDCMD2,
            STATE_QUEUEDCMD3,
            STATE_QUEUEDCMD4,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_looping : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_looping(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Looping");
        }

        ~pvplayer_async_test_looping() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_STARTPLAYBACK,
            STATE_SETPLAYBACKRANGE1,
            STATE_ENDTIMENOTREACHED1,
            STATE_SETPLAYBACKRANGE2,
            STATE_RESUME,
            STATE_ENDTIMENOTREACHED2,
            STATE_SHUTDOWN,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_waitforeos : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_waitforeos(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Play-Wait For EOS");
        }

        ~pvplayer_async_test_waitforeos() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_EOSNOTREACHED,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};



class pvplayer_async_test_multipauseresume : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_multipauseresume(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Multiple Pause-Resume");
        }

        ~pvplayer_async_test_multipauseresume() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_STARTPLAYBACK,
            STATE_PAUSERESUME1,
            STATE_PAUSE,
            STATE_RESUME,
            STATE_PAUSERESUME2,
            STATE_SHUTDOWN,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_multireposition : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_multireposition(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Multiple SetPlaybackRange");
        }

        ~pvplayer_async_test_multireposition() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_STARTPLAYBACK,
            STATE_SETPLAYBACKRANGE1,
            STATE_SETPLAYBACKRANGE2,
            STATE_SETPLAYBACKRANGE3,
            STATE_SETPLAYBACKRANGE4,
            STATE_SETPLAYBACKRANGE5,
            STATE_SETPLAYBACKRANGE6,
            STATE_SHUTDOWN,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;

        Oscl_FileServer iFS;
        Oscl_File iTimeLogFile;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_capconfigiftest : public pvplayer_async_test_base, public PvmiConfigAndCapabilityCmdObserver
{
    public:
        pvplayer_async_test_capconfigiftest(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iPlayerCapConfigIF(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iCurrentCmdId(0)
                , iErrorKVP(NULL)
        {
            iTestCaseName = _STRLIT_CHAR("Capability&Config Interface");
        }

        ~pvplayer_async_test_capconfigiftest() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        // From PvmiConfigAndCapabilityCmdObserver
        void SignalEvent(int32 req_id);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_QUERYINTERFACE,
            STATE_CAPCONFIG1,
            STATE_CAPCONFIG2,
            STATE_ADDDATASOURCE,
            STATE_INIT,
            STATE_CAPCONFIG3,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_PREPARE,
            STATE_START,
            STATE_CAPCONFIG4,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PvmiCapabilityAndConfig* iPlayerCapConfigIF;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVCommandId iCurrentCmdId;

        PvmiKvp* iErrorKVP;
        PvmiKvp iKVPSetAsync;
        OSCL_StackString<64> iKeyStringSetAsync;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
};


class pvplayer_async_test_setplaybackafterprepare : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_setplaybackafterprepare(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
                , iSourceContextData(NULL)
        {
            iTestCaseName = _STRLIT_CHAR("SetPlaybackRange After Prepare");
        }

        ~pvplayer_async_test_setplaybackafterprepare() {}

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
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_SETPLAYBACKRANGE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;

        Oscl_FileServer iFS;
        Oscl_File iTimeLogFile;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];
        PVMFSourceContextData* iSourceContextData;
};

class pvplayer_async_test_multipauseseekresume : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_multipauseseekresume(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iIONodeVideo(NULL)
                , iMIOFileOutVideo(NULL)
                , iDataSinkAudio(NULL)
                , iIONodeAudio(NULL)
                , iMIOFileOutAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeText(NULL)
                , iMIOFileOutText(NULL)
                , iCurrentCmdId(0)
                , iSessionDuration(0)
                , iSourceContextData(NULL)
        {
            iTestCaseName = _STRLIT_CHAR("Multiple Pause SetPlaybackRange Resume");
            iTargetNumPause = 1;
            iNumPause = 0;
        }

        ~pvplayer_async_test_multipauseseekresume() {}

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
            STATE_GETMETADATAKEYLIST,
            STATE_GETMETADATAVALUELIST,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_PAUSE,
            STATE_SETPLAYBACKRANGE,
            STATE_RESUME,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVMFNodeInterface* iIONodeVideo;
        PvmiMIOControl* iMIOFileOutVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVMFNodeInterface* iIONodeAudio;
        PvmiMIOControl* iMIOFileOutAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMIOFileOutText;
        PVCommandId iCurrentCmdId;
        Oscl_Vector<PVCommandId, OsclMemAllocator> iCmdIds;

        void setMultiplePauseMode(uint32 aNum)
        {
            iTargetNumPause = aNum;
        }

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
        oscl_wchar output[512];

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        uint32 iTargetNumPause;
        uint32 iNumPause;
        uint32 iSessionDuration;

        PVMFSourceContextData* iSourceContextData;
};

class PVMFLocalDataSource;
class PvOmapVideo;
class PVRefOmapAudioOutput;


class pvplayer_async_test_multiple_instance : public pvplayer_async_test_base
{
    public:
        pvplayer_async_test_multiple_instance(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iIONodeText(NULL)
                , iMOutVideo(NULL)
                , iMOutAudio(NULL)
                , iMOutText(NULL)
                , iCurrentCmdId(0)
        {
            iSinkName = _STRLIT_WCHAR("pvplayer_async_test_multiple_instance_");
            iTestCaseName = _STRLIT_CHAR("Multiple Instance Open-Play-Stop");
            iLocalDataSource = NULL;

            iParentInstance = NULL;
            iChildThreadFailures = 0;
            iChildThreadLeave = 0;
            iChildThreadExit = false;
            //make a persistent copy of the test params.
            iParam = new PVPlayerAsyncTestParam();
            iParam->Copy(aTestParam);
        }

        ~pvplayer_async_test_multiple_instance()
        {
            delete iParam;
        }

        //To support the 2nd instance of the test case.
        pvplayer_async_test_multiple_instance* iParentInstance;
        int32 iChildThreadFailures;
        int32 iChildThreadLeave;
        bool iChildThreadExit;
        PVPlayerAsyncTestParam* iParam;
        void ChildTestIsTrue(bool);
        static void InThread(pvplayer_async_test_multiple_instance* parent);
        //An observer class for the 2nd instance of the test case.
        class ChildObserver: public pvplayer_async_test_observer
        {
            public:
                void TestCompleted(test_case &tc);
        };
        //End 2nd instance support.

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
            STATE_GETMETADATAKEYLIST,
            STATE_GETMETADATAVALUELIST,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeVideo;
        PVMFNodeInterface* iIONodeAudio;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMOutVideo;
        PvmiMIOControl* iMOutAudio;
        PvmiMIOControl* iMOutText;
        PVCommandId iCurrentCmdId;

        OSCL_wHeapString<OsclMemAllocator> iFileNameWStr;
        oscl_wchar iTmpWCharBuffer[512];

        PVMFLocalDataSource* iLocalDataSource;

        void PrintMetadata();
        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;
        OSCL_wHeapString<OsclMemAllocator> iSinkName;

};

class PVMFLocalDataSource;
class PvOmapVideo;
class PVRefOmapAudioOutput;
class OsclExecScheduler;

#include "threadsafe_queue.h"

class pvplayer_async_test_multiple_thread : public pvplayer_async_test_base
        , public ThreadSafeQueueObserver
{
    public:
        pvplayer_async_test_multiple_thread(PVPlayerAsyncTestParam aTestParam):
                pvplayer_async_test_base(aTestParam)
                , iPlayer(NULL)
                , iDataSource(NULL)
                , iDataSinkVideo(NULL)
                , iDataSinkAudio(NULL)
                , iDataSinkText(NULL)
                , iIONodeVideo(NULL)
                , iIONodeAudio(NULL)
                , iIONodeText(NULL)
                , iMOutVideo(NULL)
                , iMOutAudio(NULL)
                , iMOutText(NULL)
                , iCurrentCmdId(0)
        {
            iSinkName = _STRLIT_WCHAR("pvplayer_async_test_multiple_thread_");
            iTestCaseName = _STRLIT_CHAR("Multiple Thread Open-Play-Stop");
            iLocalDataSource = NULL;
        }

        ~pvplayer_async_test_multiple_thread()
        {
        }

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
            STATE_GETMETADATAKEYLIST,
            STATE_GETMETADATAVALUELIST,
            STATE_RELEASEMETADATAVALUES,
            STATE_ADDDATASINK_VIDEO,
            STATE_ADDDATASINK_AUDIO,
            STATE_ADDDATASINK_TEXT,
            STATE_PREPARE,
            STATE_START,
            STATE_STOP,
            STATE_REMOVEDATASINK_VIDEO,
            STATE_REMOVEDATASINK_AUDIO,
            STATE_REMOVEDATASINK_TEXT,
            STATE_RESET,
            STATE_REMOVEDATASOURCE,
            STATE_WAIT_FOR_ERROR_HANDLING,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVPlayerInterface* iPlayer;
        PVPlayerDataSourceURL* iDataSource;
        PVPlayerDataSink* iDataSinkVideo;
        PVPlayerDataSink* iDataSinkAudio;
        PVPlayerDataSink* iDataSinkText;
        PVMFNodeInterface* iIONodeVideo;
        PVMFNodeInterface* iIONodeAudio;
        PVMFNodeInterface* iIONodeText;
        PvmiMIOControl* iMOutVideo;
        PvmiMIOControl* iMOutAudio;
        PvmiMIOControl* iMOutText;
        PVCommandId iCurrentCmdId;

        OSCL_wHeapString<OsclMemAllocator> iFileNameWStr;
        oscl_wchar iTmpWCharBuffer[512];

        PVMFLocalDataSource* iLocalDataSource;

        void PrintMetadata();
        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;
        OSCL_wHeapString<OsclMemAllocator> iSinkName;

        ThreadSafeQueue iThreadSafeCommandQueue;
        ThreadSafeQueue iThreadSafeErrorQueue;
        ThreadSafeQueue iThreadSafeInfoQueue;
        void ThreadSafeQueueDataAvailable(ThreadSafeQueue*);
        OsclSemaphore iThreadReadySem;
        OsclSemaphore iThreadExitSem;
        int32 iThreadErrors;
        OsclExecScheduler* iThreadScheduler;
};

#endif // TEST_PV_PLAYER_ENGINE_TESTSET1_H_INCLUDED

