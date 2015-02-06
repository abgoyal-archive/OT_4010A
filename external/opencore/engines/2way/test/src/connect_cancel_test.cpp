

#include "connect_cancel_test.h"


void connect_cancel_test::test()
{
    fprintf(fileoutput, "Start connect cancel test, proxy %d.\n", iUseProxy);
    int error = 0;

    scheduler = OsclExecScheduler::Current();

    this->AddToScheduler();

    if (start_async_test())
    {
        OSCL_TRY(error, scheduler->StartScheduler());
        if (error != 0)
        {
            OSCL_LEAVE(error);
        }
    }

    this->RemoveFromScheduler();
}


void connect_cancel_test::Run()
{
    if (terminal)
    {
        if (iUseProxy)
        {
            CPV2WayProxyFactory::DeleteTerminal(terminal);
        }
        else
        {
            CPV2WayEngineFactory::DeleteTerminal(terminal);
        }
        terminal = NULL;
    }

    scheduler->StopScheduler();
}

void connect_cancel_test::DoCancel()
{
}


void connect_cancel_test::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{

    switch (aEvent.GetEventType())
    {
        case PVT_INDICATION_DISCONNECT:
            iAudioSourceAdded = false;
            iVideoSourceAdded = false;
            iAudioSinkAdded = false;
            iVideoSinkAdded = false;
            break;

        case PVT_INDICATION_INTERNAL_ERROR:
            break;

        default:
            break;
    }
}
void connect_cancel_test::InitSucceeded()
{
    int error;
    test_base::InitSucceeded();
    OSCL_TRY(error, iCancelCmdId = terminal->CancelAllCommands());
    if (error)
    {
        test_is_true(false);
        reset();
    }
}

void connect_cancel_test::InitFailed()
{
}

void connect_cancel_test::ConnectCancelled()
{
}
void connect_cancel_test::DisCmdSucceeded()
{
    test_is_true(true);
    reset();
}

void connect_cancel_test::DisCmdFailed()
{
    test_is_true(true);
    reset();
}
void connect_cancel_test::CancelCmdCompleted()
{
    test_is_true(true);
    reset();
}




