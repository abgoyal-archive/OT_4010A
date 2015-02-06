
#ifndef PVMF_META_DATA_TYPES_H_INCLUDED
#define  PVMF_META_DATA_TYPES_H_INCLUDED

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

typedef Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> PVMFMetadataList;

#endif // PVMF_META_DATA_TYPES_H_INCLUDED
