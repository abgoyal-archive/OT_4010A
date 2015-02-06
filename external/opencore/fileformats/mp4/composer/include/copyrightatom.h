

#ifndef __CopyrightAtom_H__
#define __CopyrightAtom_H__

#include "fullatom.h"

class PVA_FF_CopyrightAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_CopyrightAtom(uint8 version, uint32 flags); // Constructor

        virtual ~PVA_FF_CopyrightAtom();

        // Member gets and sets
        uint16 getLanguage()
        {
            return _language;
        }
        void setLanguage(uint16 language)
        {
            _language = language;
        }

        OSCL_wString getNotice()
        {
            return _notice;
        }
        void setNotice(OSCL_wString notice)
        {
            _notice = notice;
        }


        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        uint16 _language; // (15bits) padded with a single bit when rendering
        OSCL_wString _notice;

};


#endif

