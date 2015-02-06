
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//           O S C L _ E R R O R _ I M P _ F A T A L E R R O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_ERROR_IMP_FATALERROR_H_INCLUDED
#define OSCL_ERROR_IMP_FATALERROR_H_INCLUDED

// Implementation File for Leave using system fatal error.

//Fatal error implementation for compilers without C++ exceptions.
//This implementation is very limited.  Leave conditions
//just cause fatal program errors.  There is no way to catch or trap
//any Leave.
#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

//Leave just calls a system fatal error.
#define PVError_DoLeave() _OSCL_Abort()

//_PV_TRAP
//_r is leave code, _s is statements.
//this macro isn't really functional since any
//leave will abort the program, but it's needed
//to compile.
#define _PV_TRAP(__r,__s) \
    __r=OsclErrNone;\
    {__s;}

//_PV_TRAP_NO_TLS
//_r is leave code, _s is statements.
//this macro isn't really functional since any
//leave will abort the program, but it's needed
//to compile.
#define _PV_TRAP_NO_TLS(__tr,__r,__s) \
    __r=OsclErrNone;\
    {__s;}



#endif // OSCL_ERROR_IMP_FATALERROR_H_INCLUDED

/*! @} */
