
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                  O S C L _ M E M

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MEM_H_INCLUDED
#define OSCL_MEM_H_INCLUDED

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_TYPES_H_INCLUDE
#include "oscl_types.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_LOCK_BASE_H_INCLUDED
#include "oscl_lock_base.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

#ifndef OSCL_MEM_INST_H_INCLUDED
#include "oscl_mem_inst.h"
#endif

#ifndef OSCL_HEAPBASE_H_INCLUDED
#include "oscl_heapbase.h"
#endif

//Default for OSCL_HAS_GLOBAL_NEW_DELETE in case it is *not* defined
//in the osclconfig_memory.h
#ifndef OSCL_HAS_GLOBAL_NEW_DELETE
#ifdef NDEBUG
//Release Mode - No definition for global new and delete.
#define OSCL_HAS_GLOBAL_NEW_DELETE 0
#else
//Debug Mode - Define global new and delete.
#define OSCL_HAS_GLOBAL_NEW_DELETE 1
#endif //NDEBUG
#endif //OSCL_HAS_GLOBAL_NEW_DELETE

class OsclMem
{
    public:
        /** Per-thread initialization of Oscl Memory
        **   @param lock: A lock class for use with multi-threaded applications.
        **      The lock is needed in use cases where memory may be allocated
        **      in one thread and freed in another.  In this case, there must
        **      be a single lock object, and its pointer must be passed to
        **      the OsclMem::Init call in each thread.
        **      If no lock is provided, the memory manager will not be thread-safe.
        ** @exception: Leaves on error
        */
        OSCL_IMPORT_REF static void Init();

        /** Per-thread cleanup of Oscl Memory
        ** @exception: Leaves on error;
        */
        OSCL_IMPORT_REF static void Cleanup();

};

#include "oscl_base.h"

#if (OSCL_BYPASS_MEMMGT)
//empty class for compilation only
class OsclAuditCB
{
    public:
};
#else
class OsclMemStatsNode;
class OsclMemAudit;
class OsclAuditCB
{
    public:
        const OsclMemStatsNode* pStatsNode;
        OsclMemAudit *pAudit;

        OsclAuditCB() :
                pStatsNode(NULL),
                pAudit(NULL)
        {}

        OsclAuditCB(const OsclMemStatsNode* myStatsNode,
                    OsclMemAudit *ptr)
                :
                pStatsNode(myStatsNode),
                pAudit(ptr)
        {
        }
};
#endif//OSCL_BYPASS_MEMMGT

OSCL_COND_IMPORT_REF uint oscl_mem_aligned_size(uint size);

OSCL_IMPORT_REF void OsclMemInit(OsclAuditCB & auditCB);

#define OSCL_CLEANUP_BASE_CLASS(T) _OSCL_CLEANUP_BASE_CLASS(T)


#if(OSCL_BYPASS_MEMMGT)
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate(1)) T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate_fl(1,__FILE__,__LINE__)) T params
#else
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate(1)) T params
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate(1),T_allocator.deallocate,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate_fl(1,__FILE__,__LINE__),T_allocator.deallocate,T_ptr,T,params)
#else
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate(1),T_allocator.deallocate,T_ptr,T,params)
#endif

#define OSCL_ALLOC_DELETE(ptr, T_allocator, T) \
  {\
  ptr->~T();\
  T_allocator.deallocate(ptr);\
  }



