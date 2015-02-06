
#ifndef A_ONLY_PAUSE_REC_TEST_H_INCLUDED
#define A_ONLY_PAUSE_REC_TEST_H_INCLUDED

#include "test_base.h"


class a_only_pause_rec_test : public test_base
{
    public:
        a_only_pause_rec_test(bool aUseProxy, int aMaxRuns) : test_base(aUseProxy, aMaxRuns) {};

        ~a_only_pause_rec_test()
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


