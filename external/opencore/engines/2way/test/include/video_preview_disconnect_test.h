
#ifndef VIDEO_PREVIEW_DISCONNECT_TEST_H_INCLUDED
#define VIDEO_PREVIEW_DISCONNECT_TEST_H_INCLUDED

#include "test_base.h"


class video_preview_disconnect_test : public test_base
{
    public:
        video_preview_disconnect_test(bool aUseProxy) : test_base(aUseProxy) {};

        ~video_preview_disconnect_test()
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


