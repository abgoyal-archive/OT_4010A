
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L_ S T R I N G _ R E P
//
//    This file contains a standardized set of string containers that
//    can be used in place of character arrays.

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_STRING_REP_H_INCLUDED
#define OSCL_STRING_REP_H_INCLUDED


#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif



class CHeapRep
{
    public:
        CHeapRep()
                : refcount(0)
                , buffer(NULL)
                , maxsize(0)
                , size(0)
        {}

        OSCL_IMPORT_REF static void set_rep(CHeapRep*&, Oscl_DefAlloc&, const char*, uint32);
        OSCL_IMPORT_REF static void set_rep(CHeapRep*&, Oscl_DefAlloc&, const oscl_wchar*, uint32);
        OSCL_IMPORT_REF static void append_rep(CHeapRep*&, Oscl_DefAlloc&, const char*, uint32);
        OSCL_IMPORT_REF static void append_rep(CHeapRep*&, Oscl_DefAlloc&, const oscl_wchar*, uint32);

        uint32 refcount;
        OsclAny* buffer;
        uint32 maxsize;
        uint32 size;
        OSCL_IMPORT_REF bool set(uint32, const char*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool set(uint32, const oscl_wchar*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool append(uint32, const char*, uint32, const char*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF bool append(uint32, const oscl_wchar*, uint32, const oscl_wchar*, Oscl_DefAlloc &);
        OSCL_IMPORT_REF void add_ref();
        OSCL_IMPORT_REF void remove_ref(Oscl_DefAlloc &);

        OSCL_IMPORT_REF static void assign(CHeapRep*&, CHeapRep*, Oscl_DefAlloc &);
    private:
        static CHeapRep *New(Oscl_DefAlloc &);
};

class CStackRep
{
    public:
        CStackRep()
                : maxsize(0)
                , size(0)
                , buffer(NULL)
        {}
        uint32 maxsize;
        uint32 size;
        OsclAny* buffer;
        OSCL_IMPORT_REF void set(const char* cp, uint32 len);
        OSCL_IMPORT_REF void set(const oscl_wchar* cp, uint32 len);
        OSCL_IMPORT_REF void append(const char* cp, uint32 len);
        OSCL_IMPORT_REF void append(const oscl_wchar* cp, uint32 len);
};


class CFastRep
{
    public:
        CFastRep()
                : maxsize(0)
                , size(0)
                , buffer(NULL)
                , writable(false)
        {}
        uint32 maxsize;
        uint32 size;
        OsclAny* buffer;
        bool writable;
        OSCL_IMPORT_REF void set_w(char* cp, uint32 len, uint32 maxlen);
        OSCL_IMPORT_REF void set_w(oscl_wchar* cp, uint32 len, uint32 maxlen);
        OSCL_IMPORT_REF void set_r(const char* cp, uint32 len);
        OSCL_IMPORT_REF void set_r(const oscl_wchar* cp, uint32 len);
        OSCL_IMPORT_REF void append(const char* cp, uint32 len);
        OSCL_IMPORT_REF void append(const oscl_wchar* cp, uint32 len);
};


#endif   // OSCL_STRING_REP_H_INCLUDED

/*! @} */
