
#ifndef __MovieFragmentAtom_H__
#define __MovieFragmentAtom_H__

#include "atom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

#include "moviefragmentheaderatom.h"
#include "trackfragmentatom.h"

class PVA_FF_MovieFragmentAtom : public PVA_FF_Atom
{

    public:
        // constructor and destructor
        PVA_FF_MovieFragmentAtom(uint32 sequenceNumber, uint32 movieFragmentDuration,
                                 uint32 interleaveDuration);
        virtual ~PVA_FF_MovieFragmentAtom();

        // Add and get track fragment
        void    addTrackFragment(uint32 mediaType, uint32 codecType, uint32 trackId,
                                 uint32 timescale);

        PVA_FF_TrackFragmentAtom* getTrackFragment(uint32 trackId);


        void    addSampleToFragment(uint32 trackId, uint32 size,
                                    uint32 ts, uint8 flags, uint32 baseOffset, bool otrunStart);

        int32    getTrackFragmentDuration(uint32 trackId);


        uint32   getSequenceNumber();

        void    setTrackFragmentHeaderData(uint32 trackId, uint32 flags);

        void    setTrackFragmentBaseDataOffset(uint32 trackId, uint32 offset);

        uint32  getTrackFragmentNumber(uint32 trackId);

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

        int32   getMaxTrackDuration();


    private:

        uint32  _movieFragmentDuration;     // determine the duration of samples in this movie fragment
        uint32  _interleaveDuration;        // determine the duration of samples in each trun

        uint32  _fragmentDuration;          // determines the duration of samples in this fragment


        PVA_FF_MovieFragmentHeaderAtom                              *_pMfhdAtom;
        Oscl_Vector <PVA_FF_TrackFragmentAtom*, OsclMemAllocator>   *_pTrafList;    // list of TRAF corresponding to tracks

        virtual void recomputeSize();
};

#endif
