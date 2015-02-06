
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//     O S C L C O N F I G   ( P L A T F O R M   C O N F I G   I N F O )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCLCONFIG_H_INCLUDED
#define OSCLCONFIG_H_INCLUDED

// system includes for dynamic registry
#include <dirent.h>
#include <dlfcn.h>

#define OSCL_HAS_ANDROID_SUPPORT 1
#define OSCL_HAS_ANDROID_FILE_IO_SUPPORT 1

#define OSCL_EXPORT_REF __attribute__ ((visibility("default")))
#define OSCL_IMPORT_REF __attribute__ ((visibility("default")))

// include common include for determining sizes from limits.h
#include "osclconfig_limits_typedefs.h"

//This switch turns off some profiling and debug settings
#ifdef NDEBUG
#define OSCL_RELEASE_BUILD 1
#else
#define OSCL_RELEASE_BUILD 0
#endif

// include common unix definitions
#include "osclconfig_unix_android.h"

// define the suffix for unsigned constants
#define OSCL_UNSIGNED_CONST(x) x##u

// override the common definition for
#undef OSCL_NATIVE_UINT64_TYPE
#define OSCL_NATIVE_UINT64_TYPE    u_int64_t

// include the definitions for the processor
#include "osclconfig_ix86.h"

// the syntax for explicitly calling the destructor varies on some platforms
// below is the default syntax as defined by another ARM project
#define OSCL_TEMPLATED_DESTRUCTOR_CALL(type,simple_type) ~type ()


#define __TFS__ <>

#define OSCL_BEGIN_PACKED
#define OSCL_PACKED_VAR(x)  x __attribute__((packed))
#define OSCL_PACKED_STRUCT_BEGIN
#define OSCL_PACKED_STRUCT_END  __attribute__((packed))
#define OSCL_END_PACKED

#define PVLOGGER_INST_LEVEL 0
#define PVLOGGER_ANDROIDLOG_LEVEL 0

//set this to 1 to enable OSCL_ASSERT in release builds.
#define OSCL_ASSERT_ALWAYS 0


// check all osclconfig required macros are defined
#include "osclconfig_check.h"

#endif
