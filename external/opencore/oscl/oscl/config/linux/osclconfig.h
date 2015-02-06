
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//     O S C L C O N F I G   ( P L A T F O R M   C O N F I G   I N F O )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCLCONFIG_H_INCLUDED
#define OSCLCONFIG_H_INCLUDED

// system includes for dynamic registry
#include <dirent.h>
#include <dlfcn.h>

// include common include for determining sizes from limits.h
#include "osclconfig_limits_typedefs.h"

//This switch turns off some profiling and debug settings
#ifdef NDEBUG
#define OSCL_RELEASE_BUILD 1
#else
#define OSCL_RELEASE_BUILD 0
#endif

// include common unix definitions
#include "osclconfig_unix_common.h"

// define the suffix for unsigned constants
#define OSCL_UNSIGNED_CONST(x) x##u

// override the common definition for
#undef OSCL_NATIVE_UINT64_TYPE
#define OSCL_NATIVE_UINT64_TYPE    u_int64_t

// include the definitions for the processor
#include "osclconfig_ix86.h"

#define __TFS__ <>

#ifdef __GNUC__
#if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4))
#define OSCL_TEMPLATED_DESTRUCTOR_CALL(type,simple_type) ~type ()
#endif
#endif

#define OSCL_BEGIN_PACKED
#define OSCL_PACKED_VAR(x)  x __attribute__((packed))
#define OSCL_PACKED_STRUCT_BEGIN
#define OSCL_PACKED_STRUCT_END __attribute__((packed))
#define OSCL_END_PACKED

#if(OSCL_RELEASE_BUILD)
//no logging
#define PVLOGGER_INST_LEVEL 0
#elif defined(NDEBUG)
//profiling only
#define PVLOGGER_INST_LEVEL 2
#else
//full logging
#define PVLOGGER_INST_LEVEL 5
#endif

//set this to 1 to enable OSCL_ASSERT in release builds.
#define OSCL_ASSERT_ALWAYS 0


// check all osclconfig required macros are defined
#include "osclconfig_check.h"

#endif
