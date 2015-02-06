
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ E X C L U S I V E _ P T R

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_EXCLUSIVE_PTR_H_INCLUDED
#define OSCL_EXCLUSIVE_PTR_H_INCLUDED

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


template<class T> class OsclExclusivePtr
{
    protected:
        T* _Ptr;

    public:

        /**
        * @brief Default constructor
        * Initializes the pointer and takes ownership.
        */
        explicit OsclExclusivePtr(T* inPtr = 0) : _Ptr(inPtr) {};

        /**
        * @brief Copy constructor
        *
        * Initializes the pointer and takes ownership from another OsclExclusivePtr.
        * Note that the other class does NOT own the pointer any longer, and
        * hence it is NOT its responsibility to free it.
        */
        OsclExclusivePtr(OsclExclusivePtr<T>& _Y): _Ptr(_Y.release()) {};


        /**
        * @brief Assignment operator from an another OsclExclusivePtr
        *
        * @param _Y The value parameter should be another OsclExclusivePtr
        * @returns Returns a reference to this OsclExclusivePtr instance with
        *   pointer initialized.
        * @pre The input class should be non-null and should point to
        *   a valid pointer.
        *
        * This assignment operator initializes the class to the contents
        * of the OsclExclusivePtr given as the input parameter. The ownership
        * of the pointer is transferred.
        */
        OsclExclusivePtr<T>& operator=(OsclExclusivePtr<T>& _Y)
        {
            if (this != &_Y)
            {
                if (_Ptr != _Y.get())
                {
                    delete _Ptr;
                }
                _Ptr = _Y.release();
            }
            return (*this);
        }

        /**
        * @brief Destructor
        *
        * The pointer is deleted in case this class still has ownership
        */
        virtual ~OsclExclusivePtr()
        {
            if (_Ptr)
                delete _Ptr;
        }

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusivePtr can be used like the
        * regular pointer that it was initialized with.
        */
        T& operator*() const
        {
            return (*get());
        }

        /**
        * @brief The indirection operator (->) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusivePtr can be used like the
        * regular pointer that it was initialized with.
        */
        T *operator->() const
        {
            return (get());
        }

        /**
        * @brief get() method returns the pointer, currently owned by the class.
        *
        */
        T *get() const
        {
            return (_Ptr);
        }

        /**
        * @brief release() method releases ownership of the pointer, currently owned
        * by the class. It returns the pointer as well.
        *
        */
        T *release()
        {
            T *tmp = _Ptr;
            _Ptr = NULL;
            return (tmp);
        }

        /**
        * @brief set() method sets ownership to the pointer, passed.
        * This method is needed when the class is created with a default
        * constructor. Returns false in case the class is non-empty.
        *
        */
        bool set(T* ptr)
        {
            if ((_Ptr == NULL))
            {
                _Ptr = ptr;
                return true;
            }
            return false;
        }

};

template<class T> class OsclExclusiveArrayPtr
{
    protected:
        T* _Ptr;

    public:

        /**
        * @brief Default constructor
        * Initializes the pointer and takes ownership.
        */
        explicit OsclExclusiveArrayPtr(T* inPtr = 0) : _Ptr(inPtr) {};

        /**
        * @brief Copy constructor
        *
        * Initializes the pointer and takes ownership from another OsclExclusiveArrayPtr.
        * Note that the other class does NOT own the pointer any longer, and
        * hence it is NOT its responsibility to free it.
        */
        OsclExclusiveArrayPtr(OsclExclusiveArrayPtr<T>& _Y): _Ptr(_Y.release()) {};


        /**
        * @brief Assignment operator from an another OsclExclusiveArrayPtr
        *
        * @param _Y The value parameter should be another OsclExclusiveArrayPtr
        * @returns Returns a reference to this OsclExclusiveArrayPtr instance with
        *   pointer initialized.
        * @pre The input class should be non-null and should point to
        *   a valid pointer.
        *
        * This assignment operator initializes the class to the contents
        * of the OsclExclusiveArrayPtr given as the input parameter. The ownership
        * of the pointer is transferred.
        */
        OsclExclusiveArrayPtr<T>& operator=(OsclExclusiveArrayPtr<T>& _Y)
        {
            if (this != &_Y)
            {
                if (_Ptr != _Y.get())
                {
                    delete [] _Ptr;
                }
                _Ptr = _Y.release();
            }
            return (*this);
        }

        /**
        * @brief Destructor
        *
        * The pointer is deleted in case this class still has ownership
        */
        virtual ~OsclExclusiveArrayPtr()
        {
            if (_Ptr)
                delete [] _Ptr;
        }

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusiveArrayPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T& operator*() const
        {
            return (*get());
        }

        /**
        * @brief The indirection operator (->) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusiveArrayPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T *operator->() const
        {
            return (get());
        }

        /**
        * @brief get() method returns the pointer, currently owned by the class.
        *
        */
        T *get() const
        {
            return (_Ptr);
        }

        /**
        * @brief release() method releases ownership of the pointer, currently owned
        * by the class. It returns the pointer as well.
        *
        */
        T *release()
        {
            T *tmp = _Ptr;
            _Ptr = NULL;
            return (tmp);
        }

        /**
        * @brief set() method sets ownership to the pointer, passed.
        * This method is needed when the class is created with a default
        * constructor. Returns false in case the class is non-empty.
        *
        */
        bool set(T* ptr)
        {
            if ((_Ptr == NULL))
            {
                _Ptr = ptr;
                return true;
            }
            return false;
        }

};


