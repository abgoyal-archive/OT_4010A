
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                           O S C L _ B A S E _ A L L O C

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_BASE_ALLOC_H_INCLUDED
#define OSCL_BASE_ALLOC_H_INCLUDED

#include "osclconfig.h"
#include "oscl_defalloc.h"
#include "osclconfig_memory.h"

class _OsclBasicAllocator: public Oscl_DefAlloc
{
    public:
        OsclAny* allocate(const uint32 size)
        {
            return malloc(size);
        }

        void deallocate(OsclAny *p)
        {
            free(p);
        }

        virtual ~_OsclBasicAllocator() {}
};

/*! @} */

#endif  // OSCL_BASE_H_INCLUDED
