

#ifndef PVMF_MEDIA_DATA_IMPL_H_INCLUDED
#define PVMF_MEDIA_DATA_IMPL_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_REFCOUNTER_MEMFRAG_H_INCLUDED
#include "oscl_refcounter_memfrag.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class PVMFMediaDataImpl: public HeapBase
{


    public:
        virtual ~PVMFMediaDataImpl() {}
        OSCL_IMPORT_REF virtual uint32 getMarkerInfo() = 0;
        OSCL_IMPORT_REF virtual bool isRandomAccessPoint() = 0;
        OSCL_IMPORT_REF virtual uint32 getErrorsFlag() = 0;
        OSCL_IMPORT_REF virtual uint32 getNumFragments() = 0;
        OSCL_IMPORT_REF virtual bool getMediaFragment(uint32 index, OsclRefCounterMemFrag& memfrag) = 0;
        OSCL_IMPORT_REF virtual bool getMediaFragmentSize(uint32 index, uint32& size) = 0;
        OSCL_IMPORT_REF virtual uint32 getFilledSize() = 0;
        OSCL_IMPORT_REF virtual uint32 getCapacity() = 0;

        OSCL_IMPORT_REF virtual void setCapacity(uint32 aCapacity) = 0;
        OSCL_IMPORT_REF virtual bool setMediaFragFilledLen(uint32 index, uint32 len) = 0;
        OSCL_IMPORT_REF virtual bool setMarkerInfo(uint32 marker) = 0;
        OSCL_IMPORT_REF virtual bool setRandomAccessPoint(bool flag) = 0;
        OSCL_IMPORT_REF virtual bool setErrorsFlag(uint32 flag) = 0;

        OSCL_IMPORT_REF virtual bool appendMediaFragment(OsclRefCounterMemFrag& memfrag) = 0;
        OSCL_IMPORT_REF virtual bool clearMediaFragments() = 0;

};



#endif