//These are for internal use but need to be visible since they're used
//in macros.
#if(!OSCL_BYPASS_MEMMGT)
OSCL_IMPORT_REF void* _oscl_audit_malloc(size_t , OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_calloc(size_t , size_t, OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_realloc(void*, size_t , OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_new(size_t , OsclAuditCB & , const char * f = NULL, const int l = 0) ;
OSCL_IMPORT_REF void* _oscl_default_audit_malloc(size_t , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_calloc(size_t , size_t, const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_realloc(void*, size_t , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_new(size_t , const char * f = NULL, const int l = 0) ;
OSCL_IMPORT_REF void _oscl_audit_free(void *);
#else
OSCL_IMPORT_REF void* _oscl_default_new(size_t nBytes);
#endif//OSCL_BYPASS_MEMMGT

#if (OSCL_HAS_GLOBAL_NEW_DELETE)
//Global New operator overloaded to check native new operators called

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new(size_t aSize, const char *aFile, int aLine)
{
#if(PVMEM_INST_LEVEL>0)
    //in case NULL is passed in, record this file & line #
    if (!aFile)
        return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
    return _oscl_default_audit_new(aSize, aFile, aLine);
};
#endif

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new(size_t aSize)
{
#if(PVMEM_INST_LEVEL>0)
    return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#else
    return _oscl_default_audit_new(aSize);
#endif
};
#endif

#if(!OSCL_BYPASS_MEMMGT)
inline void operator delete(void *aPtr)
{
    _oscl_audit_free(aPtr);
};
#endif

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new[](size_t aSize, const char *aFile, int aLine)
{
#if(PVMEM_INST_LEVEL>0)
    //in case NULL is passed in, record this file & line #
    if (!aFile)
        return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
    return _oscl_default_audit_new(aSize, aFile, aLine);
};
#endif

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new[](size_t aSize)
{
#if(PVMEM_INST_LEVEL>0)
    return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#else
    return _oscl_default_audit_new(aSize);
#endif
};
#endif

#if(!OSCL_BYPASS_MEMMGT)
inline void operator delete[](void *aPtr)
{
    _oscl_audit_free(aPtr);
};
#endif
#endif //OSCL_HAS_GLOBAL_NEW_DELETE

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_MALLOC(count) _oscl_malloc(count)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_MALLOC(count) _oscl_default_audit_malloc(count,__FILE__,__LINE__)
#else
#define OSCL_MALLOC(count) _oscl_default_audit_malloc(count)
#endif

#define oscl_malloc(a) OSCL_MALLOC(a)

#define OSCL_DEFAULT_MALLOC(x) OSCL_MALLOC(x)

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_malloc(count)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_audit_malloc(count, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_audit_malloc(count, auditCB)
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_CALLOC(num,size) _oscl_calloc(num,size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_CALLOC(num,size) _oscl_default_audit_calloc(num,size,__FILE__,__LINE__)
#else
#define OSCL_CALLOC(num,size) _oscl_default_audit_calloc(num,size)
#endif

#define oscl_calloc(a,b) OSCL_CALLOC(a,b)

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_calloc(num,size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_audit_calloc(num,size, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_audit_calloc(num,size, auditCB)
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_REALLOC(ptr,new_size) _oscl_realloc(ptr,new_size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_REALLOC(ptr,new_size) _oscl_default_audit_realloc(ptr,new_size,__FILE__,__LINE__)
#else
#define OSCL_REALLOC(ptr,new_size) _oscl_default_audit_realloc(ptr,new_size)
#endif

#define oscl_realloc(a,b) OSCL_REALLOC(a,b)

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_realloc(ptr,new_size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_audit_realloc(ptr,new_size, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_audit_realloc(ptr,new_size, auditCB)
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_FREE(ptr) _oscl_free(ptr)
#else
#define OSCL_FREE(ptr) _oscl_audit_free(ptr)
#endif

#define oscl_free(x) OSCL_FREE(x)

#define OSCL_DEFAULT_FREE(x) OSCL_FREE(x)


#if(OSCL_BYPASS_MEMMGT)
#define OSCL_NEW( T, params) new T params
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_NEW( T, params) new T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_NEW( T, params) new(__FILE__,__LINE__) T params
#else
#define OSCL_NEW( T, params) new T params
#endif

#define OSCL_PLACEMENT_NEW(ptr, constructor) new(ptr) constructor

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_new(sizeof(T)),_oscl_free,T_ptr,T,params)
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T)),_oscl_audit_free,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T),__FILE__,__LINE__),_oscl_audit_free,T_ptr,T,params)
#else
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T)),_oscl_audit_free,T_ptr,T,params)
#endif


#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_default_new(sizeof(T))) T params
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB)) T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB,__FILE__,__LINE__)) T params
#else
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB)) T params
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_default_new(sizeof(T)),_oscl_free,T_ptr,T,params)
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB),_oscl_audit_free,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB,__FILE__,__LINE__),_oscl_audit_free,T_ptr,T,params)
#else
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB),_oscl_audit_free,T_ptr,T,params)
#endif

