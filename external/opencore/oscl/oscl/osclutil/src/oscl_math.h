
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ M A T H

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MATH_H_INCLUDED
#define OSCL_MATH_H_INCLUDED

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

OSCL_COND_IMPORT_REF double oscl_log(double value);
OSCL_COND_IMPORT_REF double oscl_log10(double value);
OSCL_COND_IMPORT_REF double oscl_sqrt(double value);
OSCL_COND_IMPORT_REF double oscl_pow(double x, double y);
OSCL_COND_IMPORT_REF double oscl_exp(double value);

OSCL_COND_IMPORT_REF double oscl_sin(double value);

OSCL_COND_IMPORT_REF double oscl_cos(double value);

OSCL_COND_IMPORT_REF double oscl_tan(double value);

OSCL_COND_IMPORT_REF double oscl_asin(double value);

OSCL_COND_IMPORT_REF double oscl_atan(double value);

OSCL_COND_IMPORT_REF double oscl_floor(double value);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_math.inl"
#endif

#endif

/*! @} */

