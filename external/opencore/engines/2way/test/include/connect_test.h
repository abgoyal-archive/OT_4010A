
#ifndef CONNECT_TEST_H_INCLUDED
#define CONNECT_TEST_H_INCLUDED

#include "test_base.h"


class connect_test : public test_base
{
    public:
        connect_test(bool aUseProxy, int aMaxRuns, bool runTimerTest = false) :
                test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, aMaxRuns) ,
                iRunTimerTest(runTimerTest),
                i324mConfigInterface(NULL),
                iMP4H263EncoderInterface(NULL) {};

        ~connect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);


    private:
        virtual void InitSucceeded();
        virtual void InitFailed();
        virtual void EncoderIFSucceeded();
        virtual void EncoderIFFailed();
        virtual void ConnectFailed();
        virtual void ConnectCancelled();
        virtual void DisCmdSucceeded();
        virtual void DisCmdFailed();

        bool iRunTimerTest;
        PVInterface *i324mConfigInterface;
        PVInterface *iMP4H263EncoderInterface;
};


#endif


