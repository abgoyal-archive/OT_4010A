
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               Oscl Registry Access Client

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_REGISTRY_ACCESS_CLIENT_H_INCLUDED
#define OSCL_REGISTRY_ACCESS_CLIENT_H_INCLUDED


#ifndef OSCL_REGISTRY_TYPES_H_INCLUDED
#include "oscl_registry_types.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class OsclRegistryAccessClientImpl;
class OsclRegistryAccessClientTlsImpl;

class OsclRegistryAccessClient
{
    public:
        OSCL_IMPORT_REF OsclRegistryAccessClient();
        OSCL_IMPORT_REF ~OsclRegistryAccessClient();

        /*!
        ** Create a session.
        ** @return OsclErrNone on success.
        */
        OSCL_IMPORT_REF int32 Connect();

        /*!
        ** Lookup a factory by registry and component mime type.
        ** @param aComponent: registry+component MIME type
        ** @return Factory.  Factory will be NULL if not found.
        */
        OSCL_IMPORT_REF OsclComponentFactory GetFactory(OSCL_String& aComponent);


        /*!
        ** Get all factories for a given registry type.
        ** @param aRegistry: registry MIME type
        ** @param aVec: output component factory + mimestring vector.
        */
        OSCL_IMPORT_REF void GetFactories(OSCL_String& aRegistry, Oscl_Vector<OsclRegistryAccessElement, OsclMemAllocator>& aVec);

        /*!
        ** Close and cleanup session.
        */
        OSCL_IMPORT_REF void Close();

    private:
        OsclRegistryAccessClientImpl* iGlobalImpl;
        OsclRegistryAccessClientTlsImpl* iTlsImpl;
};


#endif   // OSCL_STRING_H_INCLUDED

/*! @} */
