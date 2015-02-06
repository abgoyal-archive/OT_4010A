
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//             O S C L C O N F I G _ M E M O R Y

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#define OSCLCONFIG_MEMORY_H_INCLUDED


#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#ifndef OSCLCONFIG_ANSI_MEMORY_H_INCLUDED
#include "osclconfig_ansi_memory.h"
#endif

#ifdef NDEBUG
#define OSCL_BYPASS_MEMMGT 1
#else
#define OSCL_BYPASS_MEMMGT 0
#endif



#if(OSCL_RELEASE_BUILD)
#define OSCL_HAS_GLOBAL_NEW_DELETE 0
#define PVMEM_INST_LEVEL 0
#else
#define OSCL_HAS_GLOBAL_NEW_DELETE 1
#define PVMEM_INST_LEVEL 1
#endif

#if(OSCL_HAS_GLOBAL_NEW_DELETE)
//Detect if <new> or <new.h> is included anyplace to avoid a compile error.
#if defined(_INC_NEW)
#error Duplicate New Definition!
#endif //_INC_NEW
#if defined(_NEW_)
#error Duplicate New Definition!
#endif //_NEW_
#endif //OSCL_HAS_GLOBAL_NEW_DELETE

#ifdef __cplusplus
#include <new> //for placement new
#endif //__cplusplus

//OSCL_HAS_HEAP_BASE_SUPPORT - Enables or disables overloaded memory operators in HeapBase class
#define OSCL_HAS_HEAP_BASE_SUPPORT 1


#define OSCL_HAS_SYMBIAN_MEMORY_FUNCS 0


#include "osclconfig_memory_check.h"


#endif
