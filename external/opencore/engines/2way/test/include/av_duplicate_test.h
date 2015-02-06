
#ifndef AV_DUPLICATE_TEST_H_INCLUDED
#define AV_DUPLICATE_TEST_H_INCLUDED

#include "test_base.h"


class av_duplicate_test : public test_base
{
    public:
        av_duplicate_test(bool aUseProxy, int aMaxRuns) :
                test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, aMaxRuns) {};

        ~av_duplicate_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);

        void TimerCallback();

    private:
        void start_duplicates_if_ready();
        bool start_async_test();
};


#endif


