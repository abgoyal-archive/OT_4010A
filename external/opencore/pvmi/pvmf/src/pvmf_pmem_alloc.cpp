

/////////////////////////////////////////////////////////////////////////////////////////////////
// Ryan Lin !!!
///////////////////////////////////////////////////////////////////////////////////////////////////////
// for allocate physical continuous mem

#include "pvmf_pmem_alloc.h"

// ---------------------------------------------------------------------
//
// Ryan Lin !!
#undef LOG_TAG
#define LOG_TAG "PVMFPmemMemAlloc"

#ifdef PMEM_NON_CACHED
#include "oscl_mm_queue.h"
#endif

#ifndef _WIN32
#include <utils/Log.h>
#define LOG_PROFILE(...) LOGV(__VA_ARGS__)

#undef PVLOGGER_LOGMSG
#define PVLOGGER_LOGMSG(IL, LOGGER, LEVEL, MESSAGE)\
{\
    if (LEVEL == PVLOGMSG_ERR)\
    {\
        ErrMSG MESSAGE;\
    }\
    else\
    {\
        MSG MESSAGE;\
    }\
}
#define ErrMSG(x, ...) LOGE(__VA_ARGS__)
#define MSG(x, ...) LOGV(__VA_ARGS__)
#else

#define LOGD(...)
#define LOGV(...)
#define LOGE(...)

#endif
// ---------------------------------------------------------------------

#ifndef _WIN32
#define USE_PMEM 1
#else
#define USE_PMEM 0
#endif

#if USE_PMEM
#include "oscl_pmem.h"
#endif

#ifndef _WIN32
#if 0
#define LOGV(...) LOGD(__VA_ARGS__)
#endif
#else
#define LOGV(...)
#define LOGD(...)
#endif

#define PVMF_TRYALLOCMEMCOUNT    200

void* PVMFPmemMemAlloc::allocate(const uint32 size)
{
    unsigned int TryAllocMemCount = PVMF_TRYALLOCMEMCOUNT;
    char* p;
    
    while(TryAllocMemCount)
    {
#if USE_PMEM
        LOGV("Use pmem for OMX, size: %d", size);
        p = (char*)oscl_pmem_alloc(size);
#else
        LOGV("Use malloc for OMX, size: %d", size);
        p = (char*)OSCL_MALLOC(size);
#endif
        if (p == NULL)
        {
            LOGE("PVMFPmemMemAlloc::allocate %d size fail", size);
            LOGE("Alloc %d times!!, Try alloc again!!", (PVMF_TRYALLOCMEMCOUNT-TryAllocMemCount));
            TryAllocMemCount--;
            usleep(10*1000);
        }
        else
        {
            LOGV("allocate %d size memory address: 0x0%X", size, p);
            break;
        }
    }

    if (p == NULL)
    {
        LOGE("Alloc p fail %d times!!, Return error!!", PVMF_TRYALLOCMEMCOUNT);
    }    
#ifdef PMEM_NON_CACHED
    oscl_pmem_map_into_noncached((void*)p);
#endif
    
    return (OsclAny*)p;
}

void PVMFPmemMemAlloc::deallocate(OsclAny* p)
{
    LOGV("Free addr: 0x0%x", p);
#if USE_PMEM
    oscl_pmem_free((void*)p);
#else
    OSCL_FREE(p);
#endif
}



