
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ L O C K _ B A S E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_LOCK_BASE_H_INCLUDED
#define OSCL_LOCK_BASE_H_INCLUDED


class OsclLockBase
{

    public:
        virtual void Lock() = 0;
        virtual void Unlock() = 0;
        virtual ~OsclLockBase() {}

};

class OsclNullLock: public OsclLockBase
{

    public:
        virtual void Lock() {};
        virtual void Unlock() {};
        virtual ~OsclNullLock() {}

};




template<class LockClass> class OsclScopedLock
{
    private:
        LockClass* _Ptr;

        // make copy constructor private so no default is created.
        /**
        * @brief Copy constructor
        *
        * Initializes the pointer and takes ownership from another oscl_auto_ptr.
        * Note that the other class does NOT own the pointer any longer, and
        * hence it is NOT its responsibility to free it.
        */
        OsclScopedLock(const OsclScopedLock<LockClass>&) {}


        /**
        * @brief release() method releases ownership of the pointer, currently owned
        * by the class. It returns the pointer as well.
        *
        */
        void release()
        {
            if (_Ptr)
            {
                _Ptr->Unlock();
                _Ptr = NULL;
            }
        }


        /**
        * @brief acquire() method acquires ownership of the lock.
        *
        */
        void acquire()
        {
            if (_Ptr)
            {
                _Ptr->Lock();
            }
        }


    public:

        /**
        * @brief Default constructor
        * Initializes the pointer and takes ownership.
        */
        explicit OsclScopedLock(LockClass& inLock) : _Ptr(&inLock)
        {
            acquire();
        };

        /**
        * @brief Destructor
        *
        * The pointer is deleted in case this class still has ownership
        */
        ~OsclScopedLock()
        {
            release();
        }



};


/*! @} */


#endif
