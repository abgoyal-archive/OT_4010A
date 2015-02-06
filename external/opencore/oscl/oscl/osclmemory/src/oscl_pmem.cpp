

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_PMEM_H_INCLUDED
#include "oscl_pmem.h"
#endif

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#ifndef OSCL_MUTEX_H_INCLUDED
#include "oscl_mutex.h"
#endif

#include <utils/Log.h>
#undef  LOG_TAG
#define LOG_TAG "oscl_pmem"


#ifdef USES_PMEM_IN_OSCL


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/android_pmem.h>
#include "../../../../../../frameworks/base/media/libstagefright/pmem_util/pmem_util.h"

void* oscl_pmem_alloc(size_t aSize)
{
    
    return sf_pmem_alloc(aSize);   
}


void  oscl_pmem_free(void* aPtr)
{
     sf_pmem_free(aPtr);
}


void* oscl_pmem_vir2phy(void* aVirPtr)
{
    return (void*)sf_pmem_vir2phy(aVirPtr,-1);
}


bool oscl_pmem_get_info(void* aPtr, PmemInfo* info)
{
    sf_pmem_get_info(aPtr, info);

    return true;
}


bool oscl_pmem_register(ExPmemInfo* info)
{
    sf_pmem_register(info);
    return true;
}


bool oscl_pmem_unregister(ExPmemInfo* info)
{
    sf_pmem_unregister(info);
    return true;
}


bool oscl_pmem_show_addr_map(void)
{
    sf_pmem_show_addr_map();
    return true;
}

bool oscl_pmem_map_into_noncached(void* aVirPtr)
{
    return sf_pmem_map_into_noncached(aVirPtr);
}


#else  // !USES_PMEM_IN_OSCL


void* oscl_pmem_alloc(size_t aSize)
{
    return OSCL_MALLOC(aSize);
}


void  oscl_pmem_free(void* aPtr)
{
    OSCL_FREE(aPtr);
}


void* oscl_pmem_vir2phy(void* aVirPtr)
{
    return aVirPtr;
}


bool oscl_pmem_get_info(void* aPtr, PmemInfo* info)
{
    return false;
}

bool oscl_pmem_register(ExPmemInfo* info)
{
    return true;
}

bool oscl_pmem_unregister(ExPmemInfo* info)
{
    return true;
}

bool oscl_pmem_show_addr_map(void)
{
    return true;
}

bool oscl_pmem_map_into_noncached(void* aVirPtr)
{
    return true;
}

#endif //!USES_PMEM_IN_OSCL

