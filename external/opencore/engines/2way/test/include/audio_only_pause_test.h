
#ifndef AUDIO_ONLY_PAUSE_TEST_H_INCLUDED
#define AUDIO_ONLY_PAUSE_TEST_H_INCLUDED

#include "test_base.h"


class audio_only_pause_test : public test_base
{
    public:
        audio_only_pause_test(bool aUseProxy, int aMaxRuns) : test_base(aUseProxy, aMaxRuns),
                iAudioSourceResumed(false),
                iAudioSinkResumed(false)
        {};

        ~audio_only_pause_test()
        {};

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        bool check_audio_resumed()
        {
            return (iAudioSourceResumed && iAudioSinkResumed);
        }

        bool iAudioSourceResumed;
        bool iAudioSinkResumed;
};


#endif


