
#ifndef PLAY_FROM_FILE_TEST_H_INCLUDED
#define PLAY_FROM_FILE_TEST_H_INCLUDED

#include "test_base.h"


class play_from_file_test : public test_base
{
    public:
        play_from_file_test(bool aUseProxy,
                            const OSCL_wString& aFilename,
                            int aMaxRuns) : test_base(aUseProxy, aMaxRuns)
        {
            iFilename = aFilename;
        };

        ~play_from_file_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        OSCL_wHeapString<OsclMemAllocator> iFilename;
};


#endif


