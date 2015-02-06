
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//     O S C L C O N F I G _ P R O C  ( P L A T F O R M   C O N F I G   I N F O )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCLCONFIG_PROC_H_INCLUDED
#define OSCLCONFIG_PROC_H_INCLUDED

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#include "osclconfig_proc_unix_android.h"

#include "osclconfig_proc_check.h"

#include <sys/prctl.h>
#include <utils/threads.h>
#include <sys/time.h>
#include <sys/resource.h>
#define OSCL_SET_THREAD_NAME(name) prctl(PR_SET_NAME,(unsigned long)name,0,0,0)

#endif
