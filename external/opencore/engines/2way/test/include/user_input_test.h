
#ifndef USER_INPUT_TEST_H_INCLUDED
#define USER_INPUT_TEST_H_INCLUDED

#include "test_base.h"
#include "tsc_h324m_config_interface.h"

class user_input_test : public test_base//,
        //public H324MConfigObserver
{
    public:
        user_input_test(bool aUseProxy, bool aIsDTMF) : test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy),
                iIsDTMF(aIsDTMF)
                //,iUserInput(NULL)
        {};

        ~user_input_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);

        void H324MConfigCommandCompletedL(PVMFCmdResp& aResponse);

        void H324MConfigHandleInformationalEventL(PVMFAsyncEvent& aNotification);


    private:
        virtual void RstCmdCompleted();
        virtual void DisCmdSucceeded();
        virtual void DisCmdFailed();
        virtual void ConnectSucceeeded();
        virtual void InitFailed();
        bool start_async_test();
        PVInterface *i324mConfigInterface;
        bool iIsDTMF;
        PVCommandId i324mIFCommandId, iUserInputId;

        CPVUserInput *iUserInput;
};


#endif


