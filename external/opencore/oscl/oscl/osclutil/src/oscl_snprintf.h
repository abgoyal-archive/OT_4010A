
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L_ S N P R I N T F

//    This is a portable implementation of snprintf() for use on all
//    platforms supported by oscl.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_SNPRINTF_H_INCLUDED
#define OSCL_SNPRINTF_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

OSCL_IMPORT_REF int32 oscl_snprintf(char *str, uint32 count, const char *fmt, /*args*/ ...);
OSCL_IMPORT_REF int32 oscl_snprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, /*args*/ ...);
OSCL_IMPORT_REF int32 oscl_vsnprintf(char *str, uint32 count, const char *fmt, va_list args);
OSCL_IMPORT_REF int32 oscl_vsnprintf(oscl_wchar *str, uint32 count, const oscl_wchar *fmt, va_list args);
#endif

/*! @} */
