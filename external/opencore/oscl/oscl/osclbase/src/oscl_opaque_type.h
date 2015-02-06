
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ O P A Q U E _ T Y P E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_OPAQUE_TYPE_H_INCLUDED
#define OSCL_OPAQUE_TYPE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif



class Oscl_Opaque_Type_Alloc
{
    public:
        virtual ~Oscl_Opaque_Type_Alloc() {}

        /**
         * Construct element at p using element at init_val as the initial value.
         * Both pointers must be non-NULL.
         */
        virtual void construct(OsclAny* p, const OsclAny* init_val) = 0;

        /**
         * Destroy element at p.
         */
        virtual void destroy(OsclAny* p) = 0;

        /**
         * Allocate "size" bytes
         */
        virtual OsclAny* allocate(const uint32 size) = 0;

        /**
         * Deallocate memory previously allocated with "allocate"
         */
        virtual void deallocate(OsclAny* p) = 0;
};

class Oscl_Opaque_Type_Compare
{
    public:
        virtual ~Oscl_Opaque_Type_Compare() {}

        /**
         * Swap element at "a" with element at "b".
         * Both pointers must be non-NULL.
         */
        virtual void swap(OsclAny* a, const OsclAny* b) = 0;

        /**
         * Return a<b.
         */
        virtual int compare_LT(OsclAny* a, OsclAny* b) const = 0;

        /**
         * Return a==b.
         */
        virtual int compare_EQ(const OsclAny* a, const OsclAny* b) const = 0;

};

class Oscl_Opaque_Type_Alloc_LL
{
    public:
        virtual ~Oscl_Opaque_Type_Alloc_LL() {}

        /**
         * Construct element at p using element at init_val as the initial value.
         * Both pointers must be non-NULL.
         */
        virtual void construct(OsclAny* p, const OsclAny* init_val) = 0;

        /**
         * Destroy element at p.
         */
        virtual void destroy(OsclAny* p) = 0;

        /**
         * Allocate "size" bytes
         */
        virtual OsclAny* allocate(const uint32 size) = 0;

        /**
         * Deallocate memory previously allocated with "allocate"
         */
        virtual void deallocate(OsclAny* p) = 0;

        /**
         * Get next element in linked list.
         */
        virtual OsclAny* get_next(const OsclAny* elem)const = 0;

        /**
         * Set next element in linked list.
         */
        virtual void set_next(OsclAny* elem, const OsclAny* nextelem) = 0;

        /**
         * Get data
         */
        virtual void get_data(OsclAny*elem, OsclAny*data_val) = 0;

        /**
         * Compare data.
         */
        virtual bool compare_data(const OsclAny*elem, const OsclAny*data_val)const = 0;
};

/*! @} */


#endif





