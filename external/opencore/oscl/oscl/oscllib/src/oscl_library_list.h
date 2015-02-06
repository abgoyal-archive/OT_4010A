
#ifndef OSCL_LIBRARY_LIST_H_INCLUDED
#define OSCL_LIBRARY_LIST_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_LIBRARY_COMMON_H_INCLUDED
#include "oscl_library_common.h"
#endif

struct OsclUuid;
class PVLogger;

class OsclLibraryList
{
    public:
        /**
         * Default object Constructor function
         **/
        OSCL_IMPORT_REF OsclLibraryList();

        /**
         * Object destructor function
         **/
        OSCL_IMPORT_REF ~OsclLibraryList();

        /**
         * Populate the list for the given interface ID.
         *
         * @param aInterfaceId ID to find the list of libraries for.
         * @param aConfigFile - DLL Config file
         *
         * @returns OsclLibStatus about whether the librarylist vector got populated or not
         **/
        OSCL_IMPORT_REF OsclLibStatus Populate(const OsclUuid& aInterfaceId, const OSCL_String& aConfigFile);

        /**
        * Get the number of libraries in the list.
        *
        * @returns Number of libraries in list.
        */
        OSCL_IMPORT_REF uint32 Size();

        /**
         * Returns the n'th element.
         * @param n element position to return
         * @returns the reference to the library path stored in the iLibList vector
         */
        OSCL_IMPORT_REF const OSCL_String& GetLibraryPathAt(uint32 n);

    private:
        Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> iLibList;
        PVLogger* ipLogger;
#if OSCL_LIBRARY_PERF_LOGGING
        PVLogger* iDiagnosticsLogger;
        uint32 iLibHit;
        uint32 iLinesRead;
#endif
};

#endif //OSCL_LIBRARY_LIST_H_INCLUDED

