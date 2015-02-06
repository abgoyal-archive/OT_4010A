
#ifndef __MovieExtendsHeaderAtom_H__
#define __MovieExtendsHeaderAtom_H__

#include "fullatom.h"
#include "a_atomdefs.h"

class PVA_FF_MovieExtendsHeaderAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_MovieExtendsHeaderAtom();
        virtual ~PVA_FF_MovieExtendsHeaderAtom();

        // get and set fragment duration
        uint32 getFragmentDuration(void);

        void setFragmentDuration(uint32 duration);

        void updateMovieFragmentDuration(uint32 ts);

        void writeMovieFragmentDuration(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

        // Rendering the PVA_FF_MovieExtendsHeaderAtom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:
        uint32  _fragmentDuration;

        uint32  _fileOffset;
        bool    _oRendered;

        // functions needed by composer to update and write atoms into file
        virtual void recomputeSize();

};

#endif
