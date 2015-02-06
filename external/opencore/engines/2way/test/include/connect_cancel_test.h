
#ifndef CONNECT_CANCEL_TEST_H_INCLUDED
#define CONNECT_CANCEL_TEST_H_INCLUDED

#include "test_base.h"


class connect_cancel_test : public test_base
{
    public:
        connect_cancel_test(bool aUseProxy) : test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy) {};

        ~connect_cancel_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);



    private:
        virtual void InitSucceeded();
        virtual void InitFailed();
        virtual void ConnectCancelled();
        virtual void DisCmdSucceeded();
        virtual void DisCmdFailed();
        virtual void CancelCmdCompleted();
};


#endif


