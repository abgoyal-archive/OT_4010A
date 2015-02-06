
#ifndef UNIT_TEST_COMMON_H
#define UNIT_TEST_COMMON_H

//Note: the Oscl includes must be minimal here, since we use
//this framework to test Oscl.
#include "osclconfig.h"
#include "oscl_types.h"//to get basic types
#include "osclconfig_memory.h" //to get placement "new"

class unit_test_allocator
{
    public:
        static OsclAny* allocate(const uint32 size)
        {
            return malloc(size);
        }

        static  void deallocate(OsclAny *p)
        {
            free(p);
        }
};


class UnitTest_HeapBase
{
    public:

        static void* operator new(size_t aSize)
        {
            return unit_test_allocator::allocate(aSize);
        }

        static void* operator new[](size_t aSize)
        {
            return unit_test_allocator::allocate(aSize);
        }

        static void* operator new(size_t , void* aPtr)
        {
            return aPtr;
        }

        static void operator delete(void* aPtr)
        {
            unit_test_allocator::deallocate(aPtr);
        }

        static void operator delete[](void* aPtr)
        {
            unit_test_allocator::deallocate(aPtr);
        }

        UnitTest_HeapBase() {}
        virtual ~UnitTest_HeapBase() {}


};

#include "unit_test_vector.h"
#include "unit_test_local_string.h"

#define _STRING UnitTest_String
#define _APPEND(string,value) string+=(value)
#define _VECTOR(var,allocator) UnitTest_Vector<var,allocator>


#endif //UNIT_TEST_COMMON_H

