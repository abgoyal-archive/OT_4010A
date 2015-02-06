
#ifndef TEST_PV_FRAME_METADATA_UTILITY_TESTSET1_H_INCLUDED
#define TEST_PV_FRAME_METADATA_UTILITY_TESTSET1_H_INCLUDED


#ifndef TEST_PV_FRAME_METADATA_UTILITY_H_INCLUDED
#include "test_pv_frame_metadata_utility.h"
#endif

#ifndef PV_PLAYER_DATASOURCEURL_H_INCLUDED
#include "pv_player_datasourceurl.h"
#endif

#ifndef PVMF_SOURCE_CONTEXT_DATA_H_INCLUDED
#include "pvmf_source_context_data.h"
#endif

#ifndef PV_ENGINE_TYPES_H_INCLUDED
#include "pv_engine_types.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif

#define MAX_VIDEO_FRAME_SIZE 320*240*4 // Width*Height*(4 bytes per pixel)

class pvframemetadata_async_test_newdelete : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_newdelete(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
        {
            iTestCaseName = _STRLIT_CHAR("New-Delete");
        }

        ~pvframemetadata_async_test_newdelete() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);
};

class pvframemetadata_async_test_getmetadata : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_getmetadata(PVFrameMetadataAsyncTestParam aTestParam,
                                               uint32 aMode,
                                               bool aBestThumbNailMode = false):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Get Metadata");
            iMode = aMode;
            iSourceContextData = NULL;
            iBestThumbNailMode = aBestThumbNailMode;
        }

        ~pvframemetadata_async_test_getmetadata() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();
        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETMETADATAKEYS1,
            STATE_GETMETADATAVALUES1,
            STATE_GETFRAME,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVMFSourceContextData* iSourceContextData;
        PVCommandId iCurrentCmdId;

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
        Oscl_File iFrameFile;

        uint32 iMode;
        bool iBestThumbNailMode;

        // Handle to the logger node
        PVLogger* iLogger;
        PVLogger* iPerfLogger;
};


class pvframemetadata_async_test_getfirstframemetadata : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_getfirstframemetadata(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Get First Frame and Metadata");
        }

        ~pvframemetadata_async_test_getfirstframemetadata() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();
        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETMETADATAKEYS1,
            STATE_GETMETADATAVALUES1,
            STATE_GETFRAME,
            STATE_GETMETADATAKEYS2,
            STATE_GETMETADATAVALUES2,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_getfirstframeutilitybuffer : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_getfirstframeutilitybuffer(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Get First Frame (Utility Allocated Buffer)");
        }

        ~pvframemetadata_async_test_getfirstframeutilitybuffer() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETFRAME,
            STATE_RETURNBUFFER,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;


        PVFrameSelector iFrameSelector;
        uint8* iFrameBuffer;
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_get30thframe : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_get30thframe(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Get 30th Frame");
        }

        ~pvframemetadata_async_test_get30thframe() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETFRAME,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;


        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_get10secframe : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_get10secframe(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Get Frame at 10sec");
        }

        ~pvframemetadata_async_test_get10secframe() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETFRAME,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;


        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_cancelcommand : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_cancelcommand(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Cancel Command");
        }

        ~pvframemetadata_async_test_cancelcommand() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE_QUEUED_CMDS,
            STATE_CANCELALLCOMMANDS,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;
        uint32 iPendingCmds;

        PVPMetadataList iMetadataKeyList;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[10];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
};


class pvframemetadata_async_test_multigetfirstframemetadata : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_multigetfirstframemetadata(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Multiple Get First Frame and Metadata");
        }

        ~pvframemetadata_async_test_multigetfirstframemetadata() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();
        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE1,
            STATE_GETMETADATAKEYS1,
            STATE_GETMETADATAVALUES1,
            STATE_GETFRAME1,
            STATE_REMOVEDATASOURCE1,
            STATE_ADDDATASOURCE2,
            STATE_GETMETADATAKEYS2,
            STATE_GETMETADATAVALUES2,
            STATE_GETFRAME2,
            STATE_REMOVEDATASOURCE2,
            STATE_ADDDATASOURCE3,
            STATE_GETMETADATAKEYS3,
            STATE_GETMETADATAVALUES3,
            STATE_GETFRAME3,
            STATE_REMOVEDATASOURCE3,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_multigetframe : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_multigetframe(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Multiple Get Frames");
        }

        ~pvframemetadata_async_test_multigetframe() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETFRAME1,
            STATE_GETFRAME2,
            STATE_GETFRAME3,
            STATE_GETFRAME4,
            STATE_GETFRAME5,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;


        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iFrameFile;
};


