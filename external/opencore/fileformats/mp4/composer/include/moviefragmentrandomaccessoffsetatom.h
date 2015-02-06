
#ifndef __MovieFragmentRandomAccessOffsetAtom_H__
#define __MovieFragmentRandomAccessOffsetAtom_H__

#include "fullatom.h"
#include "a_atomdefs.h"
#include "atomutils.h"

class PVA_FF_MfroAtom : public PVA_FF_FullAtom
{
    public:
        PVA_FF_MfroAtom();

        virtual ~PVA_FF_MfroAtom();

        void    setSize(uint32 size);

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP* fp);

    private:
        uint32      _atomSize;

        virtual void recomputeSize();

};

#endif
