
#ifndef USE_PLAY_FILE_TEST_H_INCLUDED
#define USE_PLAY_FILE_TEST_H_INCLUDED

#include "test_base.h"


class use_play_file_test : public test_base
{
    public:
        use_play_file_test(bool aUseProxy,
                           const OSCL_wString& aFilename,
                           bool aBeforeAddSource,
                           bool aStartPlayBeforeUsePlayFile) : test_base(aUseProxy),
                iUsePlayFileBeforeAddSource(aBeforeAddSource),
                iUsePlayDone(false),
                iStartPlayBeforeUsePlayFile(aStartPlayBeforeUsePlayFile),
                iPlayStarted(false)
        {
            iFilename = aFilename;
        };

        ~use_play_file_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        void check_av_started();

        void check_test_done();

        void shutdown()
        {
            if (iIsConnected)
            {
                disconnect();
            }
            else
            {
                reset();
            }
        }

        bool iIsConnected;

        OSCL_wHeapString<OsclMemAllocator> iFilename;
        bool iUsePlayFileBeforeAddSource;
        bool iUsePlayDone;
        bool iStartPlayBeforeUsePlayFile;
        bool iPlayStarted;
};


#endif


