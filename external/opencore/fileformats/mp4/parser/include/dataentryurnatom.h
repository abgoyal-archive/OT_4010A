
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                        MPEG-4 DataEntryUrnAtom Class                          */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef DATAENTRYURNATOM_H_INCLUDED
#define DATAENTRYURNATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef DATAENTRYATOM_H_INCLUDED
#include "dataentryatom.h"
#endif

class DataEntryUrnAtom : public DataEntryAtom
{

    public:
        DataEntryUrnAtom(uint8 version, uint32 flags); // Constructor
        DataEntryUrnAtom(DataEntryUrnAtom atom); // Copy constructor
        virtual ~DataEntryUrnAtom();

        // Member gets and sets
        OSCL_wString& getName()
        {
            return _name;
        }
        OSCL_wString& getLocation()
        {
            return _location;
        }

    private:
        OSCL_wHeapString<OsclMemAllocator> _name;
        OSCL_wHeapString<OsclMemAllocator> _location;

};

#endif // DATAENTRYURNATOM_H_INCLUDED


