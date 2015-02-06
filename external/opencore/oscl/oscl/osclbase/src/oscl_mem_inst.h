
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ M E M _ I N S T . H

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MEM_INST_H_INCLUDED
#define OSCL_MEM_INST_H_INCLUDED

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

#ifndef PVMEM_INST_LEVEL
#if defined(NDEBUG)
/* release mode-- no instrumentation */
#define PVMEM_INST_LEVEL 0
#else
/* debug mode-- full instrumentation */
#define PVMEM_INST_LEVEL 1
#endif
#endif

#if(OSCL_BYPASS_MEMMGT)
#undef PVMEM_INST_LEVEL
#define PVMEM_INST_LEVEL 0
#endif


/*! @} */

#endif
