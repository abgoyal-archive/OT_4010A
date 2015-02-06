


#ifndef DATAENTRYATOM_H_INCLUDED
#define DATAENTRYATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class DataEntryAtom : public FullAtom
{

    public:
        DataEntryAtom(MP4_FF_FILE *fp); // Stream-in Constructor
        virtual ~DataEntryAtom();

};

#endif // DATAENTRYATOM_H_INCLUDED

