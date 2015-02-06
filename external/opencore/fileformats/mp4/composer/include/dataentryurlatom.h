

#ifndef __DataEntryUrlAtom_H__
#define __DataEntryUrlAtom_H__

#include "dataentryatom.h"

class PVA_FF_DataEntryUrlAtom : public PVA_FF_DataEntryAtom
{

    public:
        PVA_FF_DataEntryUrlAtom(uint32 selfContained); // Constructor
        virtual ~PVA_FF_DataEntryUrlAtom();

        // Member gets and sets
        PVA_FF_UTF8_STRING_PARAM getLocation() const
        {
            return _location;
        }
        void setLocation(PVA_FF_UTF8_STRING_PARAM location)
        {
            _location = location;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        virtual void recomputeSize();

        PVA_FF_UTF8_HEAP_STRING _location;

};


#endif

