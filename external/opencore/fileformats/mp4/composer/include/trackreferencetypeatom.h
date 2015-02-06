


#ifndef __TrackReferenceTypeAtom_H__
#define __TrackReferenceTypeAtom_H__

#include "atom.h"

class PVA_FF_TrackReferenceTypeAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_TrackReferenceTypeAtom(uint32 refType); // Constructor
        virtual ~PVA_FF_TrackReferenceTypeAtom(); // Destructor

        // Add track reference and return the index of the reference in the table - 1-based
        int32 addTrackReference(uint32 ref);
        uint32 getTrackReferenceAt(int32 index) const;

        // Get vector of track references
        Oscl_Vector<uint32, OsclMemAllocator>* getReferencedTrackIDVec()
        {
            return _trackIDs;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        virtual void recomputeSize();

        Oscl_Vector<uint32, OsclMemAllocator>* _trackIDs;

};



#endif

