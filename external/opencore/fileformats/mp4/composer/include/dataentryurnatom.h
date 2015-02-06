

#ifndef __DataEntryUrnAtom_H__
#define __DataEntryUrnAtom_H__

#include "dataentryatom.h"

class PVA_FF_DataEntryUrnAtom : public PVA_FF_DataEntryAtom
{

    public:
        PVA_FF_DataEntryUrnAtom(uint8 version, uint32 flags); // Constructor

        virtual ~PVA_FF_DataEntryUrnAtom();

        // Member gets and sets
        OSCL_cString getName()
        {
            return _name;
        }
        void setName(OSCL_cString name)
        {
            _name = name;
        }

        OSCL_cString getLocation()
        {
            return _location;
        }
        void setLocation(OSCL_cString location)
        {
            _location = location;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        OSCL_cString _name;
        OSCL_cString _location;

};


#endif

