

#include "init_cancel_test.h"

void init_cancel_test::test()
{
    fprintf(fileoutput, "Start init cancel test, proxy %d.\n", iUseProxy);
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


void init_cancel_test::Run()
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

void init_cancel_test::DoCancel()
{
}


void init_cancel_test::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{
    OSCL_UNUSED_ARG(aEvent);
}

void init_cancel_test::InitSucceeded()
{
    test_is_true(true);
    reset();
}

void init_cancel_test::InitCancelled()
{
    InitSucceeded();
}

void init_cancel_test::InitFailed()
{
    test_is_true(false);
    RunIfNotReady();
}


void init_cancel_test::QueryInterfaceSucceeded()
{
    test_base::QueryInterfaceSucceeded();

    int error = 0;
    OSCL_TRY(error, iCnclCmdId = terminal->CancelAllCommands());
    if (error)
    {
        test_is_true(false);
    }

}


