
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               Oscl Registry Serv Impl

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =



#ifndef OSCL_REGISTRY_SERV_IMPL_H_INCLUDED
#define OSCL_REGISTRY_SERV_IMPL_H_INCLUDED

#include "oscl_base.h"
#include "osclconfig_proc.h"

#include "oscl_registry_types.h"


#include "oscl_string.h"
#include "oscl_vector.h"
#include "oscl_mem.h"

class OsclComponentRegistryElement
{
    public:
        OsclComponentRegistryElement(OSCL_String&, OsclComponentFactory);
        OsclComponentRegistryElement(const OsclComponentRegistryElement&);
        OsclComponentRegistryElement& operator=(const OsclComponentRegistryElement& src);
        ~OsclComponentRegistryElement();

        bool Match(OSCL_String& aStr, bool aExact);

        OSCL_String* iId;//use a pointer here instead of a container for efficiency
        //since this element is used in an oscl vector.
        OsclComponentFactory iFactory;
        uint32 iComponentId;
};


class OsclComponentRegistryData
{
    public:
        OsclComponentRegistryElement* Find(OSCL_String&, bool aExact);
        Oscl_Vector<OsclComponentRegistryElement, OsclMemAllocator> iVec;
};

#include "oscl_mutex.h"
class OsclComponentRegistry
{
    public:
        OsclComponentRegistry();
        ~OsclComponentRegistry();
        int32 Register(uint32& aId, OSCL_String&, OsclComponentFactory);
        int32 Unregister(OSCL_String&);
        int32 Unregister(uint32);
        OsclComponentFactory FindExact(OSCL_String&);
        void FindHierarchical(OSCL_String& , Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>&);
        void OpenSession();
        void CloseSession();

        OsclComponentRegistryData iData;
        OsclMutex iMutex;

        uint32 iComponentIdCounter;

        uint32 iNumSessions;
};


#endif //OSCL_REGISTRY_IMPL_H_INCLUDED
/*! @} */


