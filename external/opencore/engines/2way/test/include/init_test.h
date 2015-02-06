
#ifndef INIT_TEST_H_INCLUDED
#define INIT_TEST_H_INCLUDED

#include "test_base.h"


class init_test : public test_base
{
    public:
        init_test(bool aUseProxy, int aMaxRuns, bool isSIP = false) :
                test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, aMaxRuns, isSIP) {  };

        ~init_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);


    private:
        virtual void InitSucceeded();
        virtual void InitFailed();
        virtual void RstCmdCompleted();
};


#endif


