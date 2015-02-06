
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//           O S C L _ R E F C O U N T E R _ M E M F R A G

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_REFCOUNTER_MEMFRAG_H_INCLUDED
#define OSCL_REFCOUNTER_MEMFRAG_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_REFCOUNTER_H_INCLUDED
#include "oscl_refcounter.h"
#endif


class OsclRefCounterMemFrag
{
    public:

        /**
         * Constructor.
         * A valid memory fragment and reference counter are
         * required as input.  The memory fragment structure
         * will be copied locally.
         *
         * @param m      reference to memory fragment
         * @param r      pointer to the reference counter associated with the
         *               memory fragment.
         */
        OsclRefCounterMemFrag(OsclMemoryFragment &m, OsclRefCounter *r,
                              uint32 in_capacity) :
                memfrag(m), refcnt(r), capacity(in_capacity)
                // no need to increment refcnt--it should already be done.
        {}

        /**
         * Copy constructor.
         */
        OsclRefCounterMemFrag(const OsclRefCounterMemFrag &x) :
                memfrag(x.memfrag), refcnt(x.refcnt), capacity(x.capacity)
        {
            if (refcnt)
            {
                refcnt->addRef();
            }
        }

        /**
         * Default constructor.
         */
        OsclRefCounterMemFrag()
        {
            memfrag.ptr = 0;
            memfrag.len = 0;
            refcnt = 0;
            capacity = 0;
        }


        /**
         * Assignment Operator
         */
        OsclRefCounterMemFrag& operator= (const OsclRefCounterMemFrag &x)
        {
            if (this == &x)
            {
                // protect against self-assignment
                return *this;
            }

            // remove ref for current memfrag
            if (refcnt)
            {
                refcnt->removeRef();
            }

            // copy assigned object
            memfrag = x.memfrag;
            refcnt = x.refcnt;
            capacity = x.capacity;

            // add ref for new memfrag
            if (refcnt)
            {
                refcnt->addRef();
            }

            return *this;
        }

        /**
         * Destructor.
         * Removes this object's reference from the reference counter.
         * The reference counter will not be deleted.  The reference
         * counter is designed to self-delete when it's reference
         * count reaches 0.
         */
        ~OsclRefCounterMemFrag()
        {
            if (refcnt)
            {
                refcnt->removeRef();
            }
        }

        /**
         * Returns a pointer to the contained reference counter
         * object
         */
        OsclRefCounter* getRefCounter()
        {
            return refcnt;
        }

        /**
         * Returns a reference to the contained memory fragment
         * structure.
         */
        OsclMemoryFragment& getMemFrag()
        {
            return memfrag;
        }

        /**
         * Returns a pointer to the memory fragment data.
         */
        OsclAny* getMemFragPtr()
        {
            return memfrag.ptr;
        }

        /**
         * Returns the size of the memory fragment data which
         * equals its filled size.
         *
         * @return
         */
        uint32 getMemFragSize()
        {
            return memfrag.len;
        }

        /**
         * Returns the capacity of the memory fragment
         *
         * @return
         */
        uint32 getCapacity()
        {
            return capacity;
        }

        /**
         * Returns the reference counter's current count.
         */
        uint32 getCount()
        {
            return (refcnt) ? refcnt->getCount() : 0;
        }



    private:

        OsclMemoryFragment memfrag;
        OsclRefCounter *refcnt;
        uint32 capacity;
};


/*! @} */


#endif // OSCL_REFCOUNTER_MEMFRAG_H
