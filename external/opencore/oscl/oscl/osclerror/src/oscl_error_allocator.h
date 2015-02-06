
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ E R R O R _ A L L O C A T O R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_ERROR_ALLOCATOR_H_INCLUDED
#define OSCL_ERROR_ALLOCATOR_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_BASE_MACROS_H_INCLUDED
#include "oscl_base_macros.h"
#endif

#ifndef OSCLCONFIG_ERROR_H_INCLUDED
#include "osclconfig_error.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif


//! This class provides static methods to invoke the user defined memory allocation routines
class OsclErrorAllocator
{
    public:
        //! constructor method
        /*!
        \param allocator - a pointer to the concrete object that provides the allocator/deallocator
        */
        OsclErrorAllocator(Oscl_DefAlloc* allocator)
        {
            iAllocator = allocator;
        }
        //! static method to allocate a block of memory on heap
        /*!
        \param aSize - number of bytes to allocate
        */
        static OsclAny* allocate(uint32 aSize)
        {
            OSCL_ASSERT(iAllocator != NULL);
            return iAllocator->ALLOCATE(aSize);
        }

        //! static method to deallocate a block of memory on heap
        /*!
        \param aPointer - pointer to block of memory to be deallocated
        */
        static OsclAny deallocate(OsclAny* aPointer)
        {
            OSCL_ASSERT(iAllocator != NULL);
            iAllocator->deallocate(aPointer);
        }

        //! placement new operator that allocates memory using the user defined methods
        void* operator new(uint32 size, OsclAny* aPtr)
        {
            OSCL_UNUSED_ARG(size);
            return aPtr;
        }

        //! delete operator that doesn't do anything, user has to deallocate manually
        void operator delete(OsclAny* aPtr, OsclAny* aPtr2)
        {
            OSCL_UNUSED_ARG(aPtr);
            OSCL_UNUSED_ARG(aPtr2);
        }

    private:
        static Oscl_DefAlloc* iAllocator;
};



#endif  //OSCL_ERROR_ALLOCATOR_H_INCLUDED

/*! @} */
