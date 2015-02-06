
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                 O S C L _ A S S E R T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_ASSERT_H_INCLUDED
#define OSCL_ASSERT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


//! This function terminates the current process abnormally
OSCL_COND_IMPORT_REF void _OSCL_Abort();

//! OSCL_ASSERT macro evaluates an expression and when the result is false, prints a diagnostic message and aborts the program.
#if (!defined(NDEBUG) || (OSCL_ASSERT_ALWAYS))
OSCL_IMPORT_REF void OSCL_Assert(const char *expr, const char *filename, int line_number);

#define OSCL_ASSERT(_expr) \
    ((_expr)?((void)0):OSCL_Assert(# _expr,__FILE__,__LINE__))

#else
#define OSCL_ASSERT(_expr) ((void)0)
#endif

#if (!OSCL_DISABLE_INLINES)
#include "oscl_assert.inl"
#endif

/*! @} */

#endif // OSCL_ASSERT_H_INCLUDED
