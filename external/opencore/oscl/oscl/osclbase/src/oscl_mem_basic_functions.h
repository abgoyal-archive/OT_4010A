
/* -*- c++ -*- */
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//           O S C L _ M E M _ B A S I C _ F U N C T I O N S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#define OSCL_MEM_BASIC_FUNCTIONS_H

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

OSCL_COND_IMPORT_REF void* _oscl_malloc(int32 count);

OSCL_COND_IMPORT_REF void* _oscl_calloc(int32 nelems, int32 size);


OSCL_COND_IMPORT_REF void* _oscl_realloc(void* src, int32 count);

OSCL_COND_IMPORT_REF void  _oscl_free(void* src);

OSCL_COND_IMPORT_REF void* oscl_memcpy(void* dest, const void* src, uint32 count);

OSCL_COND_IMPORT_REF void* oscl_memmove(void* dest, const void* src, uint32 count);

OSCL_COND_IMPORT_REF void* oscl_memmove32(void* dest, const void* src, uint32 count);

OSCL_COND_IMPORT_REF void* oscl_memset(void* dest, uint8 val, uint32 count);

OSCL_COND_IMPORT_REF int oscl_memcmp(const void* buf1, const void* buf2, uint32 count);


#if (!OSCL_DISABLE_INLINES)
#include "oscl_mem_basic_functions.inl"
#endif

#endif

/*! @} */
