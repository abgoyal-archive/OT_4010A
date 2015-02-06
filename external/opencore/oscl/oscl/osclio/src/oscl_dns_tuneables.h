

#ifndef OSCL_DNS_TUNEABLES_H_INCLUDED
#define OSCL_DNS_TUNEABLES_H_INCLUDED



#include "osclconfig_io.h"
#include "osclconfig_proc.h"

#ifndef PV_DNS_SERVER
#define PV_DNS_SERVER 1
#endif

#if(PV_DNS_SERVER)

#if !defined PV_DNS_IS_THREAD
#define PV_DNS_IS_THREAD OSCL_HAS_THREAD_SUPPORT
#endif

//for test-- to force AO implementation
//#undef PV_DNS_IS_THREAD
//#define PV_DNS_IS_THREAD 0


#endif//PV_DNS_SERVER
#endif

