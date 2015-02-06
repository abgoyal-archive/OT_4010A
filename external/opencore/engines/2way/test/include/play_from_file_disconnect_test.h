
#ifndef PLAY_FROM_FILE_DISCONNECT_TEST_H_INCLUDED
#define PLAY_FROM_FILE_DISCONNECT_TEST_H_INCLUDED

#include "test_base.h"


class play_from_file_disconnect_test : public test_base
{
    public:
        play_from_file_disconnect_test(bool aUseProxy,
                                       const OSCL_wString& aFilename,
                                       bool aBeforeAddSource) :
                test_base(aUseProxy),
                iUsePlayFileBeforeAddSource(aBeforeAddSource)
        {
            iFilename = aFilename;
        };

        ~play_from_file_disconnect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        void is_av_started();

        OSCL_wHeapString<OsclMemAllocator> iFilename;
        bool iUsePlayFileBeforeAddSource;
};


#endif


