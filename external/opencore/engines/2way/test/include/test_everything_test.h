
#ifndef TEST_EVERYTHING_TEST_H_INCLUDED
#define TEST_EVERYTHING_TEST_H_INCLUDED

#include "test_base.h"


class test_everything_test : public test_base
{
    public:
        test_everything_test(bool aUseProxy,
                             const OSCL_wString& aFilename) :
                test_base(aUseProxy),
                iAudioRecStarted(false),
                iAudioStartRecId(0),
                iAudioStopRecId(0),
                iVideoRecStarted(false),
                iVideoStartRecId(0),
                iVideoStopRecId(0),
                iPlayStarted(false),
                iVideoPreviewStarted(false),
                iRecordFileInitialized(false)
        {
            iFilename = aFilename;
        };

        ~test_everything_test()
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

        void is_test_done();

        bool iAudioRecStarted;
        TPVCmnCommandId iAudioStartRecId;
        TPVCmnCommandId iAudioStopRecId;
        bool iVideoRecStarted;
        TPVCmnCommandId iVideoStartRecId;
        TPVCmnCommandId iVideoStopRecId;

        bool iPlayStarted;
        bool iVideoPreviewStarted;
        bool iRecordFileInitialized;

        OSCL_wHeapString<OsclMemAllocator> iFilename;
};


#endif


