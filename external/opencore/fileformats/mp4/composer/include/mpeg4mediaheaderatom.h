


#ifndef __Mpeg4MediaHeaderAtom_H__
#define __Mpeg4MediaHeaderAtom_H__

#include "mediainformationheaderatom.h"

class PVA_FF_Mpeg4MediaHeaderAtom : public PVA_FF_MediaInformationHeaderAtom
{

    public:
        PVA_FF_Mpeg4MediaHeaderAtom(uint32 mediaType); // Constructor
        virtual ~PVA_FF_Mpeg4MediaHeaderAtom();

        virtual uint32 getMediaInformationHeaderType() const;

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        virtual void recomputeSize();

        uint32 _mediaType; // DO NOT RENDER THIS VALUE!!! (Only for internal purposes)
};


#endif

