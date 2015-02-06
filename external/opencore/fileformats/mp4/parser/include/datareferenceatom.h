



#ifndef DATAREFERENCEATOM_H_INCLUDED
#define DATAREFERENCEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif
#ifndef DATAENTRYATOM_H_INCLUDED
#include "dataentryatom.h"
#endif
#ifndef DATAENTRYURLATOM_H_INCLUDED
#include "dataentryurlatom.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

class DataReferenceAtom : public FullAtom
{

    public:
        DataReferenceAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~DataReferenceAtom();

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }
        DataEntryAtom *getEntryAt(int32 index);

    private:

        uint32 _entryCount;
        Oscl_Vector<DataEntryUrlAtom*, OsclMemAllocator> *_pdataEntryVec;

};

#endif // DATAREFERENCEATOM_H_INCLUDED

