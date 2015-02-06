

#ifndef OSCLCONFIG_PROC_CHECK_H_INCLUDED
#define OSCLCONFIG_PROC_CHECK_H_INCLUDED

#ifndef OSCL_HAS_THREAD_SUPPORT
#error "ERROR: OSCL_HAS_THREAD_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_NON_PREEMPTIVE_THREAD_SUPPORT
#error "ERROR: OSCL_HAS_NON_PREEMPTIVE_THREAD_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_SCHEDULER
#error "ERROR: OSCL_HAS_SYMBIAN_SCHEDULER has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SEM_TIMEDWAIT_SUPPORT
#error "ERROR: OSCL_HAS_SEM_TIMEDWAIT_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_PTHREAD_SUPPORT
#error "ERROR: OSCL_HAS_PTHREAD_SUPPORT has to be defined to either 1 or 0"
#endif

typedef TOsclThreadId __verify__TOsclThreadId__defined__;

typedef TOsclThreadFuncRet __verify__TOsclThreadFuncRet__defined__;

typedef TOsclThreadFuncArg __verify__TOsclThreadFuncArg__defined__;

#ifndef OSCL_THREAD_DECL
#error "ERROR: OSCL_THREAD_DECL has to be defined."
#endif


typedef TOsclThreadObject __verify__TOsclThreadObject__defined__;

typedef TOsclMutexObject __verify__TOsclMutexObject__defined__;

typedef TOsclSemaphoreObject __verify__TOsclSemaphoreObject__defined__;

typedef TOsclConditionObject __verify__TOsclConditionObject__defined__;


#endif //OSCLCONFIG_PROC_CHECK_H_INCLUDED


