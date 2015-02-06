
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ F I L E  _ HANDLE

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_HANDLE_H_INCLUDED
#define OSCL_FILE_HANDLE_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


typedef FILE* TOsclFileHandle;

class OsclFileHandle
{
    public:
        OsclFileHandle(TOsclFileHandle aHandle)
                : iHandle(aHandle)
        {}
        OsclFileHandle(const OsclFileHandle& aHandle)
                : iHandle(aHandle.iHandle)
        {}
        TOsclFileHandle Handle()const
        {
            return iHandle;
        }
    private:
        TOsclFileHandle iHandle;
        friend class Oscl_File;
};


#endif // OSCL_FILE_HANDLE_H_INCLUDED

/*! @} */

