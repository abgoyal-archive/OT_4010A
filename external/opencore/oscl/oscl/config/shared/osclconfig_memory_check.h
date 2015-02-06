

#ifndef OSCLCONFIG_MEMORY_CHECK_H_INCLUDED
#define OSCLCONFIG_MEMORY_CHECK_H_INCLUDED


#ifndef OSCL_BYPASS_MEMMGT
#error "ERROR: OSCL_BYPASS_MEMMGT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_ANSI_MEMORY_FUNCS
#error "ERROR: OSCL_HAS_ANSI_MEMORY_FUNCS has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_MEMORY_FUNCS
#error "ERROR: OSCL_HAS_SYMBIAN_MEMORY_FUNCS has to be defined to either 1 or 0"
#endif


#ifndef OSCL_HAS_HEAP_BASE_SUPPORT
#error "ERROR: OSCL_HAS_HEAP_BASE_SUPPORT has to be defined to either 1 or 0."
#endif


#ifndef OSCL_HAS_GLOBAL_NEW_DELETE
#error "ERROR: OSCL_HAS_GLOBAL_NEW_DELETE has to be defined to either 1 or 0."
#endif

#endif // OSCLCONFIG_MEMORY_CHECK_H_INCLUDED


