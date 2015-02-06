
#ifndef ALLOC_DEALLOC_TEST_H_INCLUDED
#define ALLOC_DEALLOC_TEST_H_INCLUDED

#include "test_base.h"


class alloc_dealloc_test : public test_base
{
    public:
        alloc_dealloc_test(bool aUseProxy, bool isSIP = false) :
                test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, isSIP) {  };

        ~alloc_dealloc_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void CommandCompleted(const PVCmdResponse& aResponse);

    private:
};


#endif


