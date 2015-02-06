
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//         O S C L _ E R R O R _ I M P _ C P P E X C E P T I O N S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_ERROR_IMP_CPPEXCEPTIONS_H_INCLUDED
#define OSCL_ERROR_IMP_CPPEXCEPTIONS_H_INCLUDED

#ifndef OSCL_ERROR_TRAPCLEANUP_H_INCLUDED
#include "oscl_error_trapcleanup.h"
#endif

//Implementation file for Leave using C++ exceptions.

//This is a full implementation of Leave.

class internalLeave
{
    public:
        int a;
};

//Leave throws C++ exceptions.
#define PVError_DoLeave() internalLeave __ilv;__ilv.a=0;throw(__ilv)


//_PV_TRAP catches Leaves.
//_r is the leave code, _s are statements to execute
#define _PV_TRAP(__r,__s)\
    __r=OsclErrNone;\
    {\
        OsclErrorTrapImp* __tr=OsclErrorTrapImp::Trap();\
        if(!__tr){__s;}else{\
        try{__s;}\
        catch(internalLeave __lv)\
        {__lv.a=__r=__tr->iLeave;}\
        __tr->UnTrap();}\
    }

//_PV_TRAP_NO_TLS catches Leaves.
//_r is the leave code, _s are statements to execute
#define _PV_TRAP_NO_TLS(__trapimp,__r,__s)\
    __r=OsclErrNone;\
    {\
        OsclErrorTrapImp* __tr=OsclErrorTrapImp::TrapNoTls(__trapimp);\
        if(!__tr){__s;}else{\
        try{__s;}\
        catch(internalLeave __lv)\
        {__lv.a=__r=__tr->iLeave;}\
        __tr->UnTrap();}\
    }


#endif // OSCL_ERROR_IMP_CPPEXCEPTIONS_H_INCLUDED

/*! @} */


