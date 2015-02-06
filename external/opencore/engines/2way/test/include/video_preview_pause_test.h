
#ifndef VIDEO_PREVIEW_PAUSE_TEST_H_INCLUDED
#define VIDEO_PREVIEW_PAUSE_TEST_H_INCLUDED

#include "test_base.h"


class video_preview_pause_test : public test_base
{
    public:
        video_preview_pause_test(bool aUseProxy, int aMaxRuns) : test_base(aUseProxy, aMaxRuns) {};

        ~video_preview_pause_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();
};


#endif


