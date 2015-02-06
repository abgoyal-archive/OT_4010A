
#ifndef AV_PAUSE_DISCONNECT_TEST_H_INCLUDED
#define AV_PAUSE_DISCONNECT_TEST_H_INCLUDED

#include "test_base.h"


class av_pause_disconnect_test : public test_base
{
    public:
        av_pause_disconnect_test(bool aUseProxy) : test_base(aUseProxy),
                iAudioSourcePaused(false),
                iAudioSinkPaused(false),
                iVideoSourcePaused(false),
                iVideoSinkPaused(false)
        {};

        ~av_pause_disconnect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        bool check_audio_paused()
        {
            return (iAudioSourcePaused && iAudioSinkPaused);
        }
        bool check_video_paused()
        {
            return (iVideoSourcePaused && iVideoSinkPaused);
        }

        bool iAudioSourcePaused;
        bool iAudioSinkPaused;
        bool iVideoSourcePaused;
        bool iVideoSinkPaused;
};


#endif


