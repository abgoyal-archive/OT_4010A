
#ifndef INIT_REC_RESET_TEST_H_INCLUDED
#define INIT_REC_RESET_TEST_H_INCLUDED

#include "test_base.h"


class init_rec_reset_test : public test_base
{
    public:
        init_rec_reset_test(bool aUseProxy) : test_base(aUseProxy) {};

        ~init_rec_reset_test()
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


