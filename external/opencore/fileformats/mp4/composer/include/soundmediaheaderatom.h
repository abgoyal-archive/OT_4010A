


#ifndef __SoundMediaHeaderAtom_H__
#define __SoundMediaHeaderAtom_H__

#include "mediainformationheaderatom.h"

class PVA_FF_SoundMediaHeaderAtom : public PVA_FF_MediaInformationHeaderAtom
{

    public:
        PVA_FF_SoundMediaHeaderAtom(); // Constructor
        virtual ~PVA_FF_SoundMediaHeaderAtom();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        virtual uint32 getMediaInformationHeaderType() const;

    private:
        virtual void recomputeSize();

        // Static reserved constants
        uint32 _reserved; // = 0; // 4 (32bits)
};



#endif

