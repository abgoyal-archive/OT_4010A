
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ E R R N O

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




//! oscl_errno.h contains functions to access the global errno

#ifndef OSCL_ERRNO_H_INCLUDED
#define OSCL_ERRNO_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//! This function determines if a particular system saves the error number that occurs on a system call
OSCL_IMPORT_REF bool OSCL_IsErrnoSupported();

//! This function returns the value of the system's global error number variable
OSCL_IMPORT_REF int OSCL_GetLastError();

//! This function sets the last error code for the system
OSCL_IMPORT_REF bool OSCL_SetLastError(int newVal);

//! This function maps an error number to an error-message string.
OSCL_IMPORT_REF char *OSCL_StrError(int errnum);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_errno.inl"
#endif


#endif // INCLUDED_OSCL_ERRNO_H

/*! @} */
