
#ifndef OSCLCONFIG_UTIL_CHECK_H_INCLUDED
#define OSCLCONFIG_UTIL_CHECK_H_INCLUDED

#ifndef OSCL_HAS_SYMBIAN_TIMERS
#error "ERROR: OSCL_HAS_SYMBIAN_TIMERS has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_MATH
#error "ERROR: OSCL_HAS_SYMBIAN_MATH has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_ANSI_MATH_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_MATH_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_CLOCK_HAS_DRIFT_CORRECTION
#error "ERROR: OSCL_CLOCK_HAS_DRIFT_CORRECTION has to be defined to either 1 or 0"
#endif

#endif // OSCLCONFIG_UTIL_CHECK_H_INCLUDED


