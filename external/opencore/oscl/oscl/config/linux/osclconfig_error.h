
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                O S C L C O N F I G _ E R R O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#define OSCLCONFIG_ERROR_H_INCLUDED

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

#define OSCL_HAS_EXCEPTIONS                     1
#define OSCL_HAS_ERRNO_H                        1
#define OSCL_HAS_SYMBIAN_ERRORTRAP      0
#define OSCL_HAS_SETJMP_H 1

// system header files
#include <setjmp.h>
#include <errno.h>


// confirm that all definitions have been defined
#include "osclconfig_error_check.h"

#endif // OSCLCONFIG_ERROR_H_INCLUDED

