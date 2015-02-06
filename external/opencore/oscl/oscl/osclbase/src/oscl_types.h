
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//       O S C L _ T Y P E S   ( B A S I C   T Y P E D E F S )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_TYPES_H_INCLUDED
#define OSCL_TYPES_H_INCLUDED


// include the config header for the platform
#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

//! The c_bool type is mapped to an integer to provide a bool type for C interfaces
typedef int c_bool;


//! The OsclAny is meant to be used the context of a generic pointer (i.e., no specific type).
typedef void OsclAny;

//! mbchar is multi-byte char (e.g., UTF-8) with null termination.
typedef char mbchar;

//! The uint type is a convenient abbreviation for unsigned int.
#if !defined(__USE_MISC)
// uint is defined in some Linux platform sys\types.h
typedef unsigned int uint;
#endif

//! The octet type is meant to be used for referring to a byte or collection bytes without suggesting anything about the underlying meaning of the bytes.
typedef uint8 octet;

//! The Float type defined as OsclFloat
typedef float OsclFloat;

#ifndef OSCL_INT64_TYPES_DEFINED
//use native type
typedef OSCL_NATIVE_INT64_TYPE int64;
//use native type
typedef OSCL_NATIVE_UINT64_TYPE uint64;
#define OSCL_INT64_TYPES_DEFINED
#endif

// define OSCL_WCHAR
typedef OSCL_NATIVE_WCHAR_TYPE oscl_wchar;

//! define OSCL_TCHAR
typedef oscl_wchar OSCL_TCHAR;

// The definition of the MemoryFragment will probably
// be OS-dependant since the goal is to allow this data
// structure to be passed directly to I/O routines that take
// scatter/gather arrays.
#if ( OSCL_MEMFRAG_PTR_BEFORE_LEN )

struct OsclMemoryFragment
{
    void *ptr;
    uint32 len;
};

#else
struct OsclMemoryFragment
{
    uint32 len;
    void *ptr;
};
#endif


/*! @} */


#endif  // OSCL_TYPES_H_INCLUDED
