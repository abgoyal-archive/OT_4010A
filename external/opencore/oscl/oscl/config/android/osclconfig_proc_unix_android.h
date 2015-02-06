
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//     O S C L C O N F I G _ P R O C  ( P L A T F O R M   C O N F I G   I N F O )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCLCONFIG_PROC_UNIX_ANDROID_H_INCLUDED
#define OSCLCONFIG_PROC_UNIX_ANDROID_H_INCLUDED

#define OSCL_HAS_SYMBIAN_SCHEDULER 0

#define OSCL_HAS_THREAD_SUPPORT 1
#define OSCL_HAS_NON_PREEMPTIVE_THREAD_SUPPORT 0

//semaphore with advanced realtime features incl. timed wait.
#define OSCL_HAS_SEM_TIMEDWAIT_SUPPORT 0
//#include <time.h>
//#include <semaphore.h>

//pthreads
#define OSCL_HAS_PTHREAD_SUPPORT 1
#include <pthread.h>
#include <errno.h>
#include <signal.h>

// threads, mutex, semaphores
typedef pthread_t TOsclThreadId;
typedef void* TOsclThreadFuncArg;
typedef void* TOsclThreadFuncRet;
#define OSCL_THREAD_DECL
typedef pthread_t TOsclThreadObject;
typedef pthread_mutex_t TOsclMutexObject;
typedef int TOsclSemaphoreObject;
typedef pthread_cond_t TOsclConditionObject;

#endif
