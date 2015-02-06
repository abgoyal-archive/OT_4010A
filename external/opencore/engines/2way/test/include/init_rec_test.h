
#ifndef INIT_REC_TEST_H_INCLUDED
#define INIT_REC_TEST_H_INCLUDED

#include "test_base.h"


class init_rec_test : public test_base
{
    public:
        init_rec_test(bool aUseProxy, int aMaxRuns) : test_base(aUseProxy, aMaxRuns) {};

        ~init_rec_test()
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


