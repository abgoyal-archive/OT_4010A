
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//       O S C L _ P R I Q U E U E   ( P R I O R I T Y   Q U E U E )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_PRIQUEUE_H_INCLUDED
#define OSCL_PRIQUEUE_H_INCLUDED




#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

class OsclPriorityQueueBase
{
    protected:
        virtual ~OsclPriorityQueueBase()
        {}

        OSCL_IMPORT_REF void push_heap(OsclAny* first, OsclAny* last) ;

        OSCL_IMPORT_REF void pop_heap(OsclAny* first, OsclAny* last) ;

        OSCL_IMPORT_REF OsclAny* find_heap(const OsclAny* input, OsclAny* first, OsclAny* last) ;

        OSCL_IMPORT_REF int remove(const OsclAny* input) ;

        void construct(Oscl_Opaque_Type_Compare* ot, Oscl_Vector_Base* vec)
        {
            pOpaqueType = ot;
            pVec = vec;
        }

    private:

        //return delta from "first" to "last" expressed as a number of T elements.
        int delta_T(OsclAny*first, OsclAny*last)
        {
            return ((int)last - (int)first) / pVec->sizeof_T;
        }

        Oscl_Opaque_Type_Compare* pOpaqueType;
        Oscl_Vector_Base* pVec;
};

template < class T> class OsclCompareLess
{
    public:
        int compare(T& a, T& b) const
        {
            return (a < b);
        }
};








template < class Qelem, class Alloc,
class Container = Oscl_Vector<Qelem, Alloc>,
class Compare = OsclCompareLess<Qelem> >

class OsclPriorityQueue : public OsclPriorityQueueBase
        , public Oscl_Opaque_Type_Compare
{

    public:
        typedef typename Container::value_type  value_type;
        typedef Container                       container_type;
        typedef typename Container::iterator    iterator;
        typedef typename Container::const_reference const_reference;

        bool empty() const
        {
            return c.empty();
        };
        uint32 size() const
        {
            return c.size();
        };
        void reserve(uint32 n)
        {
            c.reserve(n);
        };
        const_reference top() const
        {
            return c.front();
        };
        const Container & vec()
        {
            return c;
        }

        void push(const value_type& input)
        {
            c.push_back(input);
            push_heap(c.begin(), c.end());
        }

        //remove top element
        void pop()
        {
            pop_heap(c.begin(), c.end());
            c.pop_back();
        }

        //Remove an arbitrary element, by value.
        //If there are multiple matches, this removes the first one it finds.
        //Returns number of items removed(either 0 or 1).
        int remove(const value_type& input)
        {
            return OsclPriorityQueueBase::remove(&input);
        }

        //Constructor
        OsclPriorityQueue(): OsclPriorityQueueBase(), Oscl_Opaque_Type_Compare()
                , c()
        {
            OsclPriorityQueueBase::construct(this, &c);
        }

        virtual ~OsclPriorityQueue()
        {}

    protected:
        Container c;
        Compare comp;


        void push_heap(iterator first, iterator last)
        {
            OsclPriorityQueueBase::push_heap(first, last);
        }

        void pop_heap(iterator first, iterator last)
        {
            OsclPriorityQueueBase::pop_heap(first, last);
        }

        iterator find_heap(const value_type& input, iterator first, iterator last)
        {
            return OsclPriorityQueueBase::find_heap(&input, first, last);
        }

        //a debug routine for validating the current sort.
        int validate()
        {
            unsigned int ch;
            for (unsigned int par = 0; par < c.size(); par++)
            {
                ch = 2 * par + 1;
                if (ch < c.size() && comp.compare(c[par], c[ch]))
                    return par;//error-- parent<child
                ch++;
                if (ch < c.size() && comp.compare(c[par], c[ch]))
                    return par;//error-- parent<child
            }
            return -1;//ok
        }

        friend class oscl_priqueue_test;

        //from Oscl_Opaque_Type_Compare
        void swap(OsclAny* dest, const OsclAny* src)
        {
            OSCL_ASSERT(dest);
            OSCL_ASSERT(src);
            if (dest != src)
            {
                value_type temp(*((value_type*)dest));
                *((value_type*)dest) = *((value_type*)src);
                *((value_type*)src) = temp;
            }
        }

        //from Oscl_Opaque_Type_Compare
        int compare_LT(OsclAny* a, OsclAny* b) const
        {
            OSCL_ASSERT(a);
            OSCL_ASSERT(b);
            return comp.compare(*((value_type*)a), *((value_type*)b));
        }

        //from Oscl_Opaque_Type_Compare
        int compare_EQ(const OsclAny* a, const OsclAny* b) const
        {
            OSCL_ASSERT(a);
            OSCL_ASSERT(b);
            return (*((value_type*)a)) == (*((value_type*)b));

        }

};

#endif

/*! @} */
