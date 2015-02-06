
#ifndef PFF_EOS_TEST_H_INCLUDED
#define PFF_EOS_TEST_H_INCLUDED

#include "test_base.h"


class pff_eos_test : public test_base
{
    public:
        pff_eos_test(bool aUseProxy,
                     const OSCL_wString& aFilename,
                     bool aWaitForAudioEOS,
                     bool aWaitForVideoEOS,
                     int aMaxRuns) : test_base(aUseProxy, aMaxRuns),
                iWaitForAudioEOS(aWaitForAudioEOS),
                iWaitForVideoEOS(aWaitForVideoEOS),
                iAudioEOSRecv(false),
                iVideoEOSRecv(false),
                iStopPlayLCalled(false)
        {
            iFilename = aFilename;
        };

        ~pff_eos_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        void check_eos();

        OSCL_wHeapString<OsclMemAllocator> iFilename;

        bool iWaitForAudioEOS;
        bool iWaitForVideoEOS;

        bool iAudioEOSRecv;
        bool iVideoEOSRecv;
        bool iStopPlayLCalled;
};


#endif


