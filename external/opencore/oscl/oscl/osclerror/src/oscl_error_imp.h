
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ E R R O R _ I M P

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_ERROR_IMP_H_INCLUDED
#define OSCL_ERROR_IMP_H_INCLUDED

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif


//This selects the type of implementation
//based on OSCL capabilities.
#define PVERROR_IMP_JUMPS



#if defined( PVERROR_IMP_JUMPS)
#include "oscl_error_imp_jumps.h"
#elif defined (PVERROR_IMP_CPP_EXCEPTIONS)
#include "oscl_error_imp_cppexceptions.h"
#elif defined(PVERROR_IMP_FATAL_ERROR)
#include "oscl_error_imp_fatalerror.h"
#else
#error No leave implementation!
#endif




#endif

/*! @} */

