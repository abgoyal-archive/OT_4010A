
#ifndef typedef_h
#define typedef_h "$Id $"

#undef ORIGINAL_TYPEDEF_H /* CHANGE THIS TO #define to get the      */
/*  "original" ETSI version of typedef.h  */
/* CHANGE TO #undef for PV version        */

#ifdef ORIGINAL_TYPEDEF_H

#if   defined(__unix__) || defined(__unix)
typedef signed char Word8;
typedef short Word16;
typedef int Word32;
typedef int Flag;

#else
#error No System recognized
#endif
#else /* not original typedef.h */

#include "gsm_amr_typedefs.h"

#endif

#endif