class pvframemetadata_async_test_invalidsourcefile : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_invalidsourcefile(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Invalid Source File");
        }

        ~pvframemetadata_async_test_invalidsourcefile() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
};


class pvframemetadata_async_test_nogetframe : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_nogetframe(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("No GetFrame() Call");
        }

        ~pvframemetadata_async_test_nogetframe() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETMETADATAKEYS,
            STATE_GETMETADATAVALUES,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
};


class pvframemetadata_async_test_novideotrack : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_novideotrack(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Source With No Video Track");
        }

        ~pvframemetadata_async_test_novideotrack() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_GETMETADATAKEYS1,
            STATE_GETMETADATAVALUES1,
            STATE_GETFRAME,
            STATE_GETMETADATAKEYS2,
            STATE_GETMETADATAVALUES2,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;
        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

        PVPMetadataList iMetadataKeyList;
        Oscl_Vector<PvmiKvp, OsclMemAllocator> iMetadataValueList;
        int32 iNumValues;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
};


class pvframemetadata_async_test_settimeout_getframe : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_settimeout_getframe(PVFrameMetadataAsyncTestParam aTestParam,
                uint32 aMode,
                bool aBestThumbNailMode = false):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Set Timeout and Get Frame");
            iMode = aMode;
            iSourceContextData = NULL;
            iBestThumbNailMode = aBestThumbNailMode;
        }

        ~pvframemetadata_async_test_settimeout_getframe() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void SaveMetadataInfo();
        void SaveVideoFrame();

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_QUERYINTERFACE,
            STATE_SETFRAMERETRIEVALTIMEOUT,
            STATE_GETFRAME,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;

        PvmiCapabilityAndConfig* iFMUCapConfigIF;
        PvmiKvp* iErrorKVP;
        PvmiKvp iKVPSetAsync;
        OSCL_StackString<128> iKeyStringSetAsync;

        PVPlayerDataSourceURL* iDataSource;
        PVMFSourceContextData* iSourceContextData;
        PVCommandId iCurrentCmdId;

        PVFrameSelector iFrameSelector;
        uint8 iFrameBuffer[MAX_VIDEO_FRAME_SIZE];
        uint32 iFrameBufferSize;
        PVFrameBufferProperty iFrameBufferProp;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;

        Oscl_FileServer iFS;
        Oscl_File iMetadataFile;
        char iTextOutputBuf[512];
        Oscl_File iFrameFile;

        uint32 iMode;
        bool iBestThumbNailMode;
};


class pvframemetadata_async_test_set_player_key : public pvframemetadata_async_test_base
{
    public:
        pvframemetadata_async_test_set_player_key(PVFrameMetadataAsyncTestParam aTestParam):
                pvframemetadata_async_test_base(aTestParam)
                , iFrameMetadataUtil(NULL)
                , iDataSource(NULL)
                , iCurrentCmdId(0)
        {
            iTestCaseName = _STRLIT_CHAR("Set Player Engine Key");
        }

        ~pvframemetadata_async_test_set_player_key() {}

        void StartTest();
        void Run();

        void CommandCompleted(const PVCmdResponse& aResponse);
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        enum PVTestState
        {
            STATE_CREATE,
            STATE_ADDDATASOURCE,
            STATE_QUERYINTERFACE,
            STATE_SETPLAYERKEY,
            STATE_REMOVEDATASOURCE,
            STATE_CLEANUPANDCOMPLETE
        };

        PVTestState iState;

        PVFrameAndMetadataInterface* iFrameMetadataUtil;

        PvmiCapabilityAndConfig* iFMUCapConfigIF;
        PvmiKvp* iErrorKVP;
        PvmiKvp iKVPSetAsync;
        OSCL_StackString<128> iKeyStringSetAsync;

        PVPlayerDataSourceURL* iDataSource;
        PVCommandId iCurrentCmdId;

    private:
        OSCL_wHeapString<OsclMemAllocator> wFileName;
};

#endif // TEST_PV_FRAME_METADATA_UTILITY_TESTSET1_H_INCLUDED

