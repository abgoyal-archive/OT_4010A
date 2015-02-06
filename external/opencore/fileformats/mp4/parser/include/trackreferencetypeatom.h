
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                     MPEG-4 TrackReferenceTypeAtom Class                       */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef TRACKREFERENCETYPEATOM_H_INCLUDED
#define TRACKREFERENCETYPEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif


class TrackReferenceTypeAtom : public Atom
{

    public:
        TrackReferenceTypeAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~TrackReferenceTypeAtom(); // Destructor

        // Get track reference
        uint32 getTrackReferenceAt(int32 index) const;

    private:
        Oscl_Vector<uint32, OsclMemAllocator> *_trackIDs;
};

#endif // TRACKREFERENCETYPEATOM_H_INCLUDED


