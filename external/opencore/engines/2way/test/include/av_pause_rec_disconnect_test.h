
#ifndef AV_PAUSE_REC_DISCONNECT_TEST_H_INCLUDED
#define AV_PAUSE_REC_DISCONNECT_TEST_H_INCLUDED

#include "test_base.h"


class av_pause_rec_disconnect_test : public test_base
{
    public:
        av_pause_rec_disconnect_test(bool aUseProxy) : test_base(aUseProxy),
                iAudioRecPaused(false),
                iAudioStartRecId(0),
                iAudioPauseRecId(0),
                iVideoRecPaused(false),
                iVideoStartRecId(0),
                iVideoPauseRecId(0)
        {};

        ~av_pause_rec_disconnect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        bool check_rec_paused()
        {
            return (iAudioRecPaused && iVideoRecPaused);
        }

        bool iAudioRecPaused;
        TPVCmnCommandId iAudioStartRecId;
        TPVCmnCommandId iAudioPauseRecId;
        bool iVideoRecPaused;
        TPVCmnCommandId iVideoStartRecId;
        TPVCmnCommandId iVideoPauseRecId;
};


#endif


