
#ifndef INIT_CANCEL_TEST_H_INCLUDED
#define INIT_CANCEL_TEST_H_INCLUDED

#include "test_base.h"


class init_cancel_test : public test_base
{
    public:
        init_cancel_test(bool aUseProxy, bool isSIP = false) : test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, isSIP)
        {
            iRstCmdId  = iInitCmdId = iCnclCmdId = 0;
        }

        ~init_cancel_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);


    private:
        virtual void InitSucceeded();
        virtual void InitCancelled();
        virtual void InitFailed();
        virtual void QueryInterfaceSucceeded();

        PVCommandId iCnclCmdId;
};


#endif


