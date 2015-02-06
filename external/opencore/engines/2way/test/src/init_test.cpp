
#include "init_test.h"



void init_test::test()
{
    fprintf(fileoutput, "Start init test, num runs %d, proxy %d.\n", iMaxRuns, iUseProxy);
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


void init_test::Run()
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

void init_test::DoCancel()
{
}


void init_test::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{
    OSCL_UNUSED_ARG(aEvent);
}

void init_test::InitSucceeded()
{
    iRstCmdId = 0;
    int error;
    OSCL_TRY(error, iRstCmdId =  terminal->Reset());
    if (error)
    {
        test_is_true(false);
        RunIfNotReady();
    }
    else
    {
        if (iCurrentRun >= (iMaxRuns - 1))
        {
            test_is_true(true);
        }
    }
}

void init_test::InitFailed()
{
    test_is_true(false);
    RunIfNotReady();
}

void init_test::RstCmdCompleted()
{
    iCurrentRun++;
    if (iCurrentRun < iMaxRuns)
    {
        //iInitCmdId = 0;
        Init();
    }
    else
    {
        destroy_sink_source();
        RunIfNotReady();
    }
}





