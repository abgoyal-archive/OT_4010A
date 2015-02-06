
#include "oscl_trace.h"
#include <utils/Log.h>

#define ENABLE_FTRACE 0

#if ENABLE_FTRACE
#include "MediaHal.h"
#include "mhal_misc.h"
#endif

#undef LOG_TAG
#define LOG_TAG "OsclTrace"

OSCL_EXPORT_REF void Oscl_Trace::begin()
{
    LOGD("Oscl_Trace::begin");
#if ENABLE_FTRACE    
    mhalSchedLog(1);
#endif
}

OSCL_EXPORT_REF void Oscl_Trace::end()
{    
    LOGD("Oscl_Trace::end");
#if ENABLE_FTRACE    
    mhalSchedLog(0);
#endif
}
