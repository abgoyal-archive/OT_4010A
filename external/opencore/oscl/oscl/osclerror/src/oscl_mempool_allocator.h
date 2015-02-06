
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ M E M P O O L _ A L L O C A T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MEMPOOL_ALLOCATOR_H_INCLUDED
#define OSCL_MEMPOOL_ALLOCATOR_H_INCLUDED

#ifndef OSCL_DEFALLOC_H_INCLUDED
// For custom allocator Oscl_DefAlloc object
#include "oscl_defalloc.h"
#endif

class OsclMemPoolAllocator
{
    public:
        // Constructor
        OsclMemPoolAllocator(Oscl_DefAlloc* gen_alloc = NULL);

        // Virtual destructor
        virtual ~OsclMemPoolAllocator();

        // Create memory pool
        OsclAny* CreateMemPool(const uint32 aNumChunk = 2, const uint32 aChunkSize = 4);

        // Delete memory pool
        void DestroyMemPool();

        // Memory alignment
        uint oscl_mem_aligned_size(uint size);

    private:
        // Custom allocator for memory
        Oscl_DefAlloc* iCustomAllocator;

        // Base address for memory pool
        OsclAny* iBaseAddress;

};

#endif
