


#ifndef DATAENTRYURLATOM_H_INCLUDED
#define DATAENTRYURLATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef DATAENTRYATOM_H_INCLUDED
#include "dataentryatom.h"
#endif

class DataEntryUrlAtom : public DataEntryAtom
{

    public:
        DataEntryUrlAtom(MP4_FF_FILE *fp); // Stream-in ctor
        virtual ~DataEntryUrlAtom();

        // Member gets and sets
        OSCL_wString& getLocation()
        {
            return _location;
        }

    private:
        OSCL_wHeapString<OsclMemAllocator> _location;

};

#endif // DATAENTRYURLATOM_H_INCLUDED
