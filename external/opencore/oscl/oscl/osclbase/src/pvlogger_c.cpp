

#include "pvlogger.h"
#include "pvlogger_c.h"



//C-callable version of PVLogger::GetLoggerObject
OSCL_EXPORT_REF void* pvLogger_GetLoggerObject(const char* tag)
{
    return PVLogger::GetLoggerObject(tag);
}

//C-callable version of PVLogger::IsActive
OSCL_EXPORT_REF int pvLogger_IsActive(void* logger, int log_level)
{
    if (logger)
        return (int)((PVLogger*)logger)->IsActive(log_level);
    return 0;
}

//C-callable version of PVLogger::LogMsgString
OSCL_EXPORT_REF void pvLogger_LogMsgString(void* logger, int msgID, const char * fmt, ...)
{
    if (logger)
    {
        va_list arguments;
        va_start(arguments, fmt);

        ((PVLogger*)logger)->LogMsgStringV(msgID, fmt, arguments);
    }
}
