
#ifndef __MovieFragmentRandomAccessAtom_H__
#define __MovieFragmentRandomAccessAtom_H__

#include "atom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

#include "trackfragmentrandomaccessatom.h"
#include "moviefragmentrandomaccessoffsetatom.h"


class PVA_FF_MovieFragmentRandomAccessAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_MovieFragmentRandomAccessAtom();

        virtual ~PVA_FF_MovieFragmentRandomAccessAtom();

        void    addTrackFragmentRandomAccessAtom(uint32 trackId);

        PVA_FF_TfraAtom*    getTfraAtom(uint32 trackId);

        void    addSampleEntry(uint32 trackId, uint32 time, uint32 moofOffset,
                               uint32 trafNumber, uint32 trunNumber,
                               uint32 sampleNumber);

        void    updateMoofOffset(uint32 offset);

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:
        PVA_FF_MfroAtom                                     *_pMfroAtom;
        Oscl_Vector <PVA_FF_TfraAtom*, OsclMemAllocator>    *_pTfraList;

        virtual void recomputeSize();
};

#endif
