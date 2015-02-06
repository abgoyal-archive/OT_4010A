
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ M E M P O O L _ A L L O C A T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#include "oscl_mempool_allocator.h"

#include "oscl_exception.h"

#include "oscl_mem_basic_functions.h"

OsclMemPoolAllocator::OsclMemPoolAllocator(Oscl_DefAlloc* gen_alloc)
        : iCustomAllocator(gen_alloc),
        iBaseAddress(0)
{
}

OsclMemPoolAllocator::~OsclMemPoolAllocator()
{
    if (iBaseAddress)
    {
        DestroyMemPool();
    }
}

OsclAny* OsclMemPoolAllocator::CreateMemPool(const uint32 aNumChunk, const uint32 aChunkSize)
{
    if (aNumChunk == 0 || aChunkSize == 0)
    {
        OSCL_LEAVE(OsclErrArgument);

        // OSCL_UNUSED_RETURN(NULL);    This statement was removed to avoid compiler warning for Unreachable Code
    }

    // Heap memory alligned chunk size
    uint32 lChunkSizeMemAligned;

    lChunkSizeMemAligned = oscl_mem_aligned_size(aChunkSize);

    if (iCustomAllocator)
    {
        iBaseAddress = iCustomAllocator->ALLOCATE(aNumChunk * lChunkSizeMemAligned);
    }
    else
    {
        iBaseAddress = _oscl_malloc(aNumChunk * lChunkSizeMemAligned);
    }

    if (iBaseAddress == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);

        // OSCL_UNUSED_RETURN(NULL);    This statement was removed to avoid compiler warning for Unreachable Code

    }

#if OSCL_MEM_FILL_WITH_PATTERN

    oscl_memset(iMemPool, 0x55, aNumChunk*lChunkSizeMemAligned);

#endif

    return iBaseAddress;

}

uint OsclMemPoolAllocator::oscl_mem_aligned_size(uint x)
{

    uint y;

    if (x & 0x7)
    {
        y = x & (~0x7);
        y += 8;
    }
    else
    {
        y = x;
    }

    return y;
}

void OsclMemPoolAllocator::DestroyMemPool()
{
    // If client class calls DestroyMemPool without CreateMemPool i.e iBaseAddress is NULL then leave
    if (!iBaseAddress)
    {
        OSCL_LEAVE(OsclErrArgument);

    }

    if (iCustomAllocator)
    {
        iCustomAllocator->deallocate(iBaseAddress);
    }
    else
    {
        _oscl_free(iBaseAddress);
    }

    iCustomAllocator = 0;

    iBaseAddress = 0;

    return;
}



