
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//       O S C L C O N F I G _ C O M P I L E R  _ W A R N I N G S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



// This macro enables the "#pragma GCC system_header" found in any header file that
// includes this config file.
// "#pragma GCC system_header" suppresses compiler warnings in the rest of that header
// file by treating the header as a system header file.
// For instance, foo.h has 30 lines, "#pragma GCC system_header" is inserted at line 10,
// from line 11 to the end of file, all compiler warnings are disabled.
// However, this does not affect any files that include foo.h.
//
#ifdef __GNUC__
#define OSCL_DISABLE_GCC_WARNING_SYSTEM_HEADER
#endif

#define OSCL_FUNCTION_PTR(x) (&x)

