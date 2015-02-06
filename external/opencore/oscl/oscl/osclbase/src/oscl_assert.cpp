
#include "oscl_assert.h"
#include "osclconfig.h"

#if (OSCL_DISABLE_INLINES)
#include "oscl_assert.inl"
#endif

#include "pvlogger.h"

OSCL_EXPORT_REF void OSCL_Assert(const char *expression, const char *filename, int line_number)
{
#if(OSCL_HAS_ERROR_HOOK)
    //If there's an error hook installed, just log and jump there.
    //This is mainly for unit test support, but conceivably some platform
    //might also have an error hook.
    int32 error;
    OsclAny* ptr = OsclTLSRegistry::getInstance(OSCL_TLS_ID_ERRORHOOK, error);
    if (ptr)
    {
        //log the assertion.
        PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, PVLogger::GetLoggerObject("OSCL_Assert"), PVLOGMSG_EMERG
                        , (0, "Assertion failure for: %s, at %s:%d", expression, filename, line_number));
        //jump to the error hook
        longjmp(*((jmp_buf*)ptr), -1);
    }
#endif

    //Print to standard debug output...


    //default
    fprintf(stderr, "Assertion failure for: %s, at %s:%d\n", expression, filename, line_number);


    //log the assertion.
    //this is risky if OsclBase is un-initialized, so do it last.
    PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, PVLogger::GetLoggerObject("OSCL_Assert"), PVLOGMSG_EMERG
                    , (0, "Assertion failure for: %s, at %s:%d", expression, filename, line_number));

    // ...then abort
    _OSCL_Abort();
}
