
#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#define PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED

#ifndef PVMF_MEDIA_DATA_IMPL_H_INCLUDED
#include "pvmf_media_data_impl.h"
#endif

#ifndef OSCL_SHARED_PTR_H_INCLUDED
#include "oscl_shared_ptr.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class PVMFSimpleMediaBuffer : public PVMFMediaDataImpl
{

    public:
        OSCL_IMPORT_REF virtual uint32 getMarkerInfo();
        OSCL_IMPORT_REF virtual bool isRandomAccessPoint();
        OSCL_IMPORT_REF virtual uint32 getErrorsFlag();
        OSCL_IMPORT_REF virtual uint32 getNumFragments();
        OSCL_IMPORT_REF virtual bool getMediaFragment(uint32 index, OsclRefCounterMemFrag& memfrag);
        OSCL_IMPORT_REF virtual bool getMediaFragmentSize(uint32 index, uint32& size);
        OSCL_IMPORT_REF virtual uint32 getFilledSize();
        OSCL_IMPORT_REF virtual uint32 getCapacity();

        OSCL_IMPORT_REF virtual void setCapacity(uint32 aCapacity);
        OSCL_IMPORT_REF virtual bool setMediaFragFilledLen(uint32 index, uint32 len);
        OSCL_IMPORT_REF virtual bool setMarkerInfo(uint32 marker);
        OSCL_IMPORT_REF virtual bool setRandomAccessPoint(bool flag);
        OSCL_IMPORT_REF virtual bool setErrorsFlag(uint32 flag);
        OSCL_IMPORT_REF virtual bool appendMediaFragment(OsclRefCounterMemFrag& memfrag);
        OSCL_IMPORT_REF virtual bool clearMediaFragments();

        /* NOTE!!:  The constructor assumes the refcnt has already been incremented
         * to reflect this class holding a reference to the buffer. Increment it
         * externally if you aren't simply passing ownership of a reference
         */
        OSCL_IMPORT_REF PVMFSimpleMediaBuffer(void *ptr,
                                              uint32 capacity,
                                              OsclRefCounter *my_refcnt);

        OSCL_IMPORT_REF virtual ~PVMFSimpleMediaBuffer();

    private:
        uint32 marker_info;
        bool random_access_point;
        uint32 errors_flag;
        OsclMemoryFragment buffer;
        uint32 capacity;
        OsclRefCounter* refcnt;

};



class PVMFSimpleMediaBufferCombinedAlloc : public HeapBase
{

    public:
        PVMFSimpleMediaBufferCombinedAlloc(Oscl_DefAlloc* opt_gen_alloc):
                gen_alloc(opt_gen_alloc)
        {
            if (opt_gen_alloc == NULL)
            {
                OSCL_ASSERT(false);
            }
        };
        OSCL_IMPORT_REF OsclSharedPtr<PVMFMediaDataImpl> allocate(uint32 requested_buffer_capacity = 0);

    private:
        Oscl_DefAlloc* gen_alloc;

};


#endif

