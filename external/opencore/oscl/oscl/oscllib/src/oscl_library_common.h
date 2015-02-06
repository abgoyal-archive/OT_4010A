
#ifndef OSCL_LIBRARY_COMMON_H_INCLUDED
#define OSCL_LIBRARY_COMMON_H_INCLUDED

#include "oscl_tickcount.h"


typedef int32 OsclLibStatus;
const OsclLibStatus OsclLibSuccess = 0;
const OsclLibStatus OsclLibFail = 1;
const OsclLibStatus OsclLibNotFound = 2;
const OsclLibStatus OsclLibOutOfMemory = 3;
const OsclLibStatus OsclLibNotSupported = 4;
const OsclLibStatus OsclLibNotLoaded = 5;

#ifndef OSCL_LIBRARY_PERF_LOGGING
//defaults for cases where the flag is not defined in the osclconfig_lib.h
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//for release builds
#define OSCL_LIBRARY_PERF_LOGGING 0
#else
//for debug builds
#define OSCL_LIBRARY_PERF_LOGGING 1
#endif
#endif


#if OSCL_LIBRARY_PERF_LOGGING
#define TICK uint32
#define SET_TICK(tick1) tick1 = OsclTickCount::TickCount()
#define DIFF_TICK(tick1,diff) TICK tick2; SET_TICK(tick2);diff = tick2 - tick1
#endif

#endif //OSCL_LIBRARY_COMMON_H_INCLUDED

