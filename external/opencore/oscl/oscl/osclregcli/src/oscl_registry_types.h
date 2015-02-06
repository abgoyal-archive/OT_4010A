
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//              Oscl Registry Types

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_REGISTRY_TYPES_H_INCLUDED
#define OSCL_REGISTRY_TYPES_H_INCLUDED


#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

typedef OsclAny* OsclComponentFactory;

class OsclRegistryAccessElement
{
    public:
        OsclComponentFactory iFactory;
        OSCL_HeapString<OsclMemAllocator> iMimeString;
};



#endif   // OSCL_REGISTRY_TYPES_H_INCLUDED

/*! @} */
