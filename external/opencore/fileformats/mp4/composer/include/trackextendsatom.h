
#ifndef __TrackExtendsAtom_H__
#define __TrackExtendsAtom_H__

#include "fullatom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

class PVA_FF_TrackExtendsAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_TrackExtendsAtom(uint32  mediaType, int32 codecType, uint32 trackId);

        virtual ~PVA_FF_TrackExtendsAtom();

        // To set parameters according to codec
        void    setSampleParameters(uint32 index, uint32 duration, uint32 size, uint32 flags);

        uint32  getTrackId();

        void    setTrackId(uint32 trackId);

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:
        uint32  _mediaType;
        int32   _codecType;

        uint32  _trackId;
        uint32  _defaultSampleDescriptionIndex;
        uint32  _defaultSampleDuration;
        uint32  _defaultSampleSize;
        uint32  _defaultSampleFlags;

        virtual void recomputeSize();

};

#endif
