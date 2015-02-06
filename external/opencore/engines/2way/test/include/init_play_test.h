
#ifndef INIT_PLAY_TEST_H_INCLUDED
#define INIT_PLAY_TEST_H_INCLUDED

#include "test_base.h"


class init_play_test : public test_base
{
    public:
        init_play_test(bool aUseProxy, const OSCL_wString& aFilename, int aMaxRuns) :
                test_base(aUseProxy, aMaxRuns)
        {
            iFilename = aFilename;
        };

        ~init_play_test()
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


