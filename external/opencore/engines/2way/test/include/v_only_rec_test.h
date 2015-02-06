
#ifndef V_ONLY_REC_TEST_H_INCLUDED
#define V_ONLY_REC_TEST_H_INCLUDED

#include "test_base.h"


class v_only_rec_test : public test_base
{
    public:
        v_only_rec_test(bool aUseProxy, int aMaxRuns) : test_base(aUseProxy, aMaxRuns) {};

        ~v_only_rec_test()
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


