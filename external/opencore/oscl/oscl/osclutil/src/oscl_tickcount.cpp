
#include "oscl_tickcount.h"

#if (OSCL_DISABLE_INLINES)
#include "oscl_tickcount.inl"
#endif

#include <utils/Log.h>
#undef LOG_TAG
#define LOG_TAG "OSCLTickcount"

#define SHOW_PROFILE 0

void PrintTime(long long ms)
{
#if SHOW_PROFILE 
    LOGD("!!!!! current time = %lld ms !!!!!", ms);
#endif
}

