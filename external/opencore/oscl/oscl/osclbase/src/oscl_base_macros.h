
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L B A S E _ M A C R O S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_BASE_MACROS_H_INCLUDED
#define OSCL_BASE_MACROS_H_INCLUDED

// Pick up any platform-specific definitions for the common
// macros.
#include "osclconfig.h"

// Define default values for the common macros
#ifndef OSCL_EXPORT_REF
#define OSCL_EXPORT_REF
#endif

#ifndef OSCL_IMPORT_REF
#define OSCL_IMPORT_REF
#endif

//! The NULL_TERM_CHAR is used to terminate c-style strings.
//static const char NULL_TERM_CHAR = '\0';
#ifndef NULL_TERM_CHAR
#define NULL_TERM_CHAR '\0'
#endif

//! if the NULL macro isn't already defined, then define it as zero.
#ifndef NULL
#define NULL (0)
#endif

#if (OSCL_DISABLE_INLINES)
#define OSCL_INLINE
#define OSCL_COND_EXPORT_REF OSCL_EXPORT_REF
#define OSCL_COND_IMPORT_REF OSCL_IMPORT_REF
#else
#define OSCL_INLINE inline
#define OSCL_COND_EXPORT_REF
#define OSCL_COND_IMPORT_REF
#endif

//this macro may not be defined in all configurations
//so a default is defined here.

//! Type casting macros

#define OSCL_CONST_CAST(type,exp)           ((type)(exp))
#define OSCL_STATIC_CAST(type,exp)          ((type)(exp))
#define OSCL_REINTERPRET_CAST(type,exp)     ((type)(exp))
#define OSCL_DYNAMIC_CAST(type, exp)        ((type)(exp))


#define OSCL_UNUSED_ARG(vbl) (void)(vbl)
#define OSCL_UNUSED_RETURN(value) return value

#ifndef __TFS__
#define __TFS__
#endif

#define OSCL_MIN(a,b) ((a) < (b) ? (a) : (b))
#define OSCL_MAX(a,b) ((a) > (b) ? (a) : (b))
#define OSCL_ABS(a) ((a) > (0) ? (a) : -(a))

// the syntax for explicitly calling the destructor varies on some platforms
// below is the default syntax as defined in the C++ standard
#ifndef OSCL_TEMPLATED_DESTRUCTOR_CALL
#define OSCL_TEMPLATED_DESTRUCTOR_CALL(type,simple_type) type :: ~simple_type ()
#endif



#ifndef OSCL_UNSIGNED_CONST
#define OSCL_UNSIGNED_CONST(x) x
#endif

#ifndef OSCL_PACKED_VAR
#define OSCL_PACKED_VAR     "error"
#endif

#ifndef OSCL_BEGIN_PACKED
#define OSCL_BEGIN_PACKED   "error"
#endif

#ifndef OSCL_END_PACKED
#define OSCL_END_PACKED     "error"
#endif

/*! @} */

#endif  // OSCL_BASE_MACROS_H_INCLUDED