#define OSCL_DELETE(ptr) {\
    if(ptr){delete(ptr);}\
}



#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_default_new(sizeof(T)*(count))) T
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB)) T
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB,__FILE__,__LINE__)) T
#else
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB)) T
#endif

#if(OSCL_BYPASS_MEMMGT)
#define OSCL_ARRAY_NEW(T, count) new T[count]
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_ARRAY_NEW(T, count) new T[count]
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_ARRAY_NEW(T, count) new(__FILE__,__LINE__) T[count]
#else
#define OSCL_ARRAY_NEW(T, count) new T[count]
#endif

#define OSCL_ARRAY_DELETE(ptr) delete [] ptr



#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_REFCOUNTER_H_INCLUDED
#include "oscl_refcounter.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H_INCLUDED
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_ERROR_H_INCLUDED
#include "oscl_error.h"
#endif

#ifndef OSCL_EXCEPTION_H_INCLUDED
#include "oscl_exception.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

class OsclMemAllocator : public Oscl_DefAlloc
{
    public:
        /** This API throws an exception when malloc returns NULL.
          * n must be greater than 0.
          *
          * @return pointer (or Leave with OsclErrNoMemory )
          *
          */
        OsclAny* allocate(const uint32 n)
        {
#if(OSCL_BYPASS_MEMMGT)
            OsclAny* p = _oscl_malloc(n);
            if (!p)
                OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
#elif (PVMEM_INST_LEVEL>0)
            //this is really a usage error-- caller should provide file & line.
            //set a debug breakpoint here...
            return allocate_fl(n, __FILE__, __LINE__);
#else
            return allocate_fl(n, NULL, 0);
#endif
        }

#if(!OSCL_BYPASS_MEMMGT)
        OsclAny* allocate_fl(const uint32 n, const char * file_name, const int line_num)
        {
            OsclAny* p = _oscl_default_audit_malloc(n, file_name, line_num);
            if (!p)
                OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
        }
#endif

        void deallocate(OsclAny* p)
        {
            if (p)
                OSCL_FREE(p);
        }
};


class OsclMemBasicAllocator : public Oscl_DefAlloc
{
    public:
        /** This API throws an exception when malloc returns NULL.
          * n must be greater than 0.
          *
          * @return pointer (or Leave with OsclErrNoMemory )
          *
          */
        OsclAny* allocate(const uint32 n)
        {
            OsclAny* p = _oscl_malloc(n);
            OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
        }

        void deallocate(OsclAny* p)
        {
            if (p)
                _oscl_free(p);
        }
};

template <class T> class OsclMemAllocDestructDealloc : public OsclAllocDestructDealloc
{
    public:
#if !(OSCL_BYPASS_MEMMGT)
        OsclAny* allocate_fl(const uint32 size, const char * file_name, const int line_num)
        {
            return alloc.allocate_fl(size, file_name, line_num);
        }
#endif
        OsclAny* allocate(const uint32 size)
        {
#if(OSCL_BYPASS_MEMMGT)
            return alloc.allocate(size);
#elif(PVMEM_INST_LEVEL>0)
            //this is really a usage error-- caller should provide file & line.
            //set a debug breakpoint here...
            return allocate_fl(size, __FILE__, __LINE__);
#else
            return allocate_fl(size, NULL, 0);
#endif
        }
        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }
        void destruct_and_dealloc(OsclAny* p)
        {
            T* ptr = reinterpret_cast<T*>(p);
            ptr->~T();
            deallocate(p);
            OSCL_UNUSED_ARG(ptr); // removes warning on some compilers
        }
    private:
        OsclMemAllocator alloc;
};

