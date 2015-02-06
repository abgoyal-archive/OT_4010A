
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 Movie Extends Atom Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MOVIEEXTENDSATOM_H_INCLUDED
#define MOVIEEXTENDSATOM_H_INCLUDED

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef MOVIEEXTENDSHEADER_H_INCLUDED
#include "movieextendsheaderatom.h"
#endif

#ifndef TRACKEXTENDSATOM_H_INCLUDED
#include "trackextendsatom.h"
#endif


class MovieExtendsAtom : public Atom
{

    public:
        MovieExtendsAtom(MP4_FF_FILE *fp,
                         uint32 size,
                         uint32 type);
        virtual ~MovieExtendsAtom();

        uint64 getFragmentDuration()
        {
            if (_pMovieExtendsHeaderAtom != NULL)
                return _pMovieExtendsHeaderAtom->getFragmentDuration();
            return 0;
        }

        int32 getDefaultSampleDescriptionIndex();
        int32 getDefaultSampleDuration();
        int32 getDefaultSampleSize();
        int32 getDefaultSampleFlag();
        Oscl_Vector<TrackExtendsAtom*, OsclMemAllocator> *getTrackExtendsAtomVec()
        {
            if (_pTrackExtendsAtomVec != NULL)
                return _pTrackExtendsAtomVec;
            return NULL;
        }


    private:
        MovieExtendsHeaderAtom *_pMovieExtendsHeaderAtom;
        Oscl_Vector<TrackExtendsAtom*, OsclMemAllocator> *_pTrackExtendsAtomVec;

};

#endif


