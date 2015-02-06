

// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                  O S C L _ P M E M

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_PMEM_H_INCLUDED
#define OSCL_PMEM_H_INCLUDED

#ifndef OSCL_TYPES_H_INCLUDE
#include "oscl_types.h"
#endif

#include "../../../../../../frameworks/base/media/libstagefright/pmem_util/pmem_util.h"


OSCL_IMPORT_REF void* oscl_pmem_alloc(size_t aSize);


OSCL_IMPORT_REF void  oscl_pmem_free(void* aPtr);


OSCL_IMPORT_REF void* oscl_pmem_vir2phy(void* aVirPtr);


OSCL_IMPORT_REF bool oscl_pmem_get_info(void* aPtr, PmemInfo* info);


OSCL_IMPORT_REF bool oscl_pmem_register(ExPmemInfo* info);


OSCL_IMPORT_REF bool oscl_pmem_unregister(ExPmemInfo* info);


OSCL_IMPORT_REF bool oscl_pmem_show_addr_map(void);

OSCL_IMPORT_REF bool oscl_pmem_map_into_noncached(void* aVirPtr);

#endif


/*! @} */
