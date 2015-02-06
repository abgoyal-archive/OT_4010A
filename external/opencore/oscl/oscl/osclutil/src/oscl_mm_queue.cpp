
 
#include <utils/Log.h>
#undef LOG_TAG
#define LOG_TAG "Oscl_MM_Queue"
#include "oscl_mm_queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#include "mt6516_mm_queue.h"

#include "oscl_error.h"

#include "../../../../../../frameworks/base/media/libstagefright/pmem_util/sf_mm_queue.h"


OSCL_EXPORT_REF uint32 Oscl_MM_Queue::Open()
{
 
#if 0
    if(sf_pmem_Queue::Open())
        return OsclErrNone;
    else
#endif
        return OsclErrNoResources;  
}

OSCL_EXPORT_REF uint32 Oscl_MM_Queue::Close()
{
#if 0
    if(sf_pmem_Queue::Close())
        return OsclErrNone;
    else
#endif 
        return OsclErrNoResources;    
       
}

OSCL_EXPORT_REF uint32 Oscl_MM_Queue::Flush()
{
#if 0
   if(sf_pmem_Queue::Flush())
        return OsclErrNone;
    else
#endif
        return OsclErrNoResources;  

}

OSCL_EXPORT_REF uint32 Oscl_MM_Queue::PushInfo(uint8* aVAddr, uint32 aPAddr)
{
#if 0
    if(sf_pmem_Queue::PushInfo(aVAddr,aPAddr))
        return OsclErrNone;
    else
#endif
        return OsclErrNoResources;  

}

OSCL_EXPORT_REF uint32 Oscl_MM_Queue::ShowQueueInfo()
{
#if 0
    if(sf_pmem_Queue::ShowQueueInfo())
        return OsclErrNone;
    else
#endif
        return OsclErrNoResources; 
}

OSCL_EXPORT_REF uint32 Oscl_MM_Queue::MakePmemToNonCached(uint8* aVAddr)
{
#if 0
    if(sf_pmem_Queue::MakePmemToNonCached(aVAddr))
        return OsclErrNone;
    else
#endif
        return OsclErrNoResources; 
}

