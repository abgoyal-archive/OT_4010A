
#ifndef INVALID_COMMAND_TEST_H_INCLUDED
#define INVALID_COMMAND_TEST_H_INCLUDED

#include "test_base.h"


class invalid_command_test : public test_base
{
    public:
        invalid_command_test() {};

        ~invalid_command_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);

    private:
        bool test_command(TPV2WayCommandType cmd);
};


#endif


