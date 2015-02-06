
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//     O S C L C O N F I G_ L I M I T S _ T Y P E D E F S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCLCONFIG_LIMITS_TYPEDEFS_H_INCLUDED
#define OSCLCONFIG_LIMITS_TYPEDEFS_H_INCLUDED


#include <limits.h>

// determine if char is signed or unsigned
#if ( CHAR_MIN == 0 )
#define OSCL_CHAR_IS_UNSIGNED 1
#define OSCL_CHAR_IS_SIGNED   0
#elif ( CHAR_MIN == SCHAR_MIN )
#define OSCL_CHAR_IS_UNSIGNED 0
#define OSCL_CHAR_IS_SIGNED   1
#else
#error "Cannot determine if char is signed or unsigned"
#endif


#if ( (CHAR_MAX == 255) || (CHAR_MAX == 127) )
typedef signed char int8;
typedef unsigned char uint8;
#else
#error "Cannot determine an 8-bit interger type"
#endif


#if ( SHRT_MAX == 32767 )
typedef short int16;
typedef unsigned short uint16;

#elif ( INT_MAX == 32767 )
typedef int int16;
typedef unsigned int uint16;

#else
#error "Cannot determine 16-bit integer type"
#endif



#if ( INT_MAX == 2147483647 )
typedef int int32;
typedef unsigned int uint32;

#elif ( LONG_MAX == 2147483647 )
typedef long int32;
typedef unsigned long uint32;

#else
#error "Cannot determine 32-bit integer type"
#endif



#endif
