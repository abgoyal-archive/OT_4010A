
#ifndef VIDEO_ONLY_PAUSE_CLOSE_TEST_H_INCLUDED
#define VIDEO_ONLY_PAUSE_CLOSE_TEST_H_INCLUDED

#include "test_base.h"


class video_only_pause_close_test : public test_base
{
    public:
        video_only_pause_close_test(bool aUseProxy) : test_base(aUseProxy) {};

        ~video_only_pause_close_test()
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


