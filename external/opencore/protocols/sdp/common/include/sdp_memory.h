

#ifndef SDP_MEMORY_H
#define SDP_MEMORY_H

#include "oscl_mem.h"

#include "oscl_base_alloc.h"

typedef OsclMemAllocator SDPParserAlloc;

class SDPMemory
{
    public:
        SDPMemory() {};
        ~SDPMemory() {};
        template <class T> T* alloc(uint32 size, T* temp)
        {
            T* ptr = (T*)oscl_malloc(size);
            return ptr;
        };
        template <class T> void dealloc(T* p)
        {
            oscl_free(p);
        };
};

template <class T> class SDPAllocDestructDealloc: public OsclDestructDealloc
{
    public:
        virtual ~SDPAllocDestructDealloc() {}

        T* allocate(uint32 size)
        {
            SDPParserAlloc alloc;
            T* ptr = OSCL_PLACEMENT_NEW(alloc.ALLOCATE(size), T());
            return ptr;
        }

        virtual void destruct_and_dealloc(OsclAny* ptr)
        {
            T* tptr ;
            tptr = reinterpret_cast<T*>(ptr);
            tptr->~T();
            SDPParserAlloc alloc;
            alloc.deallocate(ptr);
        }
};

#endif


