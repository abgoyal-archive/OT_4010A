
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ E X C E P T I O N

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_EXCEPTION_H_INCLUDED
#define OSCL_EXCEPTION_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_ERROR_H_INCLUDED
#include "oscl_error.h"
#endif

#ifndef OSCL_ERROR_IMP_H_INCLUDED
#include "oscl_error_imp.h"
#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//! oscl_exception.h contains all the exception handling macros and classes


//! This template class provides the base exception class that all exceptions derive from
template <int LeaveCode> class OsclException
{
    public:
        OsclException() {}
        static int getLeaveCode()
        {
            return LeaveCode;
        };
};


//! Use this macro to cause a Leave. It terminates the execution of the current active function
#define OSCL_LEAVE(_leave_status) OsclError::Leave(_leave_status)


//! This macro will be used to set up a try block
#define OSCL_TRY(_leave_status,_statements) _PV_TRAP(_leave_status,_statements)
//Same as above, but avoids the TLS lookup.
// param __trampimp is the OsclErrorTrapImp* for the current thread.
#define OSCL_TRY_NO_TLS(__trapimp,_leave_status,_statements) _PV_TRAP_NO_TLS(__trapimp,_leave_status,_statements)

//! This section defines the macros to be used in the catch block following the try block

//! Use this macro to call a function that handles all exception types thrown in the preceding try block
#define OSCL_FIRST_CATCH_ANY(_leave_status, _statements) \
   if (_leave_status!=OsclErrNone) { _statements; }

//! Use this macro to define a block of code that catches the first exception type thrown in the preceding try block
#define OSCL_FIRST_CATCH( _leave_status, _catch_value, _statements) \
    if (_leave_status!=OsclErrNone && _leave_status == _catch_value){_statements;}

//! Use this macro to define a block of code for catching additional exception types
#define OSCL_CATCH( _leave_status, _catch_value, _statements) \
    else if (_leave_status!=OsclErrNone && _leave_status == _catch_value){_statements;}

//! Use this macro to call a function that will catch all remaining exception types
#define OSCL_CATCH_ANY(_leave_status,_statements) \
    else if (_leave_status!=OsclErrNone){ _statements;}

//! Use this macro if OSCL_CATCH_ANY has not been used. It will mark the end of the catch block

#define OSCL_LAST_CATCH(_leave_status) \
    else if (_leave_status!=OsclErrNone){OSCL_LEAVE(_leave_status);}


#endif // INCLUDED_OSCL_EXCEPTION_H



/*! @} */
