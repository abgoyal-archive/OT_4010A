
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                        MPEG-4 TrackReferenceAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef TRACKREFERENCEATOM_H_INCLUDED
#define TRACKREFERENCEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef TRACKREFERENCETYPEATOM_H_INCLUDED
#include "trackreferencetypeatom.h"
#endif

class TrackReferenceAtom : public Atom
{

    public:
        TrackReferenceAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~TrackReferenceAtom(); // Destructor

        // Member get method
        const TrackReferenceTypeAtom &getTrackReferenceTypeAtom() const
        {
            return *_ptrackReferenceTypeAtom;
        }

        // Assuming a hint track can reference only ONE track
        uint32 getTrackReference() const
        {
            if (_ptrackReferenceTypeAtom != NULL)
            {
                return _ptrackReferenceTypeAtom->getTrackReferenceAt(0);
            }
            else
            {
                return 0;
            }
        }

    private:
        TrackReferenceTypeAtom *_ptrackReferenceTypeAtom;

};

#endif  // TRACKREFERENCEATOM_H_INCLUDED



