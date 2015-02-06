
#ifndef TEST_PV_PLAYER_ENGINE_TESTSET_MIO_FILE_H_INCLUDED
#define TEST_PV_PLAYER_ENGINE_TESTSET_MIO_FILE_H_INCLUDED

#ifndef TEST_PV_PLAYER_ENGINE_H_INCLUDED
#include "test_pv_player_engine.h"
#endif
#ifndef PVMI_MEDIA_IO_FILEOUTPUT_H_INCLUDED
#include "pvmi_media_io_fileoutput.h"
#endif

class PVPlayerTestFileOutputMioFactory : public PVPlayerTestMioFactory
{
    public:
        PVPlayerTestFileOutputMioFactory() {}
        ~PVPlayerTestFileOutputMioFactory() {}

        PvmiMIOControl* CreateAudioOutput(OsclAny* aParam);
        PvmiMIOControl* CreateAudioOutput(OsclAny* aParam, MediaType aMediaType, bool aCompressedAudio = false);
        PvmiMIOControl* CreateAudioOutput(OsclAny* aParam, PVRefFileOutputTestObserver* aObserver, bool aActiveTiming, uint32 aQueueLimit, bool aSimFlowControl, bool logStrings = true);
        void DestroyAudioOutput(PvmiMIOControl* aMio);
        PvmiMIOControl* CreateVideoOutput(OsclAny* aParam);
        PvmiMIOControl* CreateVideoOutput(OsclAny* aParam, MediaType aMediaType, bool aCompressedVideo = false);
        PvmiMIOControl* CreateVideoOutput(OsclAny* aParam, PVRefFileOutputTestObserver* aObserver, bool aActiveTiming, uint32 aQueueLimit, bool aSimFlowControl, bool logStrings = true);
        void DestroyVideoOutput(PvmiMIOControl* aMio);
        PvmiMIOControl* CreateTextOutput(OsclAny* aParam);
        PvmiMIOControl* CreateTextOutput(OsclAny* aParam, MediaType aMediaType);
        void DestroyTextOutput(PvmiMIOControl* aMio);
};

#endif // TEST_PV_PLAYER_ENGINE_TESTSET_MIO_FILE_H_INCLUDED



