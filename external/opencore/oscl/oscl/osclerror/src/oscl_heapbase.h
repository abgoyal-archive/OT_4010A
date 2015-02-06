
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                 O S C L _ H E A P B A S E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_HEAPBASE_H_INCLUDED
#define OSCL_HEAPBASE_H_INCLUDED

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif



class _OsclHeapBase
{
    public:
        virtual ~_OsclHeapBase() {}

    protected:
        _OsclHeapBase() {}
        _OsclHeapBase(const _OsclHeapBase&) {}
    private:
        _OsclHeapBase& operator=(const _OsclHeapBase&);
        friend class PVCleanupStack;
};


typedef void (*OsclTrapOperation)(OsclAny*);

class OsclTrapItem
{
    public:
        OSCL_INLINE OsclTrapItem(OsclTrapOperation anOperation);
        OSCL_INLINE OsclTrapItem(OsclTrapOperation anOperation, OsclAny* aPtr);
    private:
        OsclTrapOperation iOperation;
        OsclAny* iPtr;
        friend class OsclTrapStackItem;
        friend class OsclTrapStack;
};



#if !(OSCL_DISABLE_INLINES)
#include "oscl_heapbase.inl"
#endif

#endif //


/*! @} */
