
#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#define OSCLCONFIG_UTIL_H_INCLUDED

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#include <stdio.h> //sprintf
#include <time.h>     // OSCL clock
#include <sys/time.h> // timeval

#define OSCL_CLOCK_HAS_DRIFT_CORRECTION 0
#define OSCL_HAS_SYMBIAN_TIMERS 0
#define OSCL_HAS_SYMBIAN_MATH   0

#define OSCL_RAND_MAX           RAND_MAX

//Define system sleep call for the tick count test here.
#include <unistd.h>
#define SLEEP_ONE_SEC sleep(1)


#include "osclconfig_util_check.h"

#endif

