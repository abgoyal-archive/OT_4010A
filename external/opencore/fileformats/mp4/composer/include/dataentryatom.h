

#ifndef __DataEntryAtom_H__
#define __DataEntryAtom_H__

#include "fullatom.h"

class PVA_FF_DataEntryAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_DataEntryAtom(uint32 type, uint8 version, uint32 flags); // Constructor

        virtual ~PVA_FF_DataEntryAtom();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp)
        {
            OSCL_UNUSED_ARG(fp);
            return true;
        }

    private:
        virtual void recomputeSize() {}

};


#endif

