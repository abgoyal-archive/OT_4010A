
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ R A N D

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_RAND_H_INCLUDED
#define OSCL_RAND_H_INCLUDED

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

class OsclRand
{
    public:
        OSCL_COND_IMPORT_REF void Seed(int32 seed);
        OSCL_COND_IMPORT_REF int32 Rand();
    private:
};

#if (!OSCL_DISABLE_INLINES)
#include "oscl_rand.inl"
#endif


#endif

/*! @} */
