

#ifndef PVMF_POOL_BUFFER_ALLOCATOR_H_INCLUDED
#define PVMF_POOL_BUFFER_ALLOCATOR_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_REFCOUNTER_MEMFRAG_H_INCLUDED
#include "oscl_refcounter_memfrag.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

class PVLogger;

//typedef OSCLMemAutoPtr<OsclRefCounterMemFrag*> OsclRefCounterMemFrag_ptr;
typedef Oscl_TAlloc<OsclRefCounterMemFrag, OsclMemAllocator> OsclRefCounterMemFrag_Alloc;

class PVMFBufferPoolAllocator : public OsclDestructDealloc
{
    public:
        OSCL_IMPORT_REF PVMFBufferPoolAllocator(bool aLeaveOnAllocFailure = true);
        OSCL_IMPORT_REF virtual ~PVMFBufferPoolAllocator();
        OSCL_IMPORT_REF void SetLeaveOnAllocFailure(bool aLeaveOnAllocFailure);
        OSCL_IMPORT_REF virtual void size(uint16 num_frags, uint16 frag_size);
        OSCL_IMPORT_REF virtual void clear();
        OSCL_IMPORT_REF virtual OsclRefCounterMemFrag get();
        OSCL_IMPORT_REF virtual void release(OsclRefCounterMemFrag&);
        // OsclDestructDealloc virtual
        OSCL_IMPORT_REF virtual void destruct_and_dealloc(OsclAny* ptr);
        OSCL_IMPORT_REF void SetFailFrequency(uint16 aFrequency);
    protected:
        uint32 iFragSize;
        bool iDestroyPool;
        Oscl_Vector<OsclRefCounterMemFrag, OsclRefCounterMemFrag_Alloc> iAvailFragments;
        PVLogger *iLogger;
        uint32 iAllocNum;
        uint32 iFailFrequency;
        bool iLeaveOnAllocFailure;
};


#endif
