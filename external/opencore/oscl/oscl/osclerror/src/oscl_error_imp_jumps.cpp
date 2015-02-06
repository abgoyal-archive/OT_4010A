

//Implementation file for OsclJump class,
//for use with the setjmp/longjmp error handler.

#include "oscl_error_imp.h"

#ifdef PVERROR_IMP_JUMPS

#include "oscl_assert.h"


////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void OsclJump::StaticJump(int jmpcode)
//jump to latest jumpmark
{
    int32 error;
    OsclErrorTrapImp *trap = OsclErrorTrapImp::GetErrorTrap(error);
    if (!trap)
    {
        //Note: you can't leave here, since leave would
        //invoke this routine again.  It is not safe to return
        //either, because calling code is expecting an execution
        //end.
        OSCL_ASSERT(false);
        _OSCL_Abort();
    }
    else
    {
        trap->iJumpData->Jump(jmpcode);
    }
}


#endif




