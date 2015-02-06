

// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                  P V M F _ P M E M _ A L L O C

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef PVMF_PMEM_ALLOC_H_INCLUDED
#define PVMF_PMEM_ALLOC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Ryan Lin !!!
///////////////////////////////////////////////////////////////////////////////////////////////////////
// for allocate physical continuous mem

class OSCL_IMPORT_REF PVMFPmemMemAlloc : public Oscl_DefAlloc
{
    public:
        virtual void* allocate(const uint32 size);
        virtual void deallocate(OsclAny* p);
};

#endif


/*! @} */
