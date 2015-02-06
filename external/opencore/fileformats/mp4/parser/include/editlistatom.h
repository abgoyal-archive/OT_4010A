
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 EditListAtom Class                          */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef EDITLISTATOM_H_INCLUDED
#define EDITLISTATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif


#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif


class EditListAtom : public FullAtom
{

    public:
        EditListAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Constructor
        virtual ~EditListAtom();
        uint32 getInitialTimeOffset();

        uint32 getEntryCount()
        {
            return _entryCount;
        }

    private:
        Oscl_Vector<uint32, OsclMemAllocator> *_psegmentDurations;
        Oscl_Vector<int32, OsclMemAllocator> *_pmediaTimes;
        Oscl_Vector<uint16, OsclMemAllocator> *_pmediaRates;
        Oscl_Vector<uint16, OsclMemAllocator> *_preserveds;

        uint32 _entryCount;
};


#endif // EDITLISTATOM_H_INCLUDED