template <class T> class OsclMemBasicAllocDestructDealloc : public OsclAllocDestructDealloc
{
    public:
        OsclAny* allocate(const uint32 size)
        {
#if(OSCL_BYPASS_MEMMGT)
            return alloc.allocate(size);
#else
            return alloc.allocate_fl(size, NULL, 0);
#endif
        }
        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }
        void destruct_and_dealloc(OsclAny* p)
        {
            T* ptr = reinterpret_cast<T*>(p);
            ptr->~T();
            deallocate(p);
            OSCL_UNUSED_ARG(ptr); // removes warning on some compilers
        }
    private:
        OsclMemBasicAllocator alloc;
};


class OsclMemAudit;
class OsclMemGlobalAuditObject
{
    public:
        typedef OsclMemAudit audit_type;
        /**
         * returns the global audit object.  For use
         * in macros only-- not a public API.
         */
        OSCL_IMPORT_REF static audit_type* getGlobalMemAuditObject();

    private:
        /**
         * creates the global audit object
         */
        static void createGlobalMemAuditObject();

        /**
         * deletes the global audit object
         */
        static void deleteGlobalMemAuditObject();

        friend class OsclMem;
};


class HeapBase : public _OsclHeapBase
{
    public:
#if (OSCL_HAS_HEAP_BASE_SUPPORT)

#if(!OSCL_BYPASS_MEMMGT)
        static void* operator new(size_t aSize, const char *aFile = NULL, const int aLine = 0)
        {
#if(PVMEM_INST_LEVEL>0)
            //in case NULL is passed in, record this file & line #
            if (!aFile)
                return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
            return _oscl_default_audit_new(aSize, aFile, aLine);
        }
#else
        static void* operator new(size_t aSize)
        {
            return _oscl_default_new(aSize);
        }
#endif

        static void* operator new[](size_t aSize)
        {
#if(!OSCL_BYPASS_MEMMGT)
            return _oscl_default_audit_new(aSize);
#else
            return _oscl_default_new(aSize);
#endif
        }

        static void* operator new[](size_t aSize, const char *aFile = NULL, const int aLine = 0)
        {
#if(!OSCL_BYPASS_MEMMGT)
#if(PVMEM_INST_LEVEL>0)
            //in case NULL is passed in, record this file & line #
            if (!aFile)
                return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
            return _oscl_default_audit_new(aSize, aFile, aLine);
#else
            OSCL_UNUSED_ARG(aFile);
            OSCL_UNUSED_ARG(aLine);
            return _oscl_default_new(aSize);
#endif
        }

        static void* operator new(size_t aSize, void *aPtr)
        {
            return aPtr;
        }

        static void operator delete(void* aPtr)
        {
#if(!OSCL_BYPASS_MEMMGT)
            _oscl_audit_free(aPtr);
#else
            _oscl_free(aPtr);
#endif
        }

        static void operator delete[](void* aPtr)
        {
#if(!OSCL_BYPASS_MEMMGT)
            _oscl_audit_free(aPtr);
#else
            _oscl_free(aPtr);
#endif
        }
#endif //OSCL_HAS_HEAP_BASE_SUPPORT
        HeapBase() {};
        virtual ~HeapBase() {};
};

#define _OSCL_TRAP_NEW(exp,freeFunc,T_ptr,T,params)\
{\
    int32 __err;\
    OsclAny*__ptr=exp;\
    OSCL_TRY(__err,T_ptr=new(__ptr) T params;);\
    if(__err){\
        freeFunc(__ptr);\
        T_ptr=NULL;\
        OsclError::Leave(__err);\
    }\
}

#ifdef PVERROR_IMP_CPP_EXCEPTIONS
//when using C++ exceptions, base class cleanup is automatic
#define _OSCL_CLEANUP_BASE_CLASS(T)
#else
//otherwise the destructor needs to be called explicitly.
#define _OSCL_CLEANUP_BASE_CLASS(T) this->T::~T()
#endif


/*! @} */


#if (!OSCL_DISABLE_INLINES)
#include "oscl_mem.inl"
#endif

#endif // OSCL_MEM_H_INCLUDED



/*! @} */

