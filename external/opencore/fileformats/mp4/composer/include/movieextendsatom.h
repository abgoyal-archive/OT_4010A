
#ifndef __MovieExtendsAtom_H__
#define __MovieExtendsAtom_H__

#include "atom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

#include "movieextendsheaderatom.h"
#include "trackextendsatom.h"


class PVA_FF_MovieExtendsAtom : public PVA_FF_Atom
{

    public:

        PVA_FF_MovieExtendsAtom();              // Constructor
        virtual ~PVA_FF_MovieExtendsAtom();     // destructor


        // Member gets and sets

        // add and retrieve Trex atom for track ID
        PVA_FF_TrackExtendsAtom* getTrexAtom(uint32 trackId);
        void addTrexAtom(PVA_FF_TrackExtendsAtom *pTrexAtom);

        //get movie extend header (to update fragment duration)
        PVA_FF_MovieExtendsHeaderAtom* getMehdAtom();
        void    setMovieFragmentDuration(uint32 duration);
        void    updateMovieFragmentDuration(uint32 ts);
        void    writeMovieFragmentDuration(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);
        uint32  getDuration();

        // Rendering the PVA_FF_MovieExtendsAtom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:

        // child atoms
        PVA_FF_MovieExtendsHeaderAtom*                              _pMehdAtom;
        Oscl_Vector <PVA_FF_TrackExtendsAtom*, OsclMemAllocator>*   _pTrexAtomVec;

        // functions needed by composer to update and write atoms into file
        virtual void recomputeSize();
};

#endif

