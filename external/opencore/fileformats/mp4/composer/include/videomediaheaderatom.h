


#ifndef __VideoMediaHeaderAtom_H__
#define __VideoMediaHeaderAtom_H__

#include "mediainformationheaderatom.h"

class PVA_FF_VideoMediaHeaderAtom : public PVA_FF_MediaInformationHeaderAtom
{

    public:
        PVA_FF_VideoMediaHeaderAtom(); // Constructor
        virtual ~PVA_FF_VideoMediaHeaderAtom();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        virtual uint32 getMediaInformationHeaderType() const;

    private:
        virtual void recomputeSize();

        // Static reserved constants
        uint32 _reserved; // = 0 // 8 (64bits)
};



#endif

