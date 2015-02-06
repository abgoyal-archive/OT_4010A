
#ifndef __MovieFragmentHeaderAtom_H__
#define __MovieFragmentHeaderAtom_H__

#include "fullatom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

class PVA_FF_MovieFragmentHeaderAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_MovieFragmentHeaderAtom(uint32 sequenceNumber);

        virtual ~PVA_FF_MovieFragmentHeaderAtom();

        uint32 getSequenceNumber();

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:

        uint32  _sequenceNumber;

        virtual void recomputeSize();
};

#endif
