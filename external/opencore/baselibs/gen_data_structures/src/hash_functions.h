
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               H A S H F U N C T I O N S   C L A S S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#include "oscl_base.h"
#include "oscl_string.h"
#include "pv_string.h"
// #include "general.h"


inline unsigned char  compute_hash(const char* s)
{
    unsigned long h = 0;
    unsigned char uc = 0;
    for (; *s; ++s)
        h = 5 * h + *s;

    for (int ii = 0; ii < 4; ++ii)
    {
        uc ^= h & 0xFF;
        h >>= 8;
    }
    return uc;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline uint32 compute_hash(const String& str)
{
    return compute_hash(str.get_cstr());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline uint32 compute_hash(const OSCL_String<char>& str)
{
    return compute_hash(str.get_cstr());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

inline uint32 compute_hash(const uint32 value)
{
    return value;
}



#endif
