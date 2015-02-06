
#ifndef AV_DISCONNECT_RESET_REC_TEST_H_INCLUDED
#define AV_DISCONNECT_RESET_REC_TEST_H_INCLUDED

#include "test_base.h"


class av_disconnect_reset_rec_test : public test_base
{
    public:
        av_disconnect_reset_rec_test(bool aUseProxy) : test_base(aUseProxy),
                iAudioRecStarted(false),
                iAudioStartRecId(0),
                iAudioStopRecId(0),
                iVideoRecStarted(false),
                iVideoStartRecId(0),
                iVideoStopRecId(0),
                iIsDisconnected(true)
        {};

        ~av_disconnect_reset_rec_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        bool check_rec_started()
        {
            return (iAudioRecStarted && iVideoRecStarted);
        }
        bool check_rec_stopped()
        {
            return (!iAudioRecStarted && !iVideoRecStarted);
        }

        bool iAudioRecStarted;
        TPVCmnCommandId iAudioStartRecId;
        TPVCmnCommandId iAudioStopRecId;
        bool iVideoRecStarted;
        TPVCmnCommandId iVideoStartRecId;
        TPVCmnCommandId iVideoStopRecId;
        bool iIsDisconnected;
};


#endif