template<class T, class Alloc> class OsclExclusivePtrA
{
    protected:
        T* _Ptr;

    public:

        /**
        * @brief Default constructor
        * Initializes the pointer and takes ownership.
        */
        explicit OsclExclusivePtrA(T* inPtr = 0) : _Ptr(inPtr) {};

        /**
        * @brief Copy constructor
        *
        * Initializes the pointer and takes ownership from another OsclExclusiveArrayPtr.
        * Note that the other class does NOT own the pointer any longer, and
        * hence it is NOT its responsibility to free it.
        */
        OsclExclusivePtrA(OsclExclusivePtrA<T, Alloc>& _Y): _Ptr(_Y.release()) {};


        /**
        * @brief Assignment operator from an another OsclExclusiveArrayPtr
        *
        * @param _Y The value parameter should be another OsclExclusiveArrayPtr
        * @returns Returns a reference to this OsclExclusiveArrayPtr instance with
        *   pointer initialized.
        * @pre The input class should be non-null and should point to
        *   a valid pointer.
        *
        * This assignment operator initializes the class to the contents
        * of the OsclExclusiveArrayPtr given as the input parameter. The ownership
        * of the pointer is transferred.
        */
        OsclExclusivePtrA<T, Alloc>& operator=(OsclExclusivePtrA<T, Alloc>& _Y)
        {
            if (this != &_Y)
            {
                if (_Ptr != _Y.get())
                {
                    defAlloc.deallocate(_Ptr);
                }
                _Ptr = _Y.release();
            }
            return (*this);
        }

        /**
        * @brief Destructor
        *
        * The pointer is deleted in case this class still has ownership
        */
        virtual ~OsclExclusivePtrA()
        {
            defAlloc.deallocate(_Ptr);
        }

        /**
        * @brief The indirection operator (*) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusiveArrayPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T& operator*() const
        {
            return (*get());
        }

        /**
        * @brief The indirection operator (->) accesses a value indirectly,
        * through a pointer
        *
        * This operator ensures that the OsclExclusiveArrayPtr can be used like the
        * regular pointer that it was initialized with.
        */
        T *operator->() const
        {
            return (get());
        }

        /**
        * @brief get() method returns the pointer, currently owned by the class.
        *
        */
        T *get() const
        {
            return (_Ptr);
        }

        /**
        * @brief release() method releases ownership of the pointer, currently owned
        * by the class. It returns the pointer as well.
        *
        */
        T *release()
        {
            T *tmp = _Ptr;
            _Ptr = NULL;
            return (tmp);
        }

        /**
        * @brief set() method sets ownership to the pointer, passed.
        * This method is needed when the class is created with a default
        * constructor. Returns false in case the class is non-empty.
        *
        */
        bool set(T* ptr)
        {
            if ((_Ptr == NULL))
            {
                _Ptr = ptr;
                return true;
            }
            return false;
        }

    private:
        Oscl_TAlloc<T, Alloc> defAlloc;
};

/*! @} */


#endif //OSCL_EXCLUSIVE_PTR_H_INCLUDED

